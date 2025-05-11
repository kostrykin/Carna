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

#ifndef LIBCARNA_GLOBAL_H_6014714286
#define LIBCARNA_GLOBAL_H_6014714286

/** \file
  * \brief
  * Contains forward-declarations.
  */

#if defined( LIBCARNA_EXPORT )
#   if defined( _MSC_VER )
#       define LIBCARNA __declspec( dllexport )
#       pragma warning( disable : 4251 )
#   elif defined( _GCC )
#       define LIBCARNA __attribute__( ( visibility( "default" ) ) )
#   else
#       define LIBCARNA
#   endif
#else
#   if defined( _MSC_VER )
#       define LIBCARNA __declspec( dllimport )
#   else
#       define LIBCARNA
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

namespace LibCarna
{

    /** \brief
      * Contains the core framework components.
      */
    namespace base
    {
    
        class  Application;
        class  AssertionFailure;
        class  BaseBuffer;
        class  BlendFunction;
        class  BoundingBox;
        class  BoundingVolume;
        class  Camera;
        class  CameraControl;
        class  Color;
        struct ColoredVertex;
        class  ColorMap;
        class  Framebuffer;
        class  FrameRenderer;
        class  Geometry;
        class  GeometryFeature;
        class  GLContext;
        struct HUV;
        struct HUVOffset;
        class  IndexBufferBase;
        class  LibCarnaException;
        class  Log;
        class  ManagedMeshBase;
        class  ManagedMeshInterface;
        class  ManagedTexture3D;
        class  ManagedTexture3DInterface;
        class  Material;
        class  MeshBase;
        class  MeshRenderingMixin;
        class  Node;
        class  NodeListener;
        class  NormalMap3D;
        class  NormalMap3DTexture;
        struct PCVertex;
        struct PNVertex;
        class  ProjectionControl;
        struct PVertex;
        class  Releasable;
        class  Renderable;
        class  RenderStage;
        class  RenderStageListener;
        class  RenderStageSequence;
        class  RenderState;
        class  RenderTask;
        class  RotatingColor;
        class  Sampler;
        class  Shader;
        class  ShaderCompilationError;
        class  ShaderManager;
        class  ShaderProgram;
        class  ShaderUniformBase;
        class  Spatial;
        class  SpatialMovement;
        class  TextureBase;
        struct VertexAttribute;
        class  VertexBufferBase;
        struct VertexColor;
        struct VertexNormal;
        struct VertexPosition;
        class  Viewport;

        template< typename AssociatedObjectType > class Aggregation;
        template< typename AssociatedObjectType > class Association;
        template< typename BufferedVectorComponentType, typename BufferType = std::vector< BufferedVectorComponentType > >
            class BufferedNormalMap3D;
        template< typename VoxelType, typename BufferType = std::vector< VoxelType > > class BufferedIntensityVolume;
        template< typename BufferedVectorFieldType > struct BufferedVectorFieldFormat;
        template< typename BufferedVectorFieldType > class BufferedVectorFieldTexture;
        template< typename AssociatedObjectType > class Composition;
        template< typename RenderableCompare > class GeometryStage;
        template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType > class VolumeGrid;
        template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType > class VolumeSegment;
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
          * Defines 16bit intensity volume.
          */
        typedef BufferedIntensityVolume< uint16_t > IntensityVolumeUInt16;

        /** \brief
          * Defines 8bit intensity volume.
          */
        typedef BufferedIntensityVolume< uint8_t > IntensityVolumeUInt8;

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
        class OrthogonalControl;
        class MeshColorCodingStage;
        class VolumeRenderingStage;
        class MIPStage;
        class DRRStage;
        class DVRStage;
        class MaskRenderingStage;
        class CuttingPlanesStage;
        class TransparentRenderingStage;
        class OccludedRenderingStage;
        class CompositionStage;

    }

    /** \brief
      * Contains components that simplify the handling of more complex interfaces
      * from both, \ref base and \ref presets.
      */
    namespace helpers
    {

        class PointMarkerHelper;
        class DefaultRenderStageOrder;
        class VolumeGridHelperBase;

        template< typename RenderStageOrder = DefaultRenderStageOrder > class FrameRendererHelper;
        template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType = void > class VolumeGridHelper;
        
        /** \brief
          * Holds implementation details.
          */
        namespace details
        {
        }

    }

}  // namespace LibCarna

#endif // LIBCARNA_GLOBAL_H_6014714286
