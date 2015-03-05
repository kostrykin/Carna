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

#ifndef CARNA_GLOBAL_H_6014714286
#define CARNA_GLOBAL_H_6014714286

#if defined( CARNA_EXPORT )
#   if defined( _MSC_VER )
#       define CARNA_LIB __declspec( dllexport )
#       pragma warning( disable : 4251 )
#   elif defined( _GCC )
#       define CARNA_LIB __attribute__( ( visibility( "default" ) ) )
#   else
#       define CARNA_LIB
#   endif
#else
#   if defined( _MSC_VER )
#       define CARNA_LIB __declspec( dllimport )
#   else
#       define CARNA_LIB
#   endif
#endif



// ----------------------------------------------------------------------------------
// Forward Declarations
// ----------------------------------------------------------------------------------

namespace Carna
{

    /** \brief  Contains the core framework components.
      */
    namespace base
    {

        typedef signed short HUV;
    
        class  Application;
        class  AssertionFailure;
        class  BoundingBox;
        class  BoundingVolume;
        class  CarnaException;
        class  Log;
        class  Text;
        class  HUVolume;
        class  BaseBuffer;
        class  BlendFunction;
        class  Camera;
        class  Color;
        class  Framebuffer;
        class  FrameRenderer;
        class  Geometry;
        class  GeometryFeature;
        class  GLContext;
        class  HUVolumeTexture;
        class  IndexBufferBase;
        class  MeshBase;
        class  MeshColorCodingStage;
        class  MeshRenderingStageBase;
        class  Node;
        class  Ray3f;
        class  Renderable;
        class  RenderStage;
        class  RenderState;
        class  RenderTask;
        class  RenderTexture;
        class  Sampler;
        class  Shader;
        class  ShaderCompilationError;
        class  Material;
        class  ShaderManager;
        class  ShaderProgram;
        class  ShaderUniformBase;
        class  Spatial;
        class  Texture3D;
        struct VertexBase;
        struct VertexAttribute;
        class  VertexBufferBase;
        class  Viewport;
        
    }

    namespace VolumeRenderings
    {
        class RayMarchingStage;

        namespace MIP
        {
            class MIPStage;
            class Channel;
        }

        namespace DRR
        {
            class DRRStage;
        }
    }

    namespace CuttingPlanes
    {
        class CuttingPlanesStage;
    }

}  // namespace Carna

#endif // CARNA_GLOBAL_H_6014714286
