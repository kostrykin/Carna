#version 330

/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

uniform mat4 tangentModel;
uniform mat4 modelViewProjection;

layout( location = 0 ) in vec4 inPosition;

out vec4 modelSpaceCoordinates;


// ----------------------------------------------------------------------------------
// Vertex Procedure
// ----------------------------------------------------------------------------------

void main()
{
    modelSpaceCoordinates = tangentModel * inPosition;
    vec4 clippingCoordinates = modelViewProjection * modelSpaceCoordinates;
    gl_Position = clippingCoordinates;
}
