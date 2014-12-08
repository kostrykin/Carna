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

#ifndef DVR_DIRECTVOLUMERENDERING_H_6014714286
#define DVR_DIRECTVOLUMERENDERING_H_6014714286

/** \file   DirectVolumeRendering.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::DirectVolumeRendering
  */

#include <Carna/VolumeRenderings/VolumeRendererSinglePassMode.h>
#include <QTimer>

class QCheckBox;
class QDoubleSpinBox;

namespace Carna
{

namespace VolumeRenderings
{

/** \brief  Provides classes related to \ref VolumeRenderer "volume renderer"
  *         mode which simulates light absorption on the basis of a color map.
  */
namespace DVR
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: DVR :: DirectVolumeRendering
// ----------------------------------------------------------------------------------

/** \brief  VolumeRendererMode implementation which simulates light absorption on the
  *         basis of a color map.
  *
  * \image html AbsorptionProjection01.png
  *
  * \section DVR_Algorithm Algorithm
  *
  * For each pixel the \ref VolumeRendererRayMarching "ray marching algorithm" samples
  * voxels along a casted ray in equidistant steps. The %DVR computes the sum of the
  * \f$ w_i \f$ weighted colors \f$ \vec f_{\mathrm{rgb}} \f$, that are obtained by
  * mapping voxel intensities at some position \f$ \vec x_i \f$ along the ray:
  *
  * \f[
  * \vec v_\text{pixel color} =
  * \sum\limits_{i=1}^s w_i \cdot \vec f_{\mathrm{rgb}}( \vec x_i )
  * \quad \text{with} \quad \text{sample rate } s := \frac{\text{samples}}{\text{pixel}}
  * \f]
  *
  * The weights \f$ w_i \f$ consist of two factors. The first factor, the <em>scaled alpha</em>,
  * controls the contribution of a voxel intensity to the final pixel color, based on the alpha value
  * that is mapped to a voxel intensity:
  *
  * \f[
  * a_i = 1 - \Big( 1 - f_{\alpha}\left( \vec x_i \right) \Big)^{ \frac{ s_0 }{ s } }
  * \f]
  *
  * In this equation, \f$ s_0 \f$ is the <em>reference sampling rate</em>. It is required to produce
  * similar results across different sampling rates \f$ s \f$. This %DVR implementation uses \f$ s_0 = 256 \f$,
  * for it has proven to produce satisfying results.
  *
  * \see
  * The original equation can be found in
  * <a href="http://http.developer.nvidia.com/GPUGems/gpugems_ch39.html#ch39equ03">
  * ''NVIDIA GPU Gems 3'', chapter 39.4.3, ''Rendering''</a>. It is denoted as Equation 3,
  * ''Formula for Opacity Correction''.
  *
  * The second factor controls the contribution of the \f$ i \f$th sample based on the \em opacity
  * of the voxels that were sampled before:
  *
  * \f[
  * o_{i} =
  * \begin{cases}
  * 1 & \text{ if } i = 1 \\
  * o_{i-1} \cdot \left( 1 - a_i \right) & \text{ else}
  * \end{cases}
  * \f]
  *
  * For each ray the color weights \f$ w_i = a_i \cdot o_i \f$ are a monotonously decreasing
  * sequence. The higher the alpha value \f$ f_{\mathrm\alpha}( \vec x_i ) \f$ is, the greater
  * the difference \f$ w_i - w_{i+1} \f$ becomes.
  *
  * \section DVR_Usage Usage
  *
  * The code below shows how the class is supposed to be used with a \ref VolumeVisualization module.
  * The %DVR is installed on the given \c %VolumeVisualization and activated immediately:
  *
  * \code
  * static void doDVR
  *         ( Carna::VolumeRenderings::VolumeVisualization& module
  *         , Carna::base::model::Scene& scene )
  * {
  *     module.doAfterInitialization( [&]()
  *     {
  *         auto const dvr = new Carna::VolumeRenderings::DVR::DirectVolumeRendering( scene );
  *         module.renderer().installMode( dvr );
  *         module.renderer().setMode( Carna::VolumeRenderings::DVR::DirectVolumeRendering::NAME );
  *     }
  * }
  * \endcode
  *
  * When you use this code snippet, you might end up with an empty rendering result. This
  * will be the case when you use the code above for rendering \em once, e.g. to an
  * \ref base::OffscreenDisplay "OffscreenDisplay". The next section explains why this
  * might happen and how to handle this case properly.
  *
  * \subsection DVR_ColorMap Color map
  *
  * The color map is an one-to-one relation \f$ \vec \varphi : D_{\vec\varphi} \to \text{Color} \f$
  * with \f$ D_{\vec\varphi} \subseteq [-1024; 3071] \f$. The function \f$ \vec f \f$, that
  * was used in the section above to denote the color mapped to a voxel by it's intensity,
  * therefore is simply:
  *
  * \f[
  * \vec f\left( \vec x \right) = \vec\varphi\left( \mathrm{intensity}\left( \vec x \right) \right)
  * \f]
  * 
  * When \c %DirectVolumeRendering is instantiated, it computes an initial color map
  * automatically. This is done by analyzing the histogram of the loaded data set.
  *
  * \par Color lookup texture
  *
  * The \ref colorMap "color map" consists of distinct \ref ColorMap::HUSpan "HU ranges",
  * that are mapped to arbitrary \ref ColorMap::ColorSpan "color ranges". Both range types
  * are specified through their boundary values. Linear interpolation is used to determine
  * the values in between. Since performing linear interpolation for each color lookup is
  * an expansive and redundant operation, a single <em>color lookup texture</em>
  * \f$ \vec\tau : [-1024; 3071] \to \text{Color} \f$ is built before the rendering. This
  * color lookup texture \f$ \vec\tau \f$ also is a one-to-one relation:
  *
  * \f[
  * \vec\tau\left( h \right) =
  * \begin{cases}
  * \vec\varphi\left( h \right) & \forall h \in D_{\vec\varphi} \\
  * \left( 0, 0, 0, 0 \right)^{\mathrm T} & \forall h \not\in D_{\vec\varphi}
  * \end{cases}
  * \f]
  *
  * Hence, HU values, that are not mapped by the color map explicitly, are now mapped by
  * \f$ \vec\tau \f$ to zero-alpha colors.
  *
  * \par Asynchronous updating
  *
  * \attention
  * The color lookup texture isn't rebuilt every time the \ref ColorMap::changed "color map changes".
  * Instead, it is just guaranteed that it is rebuilt <em>within a short time period</em>.
  * This is done to avoid unnecessary computations when the color map changes frequently,
  * e.g. because of user interaction.
  *
  * Since the %DVR \ref base::view::Renderer::invalidate "invalidates the renderer" as
  * soon as a new color lookup texture has been built, the user shouldn't notice any
  * delay.
  *
  * However, when you try to tap the result programmatically as soon as rendering is
  * finished, there is high probability for you to encounter unexpected results. This
  * is especially the case when rendering is invoked right after the
  * \c %DirectVolumeRendering class was instantiated. In this case, and whenever you need
  * the color lookup texture to be rebuilt immediately, call \ref commitColorMap before
  * invoking the rendering:
  *
  * \code
  * static void immediatelyDoDVR
  *         ( Carna::VolumeRenderings::VolumeVisualization& module
  *         , Carna::base::model::Scene& scene )
  * {
  *     doDVR( module, scene );
  *     module.doAfterInitialization( [&]()
  *     {
  *         typedef Carna::VolumeRenderings::DVR::DirectVolumeRendering DVR;
  *         const DVR& dvr = static_cast< DVR >( module.renderer().mode() );
  *         dvr.commitColorMap();
  *     }
  * }
  * \endcode
  *
  * \section DVR_Implementation Implementation
  *
  * As described \ref VolumeRendererRayMarching "here", the ray marching algorithm
  * computes a ray in volume space for each screen space pixel. This computation is
  * implemented in the fragment shader. In order to make it execute for each screen
  * pixel, a rectangle needs to be drawn over the whole screen.
  *
  * \subsection DVR_VertexShader Vertex shader
  *
  * The corresponding vertex shader transforms this rectangle's vertices through the
  * fixed functionality pipeline. The \ref base::view::Renderer::paintRectangle
  * method, that is used to draw the rectangle, denotes the vertices with
  * corresponding texture coordinates. Those coordinates are passed to the fragment
  * shader.
  *
  * \code
  * #version 120
  * 
  * void main()
  * {
  *     gl_Position = ftransform();
  *     gl_TexCoord[ 0 ] = gl_MultiTexCoord0;
  * }
  * \endcode
  *
  * \subsection DVR_FragmentShader Fragment shader
  *
  * The fragment shader is the part where the fun happens. First, we have the
  * declarations of all the uniforms \em every volume rendering mode shader must
  * accept. These variables are determined by the \ref RayMarching "ray marching configuration object",
  * that is passed in to \ref VolumeRendererSinglePassMode::renderColor from the
  * renderer. The base class \ref VolumeRendererSinglePassMode uploads these
  * variables to the shader.
  *
  * \code
  * #version 120
  * 
  * uniform sampler3D intensityTexture;
  * uniform sampler1D colorMap;
  * uniform sampler2D frontFacesTexture;
  * uniform sampler2D backFacesTexture;
  *
  * uniform int sample_count;
  * \endcode
  *
  * These uniform variables are completed by those that are specific to this
  * \c %VolumeRendererMode implementation. They are set from within the
  * \ref configureShader implementation of the base class.
  *
  * \code
  * uniform float specular_amplitude;
  * uniform float specular_bias;
  * uniform float specular_back;
  * uniform vec3 gradient_sample_distance;
  * uniform float pickingSaturation;
  * \endcode
  *
  * We will discuss the purpose of these variables later when they come into use.
  *
  * \par Helper functions
  *
  * The fragment shader defines the following helper functions, that are
  * mostly self-explanatory.
  *
  * \code
  * int min( int x, int y )
  * {
  *     return x < y ? x : y;
  * }
  *
  * float getIntensityAt( in vec3 a )
  * {
  *     return texture3D( intensityTexture, a ).r;
  * }
  *
  * vec4 getSampleAt( in vec3 a )
  * {
  *     float i = getIntensityAt( a );
  *     return texture1D( colorMap, i );
  * }
  * \endcode
  *
  * The latter two functions implement the entities that were explained in the
  * \ref DVR_Algorithm "algorithm section":
  *
  * -   \c getIntensityAt implements \f$ \mathrm{intensity}\left( \vec x \right) \f$.
  * -   \c getSampleAt implements the color mapping as defined by \f$ \vec f\left( \vec x \right) \f$
  *     by sampling the color lookup texture:
  *
  * \image html AbsorptionProjection02.png
  *
  * The next two functions are required for the specular lighting implementation.
  * \c getGradient computes and returns the gradient of the 3D volume at a certain point:
  *
  * \code
  * vec3 getGradient( in vec3 a )
  * {
  *     vec3 dx = vec3( 1, 0, 0 ) * gradient_sample_distance.x;
  *     vec3 dy = vec3( 0, 1, 0 ) * gradient_sample_distance.y;
  *     vec3 dz = vec3( 0, 0, 1 ) * gradient_sample_distance.z;
  * 
  *     vec3 gradient;
  * 
  *     gradient.x = ( getIntensityAt( a + dx ) - getIntensityAt( a - dx ) ) / ( 2 * dx.x );
  *     gradient.y = ( getIntensityAt( a + dy ) - getIntensityAt( a - dy ) ) / ( 2 * dy.y );
  *     gradient.z = ( getIntensityAt( a + dz ) - getIntensityAt( a - dz ) ) / ( 2 * dz.z );
  * 
  *     return gradient;
  * }
  * \endcode
  *
  * The result is than used by \c getSpecularAmount for the computation of the brightness shift.
  * The function uses the gradient computed by \c getGradient as the surface normal and computes
  * the dot product between this normal and the given light direction. This dot product is
  * proportional to the angle between both vectors.
  *
  * \code
  * float getSpecularAmount( in vec3 a, vec3 light_direction )
  * {
  *     if( specular_amplitude == 0.0 )
  *     {
  *         return 0;
  *     }
  * 
  *     vec3 surface_normal = normalize( getGradient( a ) );
  * 
  *     float dot_result = dot( light_direction, surface_normal );
  *     float f0 = max( 0.0, dot_result ) + specular_back * max( 0.0, -dot_result );
  *     return specular_amplitude * ( f0 - specular_bias );
  * }
  * \endcode
  *
  * The fragment shader is executed once for every pixel of the screen.
  * It's main procedure first checks, whether the ray from the viewer's eye does intersect the volume at all.
  *
  * The \c frontFacesTexture already contains the ray's first volume intersection point's texture space coordinates at any given screen space point.
  * The screen space point is held by <code>gl_TexCoord[ 0 ].st</code>.
  * Analogically, the \c backFacesTexture holds the ray's \em second volume intersection point.
  *
  * Both of these textures are in RGBA format. It's RGB components carry the texture space coordinates.
  * The alpha component indicates whether the RGB components are trust worthy (alpha value is \c 1, otherwise \c 0), meaning, whether there are intersection points at all.
  *
  * The ray marching is not performed (early exit via \c discard) when the ray does not intersect the volume at two \em different locations.
  *
  * \code
  * void main()
  * {
  *     vec4 front_texture_sample = texture2D( frontFacesTexture, gl_TexCoord[ 0 ].st );
  *     vec4 back_texture_sample = texture2D( backFacesTexture, gl_TexCoord[ 0 ].st );
  * 
  *     vec3 ray_volume_entry_point = front_texture_sample.xyz;
  *     vec3 ray_volume_exit_point = back_texture_sample.xyz;
  * 
  *     if( front_texture_sample.a < 0.01
  *      || back_texture_sample.a < 0.01
  *      || distance( ray_volume_entry_point, ray_volume_exit_point ) < 0.01 )
  *     {
  *         discard;
  *     }
  * \endcode
  *
  * Next, the ray marching parameters are computed:
  *
  * \code
  *     vec3 ray_direction = normalize( ray_volume_exit_point - ray_volume_entry_point );
  *     float path_step_length = sqrt( 3.0 ) / ( sample_count - 1 );
  *     vec3 path_step = ray_direction * path_step_length;
  *
  *     int max_sample_count = int( distance( ray_volume_entry_point, ray_volume_exit_point )
  *                               / path_step_length );
  *     int real_sample_count = min( sample_count, max_sample_count );
  * \endcode
  *
  * The ray marching algorithm samples voxels along a casted ray (from \c ray_volume_entry_point in \c ray_direction) in equidistant steps (\c path_step).
  * The DVR computes the sum of the \f$ a_i \f$ weighted colors \f$ \vec f \f$ mapped to the intensities of those voxels at some ray position \f$ x_i \f$ along the ray:
  *
  * \f[ \vec v_\text{final color} = \sum\limits_{i=0}^{n - 1} a_i \cdot \vec f( \vec x_i ) \quad \text{with} \quad n := \text{sample count} \f]
  *
  * Those colors \f$ \vec f( \vec x_i ) \f$ are obtained via \c getSampleAt, the weights are a monotonously decreasing sequence:
  * The higher the alpha value \f$ f( x_i )_4 \f$ is, the greater the difference \f$ a_i - a_{i+1} \f$ becomes.
  * The concrete calculation specification for \f$a_i\f$ is token from: http://http.developer.nvidia.com/GPUGems/gpugems_ch39.html#ch39equ03
  *
  * The initial value is chosen to be \f$a_0=1\f$:
  *
  * \code
  *     float opacity = 1.0;
  * \endcode
  *
  * And then the ray marching is performed / the sum computed:
  *
  * \code
  *     for( int i = 0; i < real_sample_count; ++i )
  *     {
  *         vec3 x_i = ray_volume_entry_point + i * path_step;
  *     
  *         if( opacity < 0.01 )
  *         {
  *             break;
  *         }
  *     
  *         vec4 sample = getSampleAt( a )
  *                     + getSpecularAmount( a, ray_direction )
  *                     * vec4( 1, 1, 1, 0 );
  *     
  *         float scaled_alpha = 1 - pow( 1 - sample.a, 256.0 / sample_count );
  *         result += sample.rgb * opacity * scaled_alpha;
  *         opacity *= 1 - scaled_alpha;
  *     }
  * \endcode
  *
  * With \f$x_i\f$ as \c x_i. When \f$a_i\f$ falls below a certain threshold, the computation is early finished.
  * To make things simple, the ray direction is used as light direction for the specular lighting computation.
  *
  * Finally, the resulted sum \f$ \vec v_\text{final color} \f$ is written to the framebuffer's color channels:
  *
  * \code
  *     gl_FragColor = vec4( result, 1.0 );
  * }
  * \endcode
  *
  * \author Leonid Kostrykin
  * \date   16.8.11 - 26.10.12
  */
class CARNA_EXPORT DirectVolumeRendering : public VolumeRendererSinglePassMode
{

