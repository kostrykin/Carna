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

uniform sampler2D maxIntensityTexture;

varying vec4 color;
varying vec4 color_bias;

void main()
{
	vec4 sample = texture2D( maxIntensityTexture, vec2( gl_TexCoord[ 0 ] ) );
    
    float lambda = sample.r;
    
	if( sample.a < 1e-4 || lambda < 0 || lambda > 1 ) discard;
    
    gl_FragColor = color_bias + lambda * color;
}
