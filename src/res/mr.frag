#version 330

/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

uniform sampler3D mask;
uniform mat4      modelTexture;
uniform bool      ignoreColor;
uniform vec4      color;

in vec4 modelSpaceCoordinates;

out vec4 gl_FragColor;


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
    float intensity = texture( mask, textureCoordinates.xyz ).r;
    if( intensity > 0 )
    {
        if( ignoreColor )
        {
            gl_FragColor = vec4( intensity, 0, 0, 1.0 );
        }
        else
        {
            gl_FragColor = vec4( color );
        }
    }
    else
    {
        discard;
    }
}
