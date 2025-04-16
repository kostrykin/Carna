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

layout( location = 0 ) out vec4 _gl_FragColor;


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    _gl_FragColor = vec4( color );
}
