#version 330

/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

struct VertexData
{
    vec4 modelSpaceCoordinates;
};

in  VertexData vert[];
out VertexData frag;


// ----------------------------------------------------------------------------------
// Geometry Procedure
// ----------------------------------------------------------------------------------

void main()
{
    for( int i = 0; i < gl_in.length(); ++i )
    {
        gl_Position = gl_in[ i ].gl_Position;
        frag = vert[ i ];
        EmitVertex();
    }
    EndPrimitive();
}