    Q_OBJECT

public:

    /** \brief	Holds the \ref VolumeRendererMode::name "human-readable name" of this volume rendering mode.
      */
    const static std::string NAME;


    /** \brief  Instantiates.
      */
    explicit DirectVolumeRendering( base::model::Scene& model );

    /** \brief  Releases the color map texture.
      */
    virtual ~DirectVolumeRendering();


    virtual void resizeBuffers( int width, int height ) override;

    /** \brief  Acquires color map texture.
      */
    virtual void prepareForRenderer( VolumeRenderer& ) override;

    /** \brief  Binds color map texture and starts color map rebuild timer.
      */
    virtual void start() override;

    /** \brief  Halts color map rebuild timer.
      */
    virtual void stop() override;


    virtual void renderColor( RayMarching& ) override;

    virtual void renderDepth( RayMarching& ) override;


    virtual QWidget* createControllerUI() override;

    /** \brief  Tells whether an \ref base::model::Object3D is selected to be re-positioned.
      */
    bool hasPositionedObject() const;
    
    /** \brief  References the \ref base::model::Object3D to be re-positioned.
      *
      * \pre    <code>hasPositionedObject()</code>
      */
    base::model::Object3D& getPositionedObject() const;


    /** \brief  Tells the currently configured light amplitude.
      */
    double getLightAmplitude() const;
    
