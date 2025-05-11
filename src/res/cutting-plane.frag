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

uniform sampler3D intensities;
uniform sampler1D colorMap;
uniform float     minIntensity;
uniform float     maxIntensity;
uniform mat4      modelTexture;

in vec4 modelSpaceCoordinates;

layout( location = 0 ) out vec4 _gl_FragColor;

#define EPS 1e-16


// ----------------------------------------------------------------------------------
// Basic Sampling
// ----------------------------------------------------------------------------------

float intensityAt( vec3 p )
{
    return texture( intensities, p ).r;
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

    /* Apply intensity clipping.
     */
    intensity = clamp(
        ( intensity - minIntensity + EPS ) / ( maxIntensity - minIntensity + EPS )
        , 0.0, 1.0 );

    /* Query color in `colorMap` and write result.
     */
    _gl_FragColor = vec4( texture( colorMap, intensity ).rgb, 1 );
}
