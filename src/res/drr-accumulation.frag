#version 330

/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

uniform sampler3D huVolume;
uniform mat4      modelTexture;
uniform float     stepLength;
uniform float     waterAttenuation;
uniform float     lowerThreshold;
uniform float     upperThreshold;
uniform float     upperMultiplier;

in vec4 modelSpaceCoordinates;

layout( location = 0 ) out vec4 _gl_FragColor;


// ----------------------------------------------------------------------------------
// Basic Sampling
// ----------------------------------------------------------------------------------

float intensityAt( vec3 p )
{
    return texture( huVolume, p ).r;
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
    float intensity = intensityAt( textureCoordinates.xyz );
          intensity = intensity + step( upperThreshold, intensity ) * ( upperMultiplier - 1 ) * intensity;
    float huv       = intensity * 4096 - 1024;
    float mu        = waterAttenuation * ( 1 + huv / 1000 );
    float summand   = step( lowerThreshold, intensity ) * mu * stepLength;
    
    _gl_FragColor = vec4( summand, 0, 0, 1 );
}
