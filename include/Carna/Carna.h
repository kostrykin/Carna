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
#include <cstdint>



// ----------------------------------------------------------------------------------
// Forward Declarations
// ----------------------------------------------------------------------------------

namespace Carna
{

    /** \brief
      * Contains the core framework components.
      */
    namespace base
    {

        /** \brief
          * Represents values in \f$\left[-1024, +3071\right]\f$.
          */
        typedef signed short HUV;
    
        class  Application;
        class  AssertionFailure;
        class  BoundingBox;
        class  BoundingVolume;
        class  CameraControl;
        class  CarnaException;
        class  Log;
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
        class  IndexBufferBase;
        class  MeshBase;
        class  ManagedMeshBase;
        class  ManagedMeshInterface;
        class  MeshRenderingStageBase;
        class  Node;
        class  ProjectionControl;
        class  Releasable;
        class  Renderable;
        class  RenderStage;
        class  RenderState;
        class  RenderTask;
        class  Sampler;
        class  Shader;
        class  ShaderCompilationError;
        class  Material;
        class  ShaderManager;
        class  ShaderProgram;
        class  ShaderUniformBase;
        class  Spatial;
        class  SpatialMovement;
        class  ManagedTexture3D;
        class  ManagedTexture3DInterface;
        struct VertexBase;
        struct VertexAttribute;
        class  VertexBufferBase;
        class  Viewport;
        class  NormalMap3D;
        class  NormalMap3DTexture;
        class  TextureBase;

        template< typename AssociatedObjectType > class Aggregation;
        template< typename AssociatedObjectType > class Association;
        template< typename BufferedVectorComponentType, typename BufferType = std::vector< BufferedVectorComponentType > >
            class BufferedNormalMap3D;
        template< typename VoxelType, typename BufferType = std::vector< VoxelType > > class BufferedHUVolume;
        template< typename BufferedVectorFieldType > struct BufferedVectorFieldFormat;
        template< typename BufferedVectorFieldType > class BufferedVectorFieldTexture;
        template< typename AssociatedObjectType > class Composition;
        template< typename RenderableCompare > class GeometryStage;
        template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType > class VolumeGrid;
        template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType > class VolumeSegment;
        template< typename IndexType > class IndexBuffer;
        template< typename VertexType, typename IndexType > class Mesh;
        template< typename VertexType, typename IndexType > class ManagedMesh;
        template< typename VertexType > class MeshFactory;
        template< typename RenderableCompare > class MeshRenderingStage;
        template< typename RenderableCompare > class RenderQueue;
        template< typename VertexType > class VertexBuffer;
        template< typename ValueType > class ShaderUniform;
        template< typename ValueType > struct ShaderUniformType;
        template< typename InstanceType > class Singleton;
        template< unsigned int dimension > class Texture;

        /** \brief
          * Defines 16bit unsigned integer \ref HUV "HU" volume data.
          */
        typedef BufferedHUVolume< uint16_t > HUVolumeUInt16;

        /** \brief
          * Defines 8bit signed integer \ref BufferedNormalMap3D.
          */
        typedef BufferedNormalMap3D< int8_t > NormalMap3DInt8;
        
        /** \brief
          * Provides set of math-related classes and functions.
          */
        namespace math
        {

            class Ray3f;

            template< typename VectorType > class Ray;
            template< typename ValueType > class VectorField;
            template< typename T > class Span;
            template< typename VectorType, typename ScalarType = typename VectorType::Scalar > class RayPlaneHitTest;

        }
        
    }

    /** \brief
      * Contains ready-to-use components that are assembled only using the
      * \ref base "core" framework components.
      */
    namespace presets
    {

        class CameraNavigationControl;
        class CameraShowcaseControl;
        class PerspectiveControl;
        class MeshColorCodingStage;
        class VolumeRenderingStage;
        class MIPStage;
        class MIPChannel;
        class DRRStage;
        class DVRStage;
        class CuttingPlanesStage;
        class TransparentRenderingStage;
        class OccludedRenderingStage;
        class CompositionStage;

    }

    /** \brief
      * Contains components that simplify the handling of more complex interfaces.
      */
    namespace helpers
    {

        class DefaultRenderStageOrder;

        template< typename RenderStageOrder = DefaultRenderStageOrder > class FrameRendererHelper;
        template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType = void > class VolumeGridHelper;

    }

}  // namespace Carna

#endif // CARNA_GLOBAL_H_6014714286
