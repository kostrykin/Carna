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
uniform sampler1D colorMap;
uniform sampler2D frontFacesTexture;
uniform sampler2D backFacesTexture;

uniform int sample_count;

uniform float light_amplitude;
uniform float light_bias;
uniform float light_back;

uniform float pickingSaturation;
uniform vec3 gradient_sample_distance;

uniform int draw_mode;

uniform mat4 objectToScreenSpace;


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

float getIntensityAt( in vec3 a )
{
    return texture3D( intensityTexture, a ).r;
}


vec4 getSampleAt( in vec3 a )
{
    float i = getIntensityAt( a );
    return texture1D( colorMap, i );
}


// ----------------------------------------------------------------------------------
// Light Atternuation
// ----------------------------------------------------------------------------------

vec3 getNormal( in vec3 a )
{
    vec3 dx = vec3( 1, 0, 0 ) * gradient_sample_distance.x;
    vec3 dy = vec3( 0, 1, 0 ) * gradient_sample_distance.y;
    vec3 dz = vec3( 0, 0, 1 ) * gradient_sample_distance.z;
    
    vec3 gradient;
    
    gradient.x = ( getIntensityAt( a + dx ) - getIntensityAt( a - dx ) ) / ( 2 * dx.x );
    gradient.y = ( getIntensityAt( a + dy ) - getIntensityAt( a - dy ) ) / ( 2 * dy.y );
    gradient.z = ( getIntensityAt( a + dz ) - getIntensityAt( a - dz ) ) / ( 2 * dz.z );
    
    return normalize( -gradient );
}


float getLightAmount( in vec3 a, vec3 light_direction )
{
    if( light_bias == 1.0 )
    {
        return 1.0;
    }
    
    vec3 surface_normal = getNormal( a );

    if( length( surface_normal ) < 1e-4 )
    {
        return 1.0;
    }
    
    float dot_result = dot( light_direction, -surface_normal );
    float light_impact = max( 0.0, dot_result ) + light_back * max( 0.0, -dot_result );
    return light_amplitude * max( 0.0, light_impact - light_bias ) + light_bias;
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

    float opacity = 1.0;
    
    vec3 result = vec3( 0, 0, 0 );
    vec3 solid_voxel_coord = ray_volume_entry_point;
    bool solid_voxel_coord_set = false;

    int max_sample_count = int( distance( ray_volume_entry_point, ray_volume_exit_point ) / path_step_length );
    int real_sample_count = min( sample_count, max_sample_count );
    
    for( int i = 0; i < real_sample_count; ++i )
    {
        vec3 a = ray_volume_entry_point + i * path_step;
        
        if( opacity < pickingSaturation && !solid_voxel_coord_set )
        {
            solid_voxel_coord = a;
            solid_voxel_coord_set = true;
        }
        
        if( opacity < 0.01 )
        {
            break;
        }
        
        vec4 sample = getSampleAt( a );
        sample = vec4( sample.rgb * getLightAmount( a, ray_direction ), sample.a );
        
        
     /* The utilized formula for sampling rate dependent alpha value adjustment was
      * token from NVidia GPU Gems 1:
      *
      * http://http.developer.nvidia.com/GPUGems/gpugems_ch39.html#ch39equ03
      */
        
        float scaled_alpha = 1 - pow( 1 - sample.a, 256.0 / sample_count );
        result += sample.rgb * opacity * scaled_alpha;
        opacity *= 1 - scaled_alpha;
    }

 // write to frame buffer

    if( draw_mode == 0 )
    {

     // write colors

        gl_FragData[ 0 ] = vec4( result, 1.0 );
        if( solid_voxel_coord_set )
        {
            vec4 transformed_coord = objectToScreenSpace * vec4( solid_voxel_coord, 1.0 );
            float ndcDepth = transformed_coord.z / transformed_coord.w;
            gl_FragDepth =
                    ( ( gl_DepthRange.diff * ndcDepth ) + gl_DepthRange.near + gl_DepthRange.far ) / 2.0;
        }
        else
        {
            gl_FragDepth = 1.0;
        }
    }
    else
    {

     // write normals

        if( solid_voxel_coord_set )
        {
            gl_FragData[ 0 ] = vec4( getNormal( solid_voxel_coord ), 1.0 );
        }
        else
        {
            gl_FragData[ 0 ] = vec4( 0.0, 0.0, 0.0, 0.0 );
        }
    }

 // write screen space to object space map, used for picking

    gl_FragData[ 1 ] = vec4( solid_voxel_coord, solid_voxel_coord_set ? 1.0 : 0.0 );
}
