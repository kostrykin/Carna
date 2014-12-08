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

#ifndef XRAY_H_6014714286
#define XRAY_H_6014714286

/** \file   XRay.h
  * \brief  Defines \ref Carna::VolumeRenderings::DRR::XRay.
  */

#include <Carna/VolumeRenderings/VolumeRendererSinglePassMode.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace DRR
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: DRR :: XRay
// ----------------------------------------------------------------------------------

/** \brief
  * \ref VolumeRendererMode that implements Digital Radiograph Reconstruction (DRR).
  *
  * \image html XRay01.png
  *
  * \section XRayUsage Usage
  *
  * An example is provided within the documentation of \ref VolumeRenderer.
  *
  * \section XRayDetails Implementation Details
  *
  * The algorithm is based on ray marching. For each screen fragment, a ray is casted
  * through the volume in order to determine the final <i>pixel intensity</i> \f$I\f$ at
  * this particular screen pixel:
  *
  * \image html XRay02.png
  *
  * Lets define the \em intensity
  * \f[ i(x) = \frac{ \mathrm{hu}(x) + 1024 }{ 4096 } \in [0,1] \f]
  *
  * and the \em attenuation
  * \f[ \mu(x) = \mu_\text{water} \cdot \bigg( 1 + \frac{ \mathrm{hu}(x) }{ 1000 } \bigg) \f]
  *
  * with the constant \f$\mu_\text{water}\f$.
  *
  * Then we can compute the <i>pixel intensity</i> as
  * \f[ I = I_0 \cdot \exp\bigg( -\int\limits_{x_0}^{x_1} \delta\big(i(x)\big) \cdot \mu(x)\ \mathrm dx \bigg) \f]
  *
  * where
  * \f[
  * \delta(i) =
  * \begin{cases}
  * 0 & \text{ if }\quad i_\text{lower} > i \\
  * 1 & \text{ if }\quad i_\text{lower} \leq i < i_\text{upper} \\
  * k & \text{ if }\quad i_\text{upper} \leq i
  * \end{cases} \f]
  *
  * with \f$ k \f$ being some arbitrary positive factor.
  *
  * Thus \f$ \mu(x) \f$ is scaled differently based on which one out of three zones it is located in:
  *
  * \image html XRay03.png
  * 
  * \see    xray.frag
  *
  * \author Leonid Kostrykin
  * \date   8.8.11 - 27.9.12
  */
class CARNA_EXPORT XRay : public VolumeRendererSinglePassMode
{

    Q_OBJECT

public:
    
    /** \brief	Holds the \ref VolumeRendererMode::name "human-readable name" of this volume rendering mode.
      */
    const static std::string NAME;


    /** \brief  Instantiates.
      */
    explicit XRay( const base::model::Scene& model );

    /** \brief  Releases acquired resources.
      */
    virtual ~XRay();


    /** \brief  Returns current \f$\mu_\text{water}\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    double waterAttenuation() const;

    /** \brief  Returns current \f$I_0\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    double brightness() const;

    /** \brief  Returns current \f$4096 \cdot i_\text{lower} - 1024\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    int lowerThreshold() const;
    
    /** \brief  Returns current \f$4096 \cdot i_\text{upper} - 1024\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    int upperThreshold() const;
    
    /** \brief  Returns current \f$k\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    double upperMultiplier() const;
    
    /** \brief  Tells the level the final pixel intensities \f$I\f$ are windowed by.
      *
      * Refer to \ref XRayDetails for further information.
      */
    double windowingLevel() const;
    
    /** \brief  Tells the width the final pixel intensities \f$I\f$ are windowed by.
      *
      * Refer to \ref XRayDetails for further information.
      */
    double windowingWidth() const;

    /** \brief  Returns mask contrast enhancement.
      *
      * \see    \ref setMaskContrast
      */
    double maskContrast() const;


    virtual QWidget* createControllerUI() override;


protected:

    virtual void configureShader
        ( RayMarching&
        , base::view::ShaderProgram::Binding& ) override;


private:

    class Details;
    std::unique_ptr< Details > pimpl;


public slots:
    
    /** \brief  Sets current \f$\mu_\text{water}\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    void setWaterAttenuation( double mu_water );
    
    /** \brief  Sets current \f$I_0\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    void setBrightness( double brightness );
    
    /** \brief  Sets current \f$4096 \cdot i_\text{lower} - 1024\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    void setLowerThreshold( int lower_threshold );
    
    /** \brief  Sets current \f$4096 \cdot i_\text{upper} - 1024\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    void setUpperThreshold( int upper_threshold );
    
    /** \brief  Sets current \f$k\f$ setting.
      *
      * Refer to \ref XRayDetails for further information.
      */
    void setUpperMultiplier( double upper_multiplier );

    /** \brief  Sets the level the final pixel intensities \f$I\f$ are windowed by.
      *
      * Refer to \ref XRayDetails for further information.
      */
    void setWindowingLevel( double window_level );
    
    /** \brief  Sets the width the final pixel intensities \f$I\f$ are windowed by.
      *
      * Refer to \ref XRayDetails for further information.
      */
    void setWindowingWidth( double window_width );
    
    /** \brief  Sets the mask contrast enhancement.
      *
      * \see    \ref maskContrast
      */
    void setMaskContrast( double mask_contrast );


signals:

    /** \brief  Emitted by \ref setWaterAttenuation.
      */
    void waterAttenuationChanged( double mu_water );

    /** \brief  Emitted by \ref setBrightness.
      */
    void brightnessChanged( double brightness );

    /** \brief  Emitted by \ref setLowerThreshold.
      */
    void lowerThresholdChanged( int lower_threshold );
    
    /** \brief  Emitted by \ref setUpperThreshold.
      */
    void upperThresholdChanged( int upper_threshold );
    
    /** \brief  Emitted by \ref setUpperMultiplier.
      */
    void upperMultiplierChanged( double upper_multiplier );
    
    /** \brief  Emitted by \ref setWindowingLevel.
      */
    void windowingLevelChanged( double window_level );
    
    /** \brief  Emitted by \ref setWindowingWidth.
      */
    void windowingWidthChanged( double window_width );
    
    /** \brief  Emitted by \ref setMaskContrast.
      */
    void maskContrastChanged( double mask_contrast );

}; // VolumeRenderings :: DRR :: XRay



}  // namespace Carna :: VolumeRenderings :: DRR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // XRAY_H_6014714286
