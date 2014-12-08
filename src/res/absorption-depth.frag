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

uniform sampler2D object_space;
uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;
uniform float z_shift;


void main()
{
    vec4 object_space_sample = texture2D( object_space, gl_TexCoord[ 0 ].st );
    
    vec4 v_objectspace = vec4( object_space_sample.rgb, 1 );
    
    vec4 v_viewspace = modelview_matrix * v_objectspace;
    
    vec4 v_screenspace = projection_matrix * v_viewspace;
    
    float z = ( max( 1.0 - ( v_screenspace.z / v_screenspace.w ) - z_shift, 0.0 ) / ( 1 - z_shift ) ) * object_space_sample.a;
    
    gl_FragColor = vec4( z, z, z, 1.0 );
}
