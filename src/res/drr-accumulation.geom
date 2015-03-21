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
layout( triangle_strip, max_vertices = 300 ) out; //GL_MAX_GEOMETRY_OUTPUT_VERTICES

out vec4 modelSpaceCoordinates;

uniform mat4 tangentModel;
uniform mat4 modelViewProjection;
uniform vec3 viewDirectionInModelSpace;
uniform int sampleRate;


// ----------------------------------------------------------------------------------
// Emits Triangles for 1 Slice
// ----------------------------------------------------------------------------------

void EmitSliceTriangles( mat4 sliceTangentModel )
{
    for( int i = 0; i < gl_in.length(); ++i )
    {
        vec4 inPosition = gl_in[ i ].gl_Position;
        modelSpaceCoordinates = sliceTangentModel * inPosition;
        vec4 clippingCoordinates = modelViewProjection * modelSpaceCoordinates;
        gl_Position = clippingCoordinates;
        EmitVertex();
    }
    EndPrimitive();
}


// ----------------------------------------------------------------------------------
// Geometry Procedure
// ----------------------------------------------------------------------------------

void main()
{
    for( int sampleIdx = 0; sampleIdx < sampleRate; ++sampleIdx )
    {
        float progress = sampleIdx / float( sampleRate - 1 );
        float offset = sqrt( 3.0 ) * ( 0.5 - progress );

        /* Construct transformation from tangent to model space for specific slice.
         */
        mat4 sliceOffset = mat4( 1 );
        sliceOffset[ 3 ] = vec4( viewDirectionInModelSpace * offset, 1 );
        mat4 sliceTangentModel = transpose( sliceOffset ) * tangentModel;
        
        /* Emit triangles.
         */
        EmitSliceTriangles( sliceTangentModel );
    }
}

