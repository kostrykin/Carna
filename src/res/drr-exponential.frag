#version 330

/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

uniform sampler2D integralMap;
uniform float     baseIntensity;
uniform int       renderInverse;

in vec2 textureCoordinates;

out vec4 gl_FragColor;


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    float integral = texture( integralMap, textureCoordinates ).r;
    float result   = clamp( baseIntensity * exp( -integral ), 0, 1 );

    if( renderInverse != 0 )
    {
        result = 1 - result;
    }

    gl_FragColor = vec4( 1, 1, 1, result );
}
