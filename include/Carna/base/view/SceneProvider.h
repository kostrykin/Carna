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

#ifndef SCENEPROVIDER_H_6014714286
#define SCENEPROVIDER_H_6014714286

/** \file   SceneProvider.h
  * \brief  Defines Carna::base::view::SceneProvider.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QObject>

class QGLContext;

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// SceneProvider
// ----------------------------------------------------------------------------------

/** \brief
  * Interfaces the \ref model::Scene "data model" from a view's perspective and
  * provides graphical representations of model elements.
  *
  * \see
  * Objects of the \ref ResourceClient class are used to access the graphical
  * resources offered by a scene provider.
  *
  * Any single instance of this class can be used by any number of renderers.
  * 
  * The volume data is represented as a 3D texture. Since it is available to every
  * renderer, a single instance of this class \em should be shared among as much
  * renderers as possible in order to enable an effective memory consumption.
  *
  * Use the \ref ResourceClient class to define a context that requires access to
  * graphical resources. For instance, a renderer's lifetime is such a context.
  * Refer to the \ref ResourceClient documentation for further examples.
  *
  * \author Leonid Kostrykin
  * \date   25.11.11 - 15.3.13
  */
class CARNA_EXPORT SceneProvider : public QObject
{

    Q_OBJECT

    NON_COPYABLE

public:

    // ------------------------------------------------------------------------------
    // General
    // ------------------------------------------------------------------------------

    /** \brief  Instantiates.
      */
    explicit SceneProvider( const model::Scene&, QObject* parent = nullptr );

    /** \brief  Does nothing.
      */
    virtual ~SceneProvider();

    /** \brief  References the data model.
      */
    const model::Scene& scene;

    /** \brief 
      * Returns the maximum number of voxels supported along any axis of a 3D texture.
      *
      * Queries the OpenGL implementation for \c GL_MAX_3D_TEXTURE_SIZE when called the first time.
      * No 3D texture's dimension is allowed to be greater than the returned value.
      *
      * \note
      * This method was part of \ref model::Scene class prior to \ref v_2_4.
      */
    static unsigned int max3DTextureSize();

    // ------------------------------------------------------------------------------
    // Resource Management
    // ------------------------------------------------------------------------------

    /** \brief 
      * Graphical resources are ensured to stay valid at least for the duration of a
      * context lifetime. This class is used to access the graphical resources, that
      * a \ref SceneProvider "scene provider" offers.
      *
      * Lets say we have a class \c Consumer that may require quick access to the
      * graphical resources at any random moment of it's whole lifetime. With respect
      * to the "is"-semantics of inheritance (as in ''Consumer \em is a resource
      * context'') in object oriented programming, consider the below code as an
      * elegant solution:
      *
      * \code
      * using namespace Carna;
      * using namespace Carna::base;
      * using namespace Carna::base::view;
      *
      * class Client : public SceneProvider::ResourceClient
      * {
      * public:
      *     Client( VisualizationEnvironment&, view::SceneProvider& );
      * }
      *
      * Client::Client( VisualizationEnvironment& environment, SceneProvider& provider )
      *     : SceneProvider::ResourceClient( environment, provider )
      * {
      * }
      * \endcode
      *
      * However, the next code is just as valid:
      *
      * \code
      * using namespace Carna;
      * using namespace Carna::base;
      * using namespace Carna::base::view;
      *
      * class Client
      * {
      *     std::unique_ptr< SceneProvider::ResourceClient > context;
      * public:
      *     Client( VisualizationEnvironment&, SceneProvider& );
      * }
      *
      * Client::Client( VisualizationEnvironment& environment, SceneProvider& provider )
      *     : context( new SceneProvider::ResourceClient( environment, provider ) )
      * {
      * }
      * \endcode
      *
      * \invariant
      * <code>maskTextureID != volumeTextureID</code>
      *
      * \author Leonid Kostrykin
      * \date   25.11.11 - 15.3.13
      */
    class CARNA_EXPORT ResourceClient
    {

        NON_COPYABLE
        
    public:

        /** \brief  References the provider, this resource context belongs to.
          *
          * \since  \ref v_2_2_2
          */
        SceneProvider& provider;
        
        /** \brief  References the visualization environment, this resource context belongs to.
          *
          * \since  \ref v_2_2_2
          */
        VisualizationEnvironment& environment;

        /** \brief
          * Holds the ID of the OpenGL texture object that represents the volume data
          * of the interfaced data model.
          *
          * \invariant
          * <code>volumeTextureID > 0</code>
          */
        const unsigned int volumeTextureID;

        /** \brief
          * Holds the ID of the OpenGL texture object that represents the mask data
          * of the interfaced data model.
          *
          * \invariant
          * <code>maskTextureID > 0</code>
          *
          * \since  \ref v_2_2_2
          */
        const unsigned int maskTextureID;

        /** \brief  Acquires graphical resources.
          *
          * Activates the GL context provided by \a environment.
          */
        ResourceClient( VisualizationEnvironment& environment, SceneProvider& provider );

        /** \brief  Releases the graphical resources.
          */
        virtual ~ResourceClient();

        /** \brief  Synchronizes the data provided by the associated \ref SceneProvider object with the interfaced \ref model::Scene "data model".
          *
          * Usually there shall be now reason to call this method manually.
          * It is invoked by \ref Renderer::paint when necessary.
          *
          * \since  \ref v_2_2_2
          */
        void updateContext();

    }; // ResourceClient


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // Scene



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SCENEPROVIDER_H_6014714286
