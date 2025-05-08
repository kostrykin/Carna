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
uniform mat4      modelTexture;
uniform float     minIntensity;
uniform float     maxIntensity;
uniform int       invert;

in vec4 modelSpaceCoordinates;

layout( location = 0 ) out vec4 _gl_FragColor;


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
    float f = step( minIntensity, intensity ) * ( 1 - step( maxIntensity, intensity ) ) * ( intensity - minIntensity ) / ( maxIntensity - minIntensity );
          f = f + step( maxIntensity, intensity );
          f = ( 1 - invert ) * f + invert * ( 1 - f );

    _gl_FragColor = vec4( f, f, f, 1 );
}
