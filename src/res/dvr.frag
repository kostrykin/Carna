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

uniform sampler3D huVolume;
uniform sampler1D colorMap;
uniform mat4      modelTexture;
uniform float     stepLength;
uniform float     translucence;

in vec4 modelSpaceCoordinates;

out vec4 gl_FragColor;


// ----------------------------------------------------------------------------------
// Basic Sampling
// ----------------------------------------------------------------------------------

vec4 sampleAt( vec3 p )
{
    float intensity = texture( huVolume, p ).r;
    return texture( colorMap, intensity );
}


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    if( abs( modelSpaceCoordinates.x ) > 0.5 || abs( modelSpaceCoordinates.y ) > 0.5 || abs( modelSpaceCoordinates.z ) > 0.5 )
    {
        discard;
    }
    
    vec4 textureCoordinates = modelTexture * modelSpaceCoordinates;
    vec4 color = sampleAt( textureCoordinates.xyz );
    
    float alpha = color.a * stepLength / ( 1 + translucence );
    gl_FragColor = vec4( color.rgb * alpha, alpha );
}
