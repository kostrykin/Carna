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

uniform sampler2D mip;
uniform sampler1D colorMap;
uniform float minIntensity;
uniform float maxIntensity;

in vec2 textureCoordinates;

layout( location = 0 ) out vec4 _gl_FragColor;

#define EPS 1e-16


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    float intensity = texture( mip, textureCoordinates ).r;

    /* Discard fragments which are outside of the volume.
     */
    if( intensity < 0 )
    {
        discard;
    }

    /* Apply intensity clipping.
     */
    intensity = clamp(
        ( intensity - minIntensity + EPS ) / ( maxIntensity - minIntensity + EPS )
        , 0.0, 1.0 );

    /* Query color in `colorMap` and write result.
     */
    _gl_FragColor = texture( colorMap, intensity );
}
