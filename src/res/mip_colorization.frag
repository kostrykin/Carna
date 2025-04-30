#version 330

/*
 *  Copyright (C) 2025 Leonid Kostrykin
 */

uniform sampler2D mip;
uniform sampler1D colorMap;

in vec2 textureCoordinates;

layout( location = 0 ) out vec4 _gl_FragColor;


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

    _gl_FragColor = texture( colorMap, intensity );
}
