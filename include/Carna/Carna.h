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
#if defined( NO_OVERRIDE_KEYWORD )
#   define override
#endif

#include <vector>



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
        class  SpatialMovement;
        class  Texture3D;
        struct VertexBase;
        struct VertexAttribute;
        class  VertexBufferBase;
        class  Viewport;

        template< typename AssociatedObjectType > class Aggregation;
        template< typename AssociatedObjectType > class Association;
        template< typename VoxelType, typename BufferType = std::vector< VoxelType > > class BufferedHUVolume;
        template< typename BufferedHUVolume > class BufferedHUVolumeTexture;
        template< typename AssociatedObjectType > class Composition;
        template< typename RenderableCompare > class GeometryStage;
        template< typename HUVolumeSegmentVolume > class HUVolumeGrid;
        template< typename HUVolumeGrid, typename HUVolumeSegmentVolume > class HUVolumeSegment;
        template< typename Index > class IndexBuffer;
        template< typename Vertex, typename Index > class Mesh;
        template< typename Vertex > class MeshFactory;
        template< typename RenderableCompare > class MeshRenderingStage;
        template< typename Vector > class Ray;
        template< typename Vector, typename Scalar = typename Vector::Scalar > class RayPlaneHitTest;
        template< typename RenderableCompare > class RenderQueue;
        template< typename Vertex > class VertexBuffer;
        template< typename Type > class ShaderUniform;
        template< typename Concrete > class Singleton;
        template< typename T > class Span;
        
    }

    namespace presets
    {

        class MeshColorCodingStage;
        class RayMarchingStage;
        class MIPStage;
        class MIPChannel;
        class DRRStage;
        class CuttingPlanesStage;
        class TransparentRenderingStage;
        class OccludedRenderingStage;

    }

    namespace helpers
    {

        class DefaultRenderStageOrder;

        template< typename RenderStageOrder = DefaultRenderStageOrder > class FrameRendererHelper;
        template< typename HUVolumeSegmentVolume > class HUVolumeGridHelper;

    }

}  // namespace Carna

#endif // CARNA_GLOBAL_H_6014714286
