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

#ifndef MIP_CHANNELEDITOR_H_6014714286
#define MIP_CHANNELEDITOR_H_6014714286

/** \file   ChannelEditor.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::ChannelEditor.
  */

#include <Carna/Carna.h>
#include <QFrame>

class QColor;
class QSpinBox;
class QCheckBox;

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: MIP :: ChannelEditor
// ----------------------------------------------------------------------------------

/** \brief  Editing widget for Channel objects
  *
  * \author Leonid Kostrykin
  * \date   26.7.11
  */
class CARNA_EXPORT ChannelEditor : public QFrame
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit ChannelEditor( Channel& channel );


    /** \brief  Returns the optimal size.
      */
    virtual QSize sizeHint() const;


private:

    /** \brief  References the channel, which is edited by this widget.
      */
    Channel& channel;

    /** \brief  Configures the channel's \f$ \mathrm{ huv }_0 \f$.
      */
    QSpinBox* const sbMin;

    /** \brief  Configures the channel's \f$ \mathrm{ huv }_1 \f$.
      */
    QSpinBox* const sbMax;

    /** \brief  Configures the factor which the alpha components of
      *         \f$ \mathrm{ color }_0 \f$ and \f$ \mathrm{ color }_1 \f$ are multiplied
      *         with before visualization.
      */
    QSpinBox* const sbOpacity;

    /** \brief  Configures whether changing \f$ \mathrm{ color }_0 \f$ automatically
      *         also changes \f$ \mathrm{ color }_1 \f$ and visa versa.
      */
    QCheckBox* const cbSyncColors;

    /** \brief  Configures the channel's \f$ \mathrm{ color }_0 \f$.
      */
    base::qt::ColorPicker* const color_picker_min;

    /** \brief  Configures the channel's \f$ \mathrm{ color }_1 \f$.
      */
    base::qt::ColorPicker* const color_picker_max;


    /** \brief  Holds whether changing \f$ \mathrm{ color }_0 \f$ automatically also
      *         changes \f$ \mathrm{ color }_1 \f$ and visa versa.
      */
    bool syncColors;
    

    /** \brief  Compares the RGB-components of two colors.
      *
      * Compares two colors, ignoring their alpha components.
      */
    static bool equalsRGB( const QColor&, const QColor& );


private slots:

    /** \brief  Updates internal state.
      */
    void functionSelected( int );

    /** \brief  Updates internal state.
      */
    void opacityChanged( int );

    /** \brief  Updates internal state.
      */
    void setColorSync( int );

    /** \brief  Updates internal state.
      */
    void setMinColor( const QColor& );

    /** \brief  Updates internal state.
      */
    void setMaxColor( const QColor& );

}; // VolumeRenderings :: MIP :: ChannelEditor



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_CHANNELEDITOR_H_6014714286
