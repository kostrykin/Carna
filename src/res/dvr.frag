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

uniform sampler3D intensities;
uniform sampler3D normalMap;
uniform sampler1D colorMap;
uniform mat4      modelTexture;
uniform mat3      normalsView;
uniform float     stepLength;
uniform float     translucence;
uniform float     diffuseLight;
uniform int       lightingEnabled;

in vec4 modelSpaceCoordinates;

layout( location = 0 ) out vec4 _gl_FragColor;


// ----------------------------------------------------------------------------------
// Basic Sampling
// ----------------------------------------------------------------------------------

vec4 sampleAt( vec3 p )
{
    float intensity = texture( intensities, p ).r;
    vec4 color = texture( colorMap, intensity );

    if( lightingEnabled == 1 )
    {
        vec3 normalDirection = texture( normalMap, p ).rgb;
        vec3 diffuseColor;
        if( dot( normalDirection, normalDirection ) < 1e-4 )
        {
            diffuseColor = vec3( 0, 0, 0 );
        }
        else
        {
            vec3 normal = normalize( normalsView * normalDirection );
            vec3 lightDirection = vec3( 0, 0, -1 );
            float diffuseLightAmount = max( 0, -dot( normal, lightDirection ) );
            diffuseColor = color.rgb * diffuseLightAmount;
        }
        return vec4( mix( color.rgb, diffuseColor, diffuseLight ), color.a );
    }
    else
    {
        return color;
    }
}


// ----------------------------------------------------------------------------------
// Fragment Procedure
// ----------------------------------------------------------------------------------

void main()
{
    if( abs( modelSpaceCoordinates.x ) > 0.5 || abs( modelSpaceCoordinates.y ) > 0.5 || abs( modelSpaceCoordinates.z ) > 0.5 )
    {
        discard;
    }
    
    vec4 textureCoordinates = modelTexture * modelSpaceCoordinates;
    vec4 color = sampleAt( textureCoordinates.xyz );
    
    float alpha = color.a * stepLength / ( 1 + translucence );
    _gl_FragColor = vec4( color.rgb * alpha, alpha );
}
