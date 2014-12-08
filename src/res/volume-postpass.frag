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
uniform int inversion;
uniform int mask;
uniform int width;
uniform int height;

float sq( float x )
{
    return x * x;
}

float isInside( float x, float y )
{
    float r = max( float( width ), float( height ) ) / 2;
    float a = min( float( height ) / width, 1.0 );
    float b = min( float( width ) / height, 1.0 );
    
    return sq( a * x ) + sq( b * y ) - r;
}

void main()
{
    vec3 color = texture2D( sceneTexture, gl_TexCoord[ 0 ].st ).rgb;
    if( inversion == 2 )
    {
        color.r = 1.0 - color.r;
        color.g = 1.0 - color.g;
        color.b = 1.0 - color.b;
    }
    else
    if( inversion == 1 )
    {
        if( sq( color.r - color.g ) + sq( color.g - color.b ) < sq( 0.01 ) * 2 )
        {
            color.r = color.g = color.b = 1.0 - color.r;
        }
    }
    if( mask == 1 )
    {
        int short_side = int( min( float( width ), float( height ) ) );
        float x = ( gl_FragCoord.x -  width / 2 ) / ( short_side / 2 );
        float y = ( gl_FragCoord.y - height / 2 ) / ( short_side / 2 );
        
        if( sq( x ) + sq( y ) > 1 )
        {
            color = vec3( 0, 0, 0 );
        }
    }
    gl_FragColor = vec4( color.r, color.g, color.b, 1.0 );
}
