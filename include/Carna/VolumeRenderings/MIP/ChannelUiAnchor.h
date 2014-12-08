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

#ifndef MIP_CHANNELUIANCHOR_H_6014714286
#define MIP_CHANNELUIANCHOR_H_6014714286

/** \file   ChannelUiAnchor.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::ChannelUiAnchor.
  */

#include <Carna/Carna.h>
#include <QWidget>
#include <functional>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: MIP :: ChannelUiAnchor
// ----------------------------------------------------------------------------------

/** \brief  User interface representation of a \ref Channel that provides \ref ChannelEditor.
  *
  * Basically a ChannelEditor with buttons for deleting, ascending and descending the
  * channel by priority.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11 - 26.4.13
  */
class CARNA_EXPORT ChannelUiAnchor : public QWidget
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit ChannelUiAnchor( Channel& );

    /** \brief	Releases acquired resources.
      */
    virtual ~ChannelUiAnchor();


    /** \brief  Returns the optimal size.
      */
    virtual QSize sizeHint() const;


    /** \brief  References the represented channel.
      */
    Channel& channel;


signals:

    /** \brief  The associated channel is going to be deleted.
      */
    void toBeDeleted( Carna::VolumeRenderings::MIP::ChannelUiAnchor& whom );

    /** \brief  The associated channel is going to be ascended by priority.
      */
    void toBeAscended( Carna::VolumeRenderings::MIP::ChannelUiAnchor& whom );

    /** \brief  The associated channel is going to be descended by priority.
      */
    void toBeDescended( Carna::VolumeRenderings::MIP::ChannelUiAnchor& whom );


private slots:

    /** \brief  Updates internal state.
      */
    void deleteMe();

    /** \brief  Updates internal state.
      */
    void ascendMe();

    /** \brief  Updates internal state.
      */
    void descendMe();


private:

    /** \brief  References the channel editor which is associated with the channel.
      */
    ChannelEditor* editor;

}; // VolumeRenderings :: MIP :: ChannelUiAnchor



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_CHANNELUIANCHOR_H_6014714286
