#version 120

/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

uniform sampler3D intensityTexture;
uniform sampler3D maskTexture;
uniform sampler2D frontFacesTexture;
uniform sampler2D backFacesTexture;

uniform int sample_count;
uniform float mu_water;
uniform float brightness;
uniform float lower_threshold;
uniform float upper_threshold;
uniform float upper_multiplier;
uniform float window_level;
uniform float window_width;
uniform float mask_bias;
uniform float mask_contrast;

uniform vec3 proportions;


// ----------------------------------------------------------------------------------
// Helper Functions
// ----------------------------------------------------------------------------------

int min( int x, int y )
{
    return x < y ? x : y;
}


// ----------------------------------------------------------------------------------
// Basic Sampling
// ----------------------------------------------------------------------------------

float getIntensityAt( vec3 a )
{
    float mask_value = texture3D( maskTexture, a ).r + mask_bias;
    float intensity  = texture3D( intensityTexture, a ).r;

    return intensity * max( 0.0, ( mask_value - 1.0 ) * mask_contrast + 1.0 );
}


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    vec4 front_texture_sample = texture2D( frontFacesTexture, gl_TexCoord[ 0 ].st );
    vec4 back_texture_sample = texture2D( backFacesTexture, gl_TexCoord[ 0 ].st );
    
    vec3 ray_volume_entry_point = front_texture_sample.xyz;
    vec3 ray_volume_exit_point = back_texture_sample.xyz;
    float ray_length = distance( ray_volume_entry_point, ray_volume_exit_point );

    if( front_texture_sample.a < 0.01
     || back_texture_sample.a < 0.01
     || ray_length < 0.01 )
    {
        gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );
    }
    else
    {
        vec3 ray_direction = normalize( ray_volume_exit_point - ray_volume_entry_point );
        float path_step_length = ray_length / ( sample_count - 1 );
        vec3 path_step = ray_direction * path_step_length;

        int max_sample_count = int( distance( ray_volume_entry_point, ray_volume_exit_point ) / path_step_length );
        int real_sample_count = min( sample_count, max_sample_count );

        float integral_mu = 0.0;
        float dx = path_step_length * length( ray_direction * proportions );

        for( int k = 0; k < real_sample_count; ++k )
        {
            float i = getIntensityAt( ray_volume_entry_point + k * path_step );

            if( i < lower_threshold )
            {
                continue;
            }
            else
            if( i >= upper_threshold )
            {
                i *= upper_multiplier;
            }

            float huv = i * 4096.0 - 1024.0;
            float mu = mu_water * ( 1.0 + huv / 1000.0 );
            integral_mu += mu * dx;
        }

        float result = brightness * ( exp( -integral_mu ) );

        result = ( result - window_level + window_width / 2 ) / window_width;

        gl_FragColor = vec4( result, result, result, 1.0 );
    }
}
