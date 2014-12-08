#version 120

/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

varying vec3 volumespace_position;

void main()
{
    volumespace_position = vec3( gl_Vertex );
    gl_Position = ftransform();
}
