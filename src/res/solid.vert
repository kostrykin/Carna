#version 330

/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

uniform mat4 modelViewProjection;

out vec4 normal;

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec4 inNormal;


// ----------------------------------------------------------------------------------
// Vertex Procedure
// ----------------------------------------------------------------------------------

void main()
{
    vec4 clippingCoordinates = modelViewProjection * inPosition;
    gl_Position = clippingCoordinates;
    normal = inNormal;
}
