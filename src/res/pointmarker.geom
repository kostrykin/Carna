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

uniform vec4 color;
uniform float pointSize;

layout( points ) in;
layout( points, max_vertices = 2 ) out;

out vec4 fragColor;


// ----------------------------------------------------------------------------------
// Geometry Procedure
// ----------------------------------------------------------------------------------

void main()
{
	gl_Position = gl_in[ 0 ].gl_Position;
	gl_PointSize = pointSize + 2;
	fragColor = vec4( 1, 1, 1, color.a );
	EmitVertex();
    EndPrimitive();
	
	gl_Position = gl_in[ 0 ].gl_Position;
	gl_PointSize = pointSize;
	fragColor = color;
	EmitVertex();
    EndPrimitive();
}
