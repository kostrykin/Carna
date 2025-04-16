#version 330

/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

uniform sampler2D labelMap;
uniform vec4      color;
uniform vec2      steps;

in vec2 textureCoordinates;

layout( location = 0 ) out vec4 _gl_FragColor;


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    vec2 tc = textureCoordinates;

    float edgeScore
        = abs( texture( labelMap, vec2( tc.x - steps.x, tc.y ) ).r - texture( labelMap, vec2( tc.x + steps.x, tc.y ) ).r )
        + abs( texture( labelMap, vec2( tc.x, tc.y - steps.y ) ).r - texture( labelMap, vec2( tc.x, tc.y + steps.y ) ).r );

    if( edgeScore > 0 )
    {
        _gl_FragColor = color;
    }
    else
    {
        discard;
    }
}
