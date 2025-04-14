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

in vec4 fragColor;

layout( location = 0 ) out vec4 _gl_FragColor;


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    _gl_FragColor = fragColor;
}
