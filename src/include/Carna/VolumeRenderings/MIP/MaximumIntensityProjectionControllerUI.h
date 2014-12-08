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

#ifndef MIP_MAXIMUMINTENSITYPROJECTIONCONTROLLERUI_H_6014714286
#define MIP_MAXIMUMINTENSITYPROJECTIONCONTROLLERUI_H_6014714286

/** \file   MaximumIntensityProjectionControllerUI.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::MaximumIntensityProjectionControllerUI.
  */

#include <Carna/Carna.h>
#include <Carna/base/qt/RotatingColor.h>
#include <Carna/VolumeRenderings/MIP/ChannelReplaceFunction.h>
#include <QWidget>

class QVBoxLayout;

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: MaximumIntensityProjectionControllerUI
// ----------------------------------------------------------------------------------

/** \brief  Configuration widget of MIP::Mode
  *
  * Allows creation of new, deleting and moving of existing channels through an
  * intuitive user interface.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11
  */
class CARNA_EXPORT MaximumIntensityProjectionControllerUI : public QWidget
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit MaximumIntensityProjectionControllerUI
        ( MaximumIntensityProjection& mode
        , QWidget* parent = nullptr );


public slots:

    /** \brief  Creates a new channel using a rotating color, covering the whole HUV
      *         range from \f$ -1024 \f$ up to \f$ 3071 \f$.
      */
    void addChannel();

    /** \brief  Creates a new channel using given parameters and a rotating color.
      */
    void addChannel
        ( int min
        , int max
        , float opacity = 1.f
        , Carna::VolumeRenderings::MIP::ChannelFunction* const = new ChannelReplaceFunction() );

    /** \brief  Creates a new channel using given parameters.
      */
    void addChannel
        ( int min
        , int max
        , const QColor& c0
        , const QColor& c1
        , float opacity = 1.f
        , Carna::VolumeRenderings::MIP::ChannelFunction* const = new ChannelReplaceFunction() );

    /** \brief  Deletes all channels.
      */
    void removeAllChannels();


    /** \brief  Prompts the user for an XML output file and saves current channels there.
      */
    void saveColorConfig();

    /** \brief  Prompts the user for an XML input file and loads channels from there.
      */
    void loadColorConfig();


private slots:

    /** \brief  Deletes specified channel.
      */
    void deleteChannel( Carna::VolumeRenderings::MIP::ChannelUiAnchor& ch );

    /** \brief  Ascends specified channel by priority.
      */
    void ascendChannel( Carna::VolumeRenderings::MIP::ChannelUiAnchor& ch );

    /** \brief  Descends specified channel by priority.
      */
    void descendChannel( Carna::VolumeRenderings::MIP::ChannelUiAnchor& ch );


private:

    MaximumIntensityProjection& mode;

    /** \brief  Layout which holds the ChannelUiAnchor instances.
      */
    QVBoxLayout* const channels;

    /** \brief  Holds the color that will be used next time for channel creation when
      *         no explicit color is given.
      */
    base::qt::RotatingColor nextColor;


    void addChannelAnchor( Channel& );

}; // MIP :: MaximumIntensityProjectionControllerUI



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_MAXIMUMINTENSITYPROJECTIONCONTROLLERUI_H_6014714286
