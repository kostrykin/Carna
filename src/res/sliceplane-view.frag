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

uniform sampler2D planeTexture;

void main()
{
    vec4 rgba = texture2D( planeTexture, gl_TexCoord[ 0 ].xy );
        
    if( rgba.a == 0 )
    {
        discard;
    }
    else
    {
        gl_FragColor = rgba;
    }
}
