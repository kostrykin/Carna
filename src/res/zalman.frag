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

uniform sampler2D sceneTexture;
uniform int mod2result;

void main()
{
    vec3 color = texture2D( sceneTexture, gl_TexCoord[ 0 ].st ).rgb;
    if( mod( int( gl_FragCoord.y + 0.5 ), 2 ) == mod2result )
    {
		gl_FragColor = vec4( color, 1 );
	}
	else
	{
		discard;
	}
}