    /** \brief  Tells the currently configured light bias.
      */
    double getLightBias() const;
    
    /** \brief  Tells whether lighting is enabled.
      */
    bool hasLighting() const;
    
    /** \brief  Tells the currently configured light amount multiplier for back faces.
      */
    double getLightBackFaceAmount() const;
    
    /** \brief
      * Tells the currently configured absorption threshold \f$\theta \in [0,1]\f$ to light rays:
      * when the ray intensity becomes \em higher than \f$1 - \theta\f$, the ray is assumed to be absorbed completely.
      * This is important for the 3D coordinates computation of clicked pixels in 2D space.
      */
    double getPickingSaturation() const;


    /** \brief  References the mutable data model.
      */
    base::model::Scene& mutableModel();

    /** \brief  References the mutable color map.
      */
    ColorMap& colorMap();
    
    /** \brief  References the color map.
      */
    const ColorMap& colorMap() const;


protected:

    virtual void configureShader( RayMarching&, base::view::ShaderProgram::Binding& ) override;


public slots:

    /** \brief  Sets \a object to be re-positioned.
      *
      * \post   <code>hasPositionedObject() && &getPositionedObject == &object</code>
      */
    void setPositionedObject( Carna::base::model::Object3D& object );

    /** \brief  Sets no \ref base::model::Object3D to be re-positioned.
      *
      * \post   <code>!hasPositionedObject()</code>
      */
    void removePositionedObject();

