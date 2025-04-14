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

uniform sampler2D colorMap;
uniform int mod2result;

in vec2 textureCoordinates;

layout( location = 0 ) out vec4 _gl_FragColor;


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    if( mod( int( gl_FragCoord.y + 0.5 ), 2 ) == mod2result )
    {
        vec4 color = texture( colorMap, textureCoordinates );
        _gl_FragColor = color;
    }
    else
    {
        discard;
    }
}
