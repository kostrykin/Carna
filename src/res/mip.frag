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
uniform sampler2D frontFacesTexture;
uniform sampler2D backFacesTexture;

uniform int sample_count;

uniform int min_huv;
uniform int max_huv;


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
    return texture3D( intensityTexture, a ).r;
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
        discard;
    }

    vec3 ray_direction = normalize( ray_volume_exit_point - ray_volume_entry_point );
    float path_step_length = ray_length / ( sample_count - 1 );
    vec3 path_step = ray_direction * path_step_length;

    float i0_max = -1024.0;

    int max_sample_count = int( distance( ray_volume_entry_point, ray_volume_exit_point ) / path_step_length );
    int real_sample_count = min( sample_count, max_sample_count );
    
    for( int i = 0; i < real_sample_count; ++i )
    {
        vec3 a = ray_volume_entry_point + i * path_step;

        float i1 = getIntensityAt( a );
        float i0 = i1 * 4096 - 1024;
        
        if( i0 > min_huv && i0 < max_huv )
        {
            i0_max = max( i0, i0_max );
        }
    }
    
    float result = ( i0_max - min_huv ) / ( max_huv - min_huv );
    
    gl_FragColor = vec4( result, result, result, 1.0 );
}
