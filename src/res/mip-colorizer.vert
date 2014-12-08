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

uniform float min_color_r;
uniform float min_color_g;
uniform float min_color_b;
uniform float min_color_a;

uniform float max_color_r;
uniform float max_color_g;
uniform float max_color_b;
uniform float max_color_a;

varying vec4 color;
varying vec4 color_bias;

void main()
{
    gl_Position = ftransform();
    gl_TexCoord[ 0 ] = gl_MultiTexCoord0;
    
    color = vec4( max_color_r - min_color_r
                , max_color_g - min_color_g
                , max_color_b - min_color_b
                , max_color_a - min_color_a );
                
	color_bias = vec4( min_color_r, min_color_g, min_color_b, min_color_a );
}
