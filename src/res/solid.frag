#version 330

/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

uniform vec4 color;
uniform mat4 normalsView;

in vec4 normal;

out vec4 gl_FragColor;


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    vec3 lightDirection = vec3( 0, 0, -1 );
    float diffuseLightAmount = max( 0, -dot( normalize( ( normalsView * normal ).xyz ), lightDirection ) );
    gl_FragColor = vec4( color.rgb * diffuseLightAmount, color.a );
}