    /** \brief  Sets the currently configured light amplitude to \a lightAmplitude.
      *
      * \post   <code>base::isEqual( getLightAmplitude(), lightAmplitude )</code>
      */
    void setLightAmplitude( double lightAmplitude );
    
    /** \brief  Sets the currently configured light bias to \a lightBias.
      *
      * \post   <code>base::isEqual( getLightBias(), lightBias )</code>
      */
    void setLightBias( double lightBias );
    
    /** \brief  Sets whether lighting is enabled.
      *
      * \post   <code>hasLighting() == lighting</code>
      */
    void setLighting( bool lighting );
    
    /** \brief  Sets the currently configured light amount multiplier for back faces.
      *
      * \post   <code>base::isEqual( getLightBackFaceAmount(), lightBackFaceAmount )</code>
      */
    void setLightBackFaceAmount( double lightBackFaceAmount );
    
    /** \brief
      * Sets the currently configured absorption threshold \f$\theta \in [0,1]\f$ to light rays:
      * when the ray intensity becomes \em higher than \f$1 - \theta\f$, the ray is assumed to be absorbed completely.
      * This is important for the 3D coordinates computation of clicked pixels in 2D space.
      *
      * \post   <code>base::isEqual( getPickingSaturation(), pickingSaturation )</code>
      */
    void setPickingSaturation( double pickingSaturation );


    /** \brief  Commits color map changes immediately.
      *
      * As described \ref DVR_ColorMap "here", changes to the color map usually are not applied immediately.
      * Use this method whenever you need to ensure that the renderer's state is up to date.
      *
      * \since  \ref v_2_5
      */
    void commitColorMap() const;


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // VolumeRenderings :: DVR :: DirectVolumeRendering



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_DIRECTVOLUMERENDERING_H_6014714286
