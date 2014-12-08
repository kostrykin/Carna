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

void main()
{
    float z = 1.0 - ( gl_FragCoord.z / gl_FragCoord.w );
    gl_FragColor = vec4( z, z, z, 1 );
}
