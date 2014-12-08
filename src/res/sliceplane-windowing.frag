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

uniform sampler3D intensityTexture;
uniform int windowingLevel;
uniform int windowingWidth;
uniform float border;
uniform int huv0;
uniform int huv1;
uniform float maskOpacity;

void main()
{
	vec3 coord = vec3( gl_TexCoord[ 0 ] );
	
    float i1 = texture3D( intensityTexture, coord ).r;
    float i0 = i1 * 4096 - 1024;
        
    if( coord.x >= 0 && coord.x <= 1
     && coord.y >= 0 && coord.y <= 1
     && coord.z >= 0 && coord.z <= 1 )
    {
        if( coord.x < border || coord.x > 1 - border
         || coord.y < border || coord.y > 1 - border
         || coord.z < border || coord.z > 1 - border )
        {
			gl_FragColor = vec4( 0, 1, 1, 1 );
        }
        else
        {
            float i1 = ( i0 - ( windowingLevel - windowingWidth / 2 ) ) / ( windowingWidth - 1 );
		    
            if( huv0 <= int( i0 ) && int( i0 ) <= huv1 )
            {
                gl_FragColor = vec4( i1 * ( 1 - maskOpacity ) + maskOpacity
                                   , i1 * ( 1 - maskOpacity ) + maskOpacity
                                   , i1 * ( 1 - maskOpacity )
                                   , 1 );
            }
            else
            {
                gl_FragColor = vec4( i1, i1, i1, 1 );
			}
		}
    }
    else
    {
		gl_FragColor = vec4( 0, 0, 0, 0 );
    }
}
