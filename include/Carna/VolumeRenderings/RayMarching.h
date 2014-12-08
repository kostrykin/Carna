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

#ifndef RAYMARCHING_H_6014714286
#define RAYMARCHING_H_6014714286

/** \file   RayMarching.h
  * \brief  Defines \ref Carna::VolumeRenderings::RayMarching.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: RayMarching
// ----------------------------------------------------------------------------------

/** \brief  Defines the \ref VolumeRendererRayMarching "ray marching" parameters.
  *
  * \author Leonid Kostrykin
  * \date   8.8.11 - 19.6.13
  */
class CARNA_EXPORT RayMarching
{

    NON_COPYABLE

public:

    /** \brief
      * Instantiates.
      *
      * \param
      * renderer references the performing renderer.
      *
      * \param
      * samples specifies the maximum number of samples to make per pixel.
      *
      * \param
      * volume_texture_sampler is the texture sampler to which the volume texture is bound.
      *
      * \param
      * mask_texture_sampler is the texture sampler to which the mask texture is bound.
      *
      * \param
      * front_faces_texture_sampler is the sampler that holds the \ref VolumeRendererRayMarching "front faces' volume coordinates".
      *
      * \param
      * back_faces_texture_sampler is the sampler that holds the \ref VolumeRendererRayMarching "back faces' volume coordinates".
      */
    RayMarching
        ( VolumeRenderer& renderer
        , unsigned int samples
        , unsigned int volume_texture_sampler
        , unsigned int mask_texture_sampler
        , unsigned int front_faces_texture_sampler
        , unsigned int back_faces_texture_sampler )

        : renderer( renderer )
        , samples( samples )
        , volumeTextureSampler( volume_texture_sampler )
        , maskTextureSampler( mask_texture_sampler )
        , frontFacesTextureSampler( front_faces_texture_sampler )
        , backFacesTextureSampler( back_faces_texture_sampler )
    {
    }

    /** \brief  Holds the number of samples to make per pixel.
      *
      * This value may override the samples count configured by the \ref renderer.
      */
    const unsigned int samples;

    /** \brief  Holds the ID of the sampler, to which the volume texture is bound.
      */
    const unsigned int volumeTextureSampler;

    /** \brief  Holds the ID of the sampler, to which the mask texture is bound.
      */
    const unsigned int maskTextureSampler;

    /** \brief  Holds the ID of the sampler, to which the front faces texture is bound.
      */
    const unsigned int frontFacesTextureSampler;

    /** \brief  Holds the ID of the sampler, to which the back faces texture is bound.
      */
    const unsigned int backFacesTextureSampler;

    /** \brief  References the \ref VolumeRenderer, this rendering pass belongs to.
      */
    VolumeRenderer& renderer;

}; // VolumeRenderings :: RayMarching



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // RAYMARCHING_H_6014714286
