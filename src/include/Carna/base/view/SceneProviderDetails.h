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

#ifndef SCENEPROVIDER_DETAILS_H_6014714286
#define SCENEPROVIDER_DETAILS_H_6014714286

/** \file   SceneProviderDetails.h
  * \brief  Defines Carna::base::view::SceneProvider::Details.
  */

#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/model/Volume.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// SceneProvider :: Details
// ----------------------------------------------------------------------------------

class SceneProvider :: Details : public QObject
{

    Q_OBJECT

    NON_COPYABLE

    SceneProvider& self;

public:

    explicit Details( SceneProvider& );

    ~Details();


    unsigned int acquireVolumeTexture();

    void releaseVolumeTexture();


    unsigned int acquireMaskTexture();

    void releaseMaskTexture();

    void updateMaskTexture();

    bool isMaskTextureInvalidated() const;


private:

    std::unique_ptr< model::Volume::Texture > volumeTexture;

    unsigned int volumeTextureReferenceCounter;


    unsigned int maskTextureID;

    unsigned int maskTextureReferenceCounter;

    bool maskTextureInvalidated;


private slots:

    void invalidateMaskTexture();

}; // SceneProvider :: Details



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SCENEPROVIDER_DETAILS_H_6014714286
