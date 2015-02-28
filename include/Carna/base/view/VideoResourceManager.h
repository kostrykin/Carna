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

#ifndef VIDEORESOURCEMANAGER_H_6014714286
#define VIDEORESOURCEMANAGER_H_6014714286

/** \file   VideoResourceManager.h
  * \brief  Defines \ref Carna::base::view::VideoResourceManager.
  */

#include <Carna/base/noncopyable.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// VideoResourceManager
// ----------------------------------------------------------------------------------
    
/** \brief
  * Controls the lifetime of a particular resource that resides in video memory.
  */
template< typename VideoResourceType, typename ManagerInstanceType, typename Source, typename SourceRef = const Source& >
class VideoResourceManager : public GeometryFeature
{

    std::unique_ptr< VideoResourceType > myResource;

protected:

    const Source source;

    VideoResourceManager( SourceRef );

    virtual void acquireVideoResource() override;

    virtual void releaseVideoResource() override;

    virtual VideoResourceType* createResource() = 0;

public:

    typedef VideoResourceManager
        < typename VideoResourceType
        , typename ManagerInstanceType
        , typename Source
        , typename SourceRef >
            VideoResourceManagerBase;

    typedef typename VideoResourceType VideoResource;

    const VideoResourceType& resource() const;

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    static ManagerInstanceType& create( SourceRef );

}; // VideoResourceManager


template< typename VideoResourceType, typename ManagerInstanceType, typename Source, typename SourceRef >
VideoResourceManager< VideoResourceType, ManagerInstanceType, Source, SourceRef >::VideoResourceManager( SourceRef source )
    : source( source )
{
}


template< typename VideoResourceType, typename ManagerInstanceType, typename Source, typename SourceRef >
ManagerInstanceType& VideoResourceManager< VideoResourceType, ManagerInstanceType, Source, SourceRef >::create( SourceRef src )
{
    return *new ManagerInstanceType( src );
}


template< typename VideoResourceType, typename ManagerInstanceType, typename Source, typename SourceRef >
void VideoResourceManager< VideoResourceType, ManagerInstanceType, Source, SourceRef >::acquireVideoResource()
{
    GeometryFeature::acquireVideoResource();
    if( videoResourceAcquisitionsCount() == 1 )
    {
        CARNA_ASSERT( myResource.get() == nullptr );
        myResource.reset( createResource() );
    }
}


template< typename VideoResourceType, typename ManagerInstanceType, typename Source, typename SourceRef >
void VideoResourceManager< VideoResourceType, ManagerInstanceType, Source, SourceRef >::releaseVideoResource()
{
    if( videoResourceAcquisitionsCount() == 1 )
    {
        myResource.reset();
    }
    GeometryFeature::releaseVideoResource();
}


template< typename VideoResourceType, typename ManagerInstanceType, typename Source, typename SourceRef >
const VideoResourceType& VideoResourceManager< VideoResourceType, ManagerInstanceType, Source, SourceRef >::resource() const
{
    CARNA_ASSERT( myResource.get() != nullptr );
    return *myResource;
}


template< typename VideoResourceType, typename ManagerInstanceType, typename Source, typename SourceRef >
bool VideoResourceManager< VideoResourceType, ManagerInstanceType, Source, SourceRef >
    ::controlsSameVideoResource( const GeometryFeature& ) const
{
    return false;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // VIDEORESOURCEMANAGER_H_6014714286
