#version 330

/*
 *  Copyright (C) 2025 Leonid Kostrykin
 */

layout( location = 0 ) in vec4 inPosition;

out vec2 textureCoordinates;


// ----------------------------------------------------------------------------------
// Vertex Procedure
// ----------------------------------------------------------------------------------

void main()
{
    textureCoordinates = ( inPosition.xy + 1 ) / 2;
    gl_Position = inPosition;
}
