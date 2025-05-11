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
