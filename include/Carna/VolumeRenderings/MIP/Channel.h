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

#ifndef MIP_CHANNEL_H_6014714286
#define MIP_CHANNEL_H_6014714286

/** \file   Channel.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::Channel.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/IntSpan.h>
#include <Carna/VolumeRenderings/MIP/ChannelReplaceFunction.h>
#include <QColor>

class QSpinBox;
class QCheckBox;
class QVBoxLayout;

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: MIP :: Channel
// ----------------------------------------------------------------------------------

/** \brief  Defines an HUV range and the way it's visualized.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11
  */
class CARNA_EXPORT Channel : public QObject
{

    NON_COPYABLE

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    Channel
        ( const base::IntSpan& huv
        , const QColor& minColor
        , const QColor& maxColor
        , float opacity = 1.f
        , ChannelFunction* function = new ChannelReplaceFunction() );

    /** \brief	Releases acquired resources.
      */
    virtual ~Channel();


    /** \brief  References the huv range.
      */
    const base::IntSpan& getHuvRange() const
    {
        return huv;
    }


    /** \brief  References \f$ \mathrm{ color }_0 \f$.
      */
    const QColor& getMinColor() const
    {
        return minColor;
    }


    /** \brief  References \f$ \mathrm{ color }_1 \f$.
      */
    const QColor& getMaxColor() const
    {
        return maxColor;
    }


    /** \brief  References a factor which the alpha components of
      *         \f$ \mathrm{ color }_0 \f$ and \f$ \mathrm{ color }_1 \f$ are multiplied
      *         with before visualization.
      */
    float getOpacity() const
    {
        return opacity;
    }


    /** \brief  References the channel function \f$ \mathrm{ cf } \f$.
      */
    const ChannelFunction& getFunction() const;

    /** \brief  Sets the channel function \f$ \mathrm{ cf } \f$.
      */
    void setFunction( ChannelFunction* f );


public slots:

    /** \brief  Sets the huv range.
      */
    void setHuvRange( const Carna::base::IntSpan& huv )
    {
        this->huv = huv;
        emit changed();
    }

    /** \brief  Sets \f$ \mathrm{ huv }_0 \f$.
      */
    void setMinHuv( int huv )
    {
        this->huv.first = huv;
        emit changed();
    }

    /** \brief  Sets \f$ \mathrm{ huv }_1 \f$.
      */
    void setMaxHuv( int huv )
    {
        this->huv.last = huv;
        emit changed();
    }

    /** \brief  Sets \f$ \mathrm{ color }_0 \f$.
      */
    void setMinColor( const QColor& minColor )
    {
        this->minColor = minColor;
        emit changed();
    }

    /** \brief  Sets \f$ \mathrm{ color }_1 \f$.
      */
    void setMaxColor( const QColor& maxColor )
    {
        this->maxColor = maxColor;
        emit changed();
    }

    /** \brief  Sets a factor which the alpha components of
      *         \f$ \mathrm{ color }_0 \f$ and \f$ \mathrm{ color }_1 \f$ are multiplied
      *         with before visualization.
      */
    void setOpacity( float opacity )
    {
        this->opacity = opacity;
        emit changed();
    }


signals:

    /** \brief  Emitted when channel data is changed.
      */
    void changed();


private: // TODO: move to PIMPL

    /** \brief  Holds the huv range.
      */
    base::IntSpan huv;

    /** \brief  Holds \f$ \mathrm{ color }_0 \f$.
      */
    QColor minColor;

    /** \brief  Holds \f$ \mathrm{ color }_1 \f$.
      */
    QColor maxColor;

    /** \brief  Holds a factor which the alpha components of
      *         \f$ \mathrm{ color }_0 \f$ and \f$ \mathrm{ color }_1 \f$ are multiplied
      *         with before visualization.
      */
    float opacity;

    /** \brief  Holds the channel function \f$ \mathrm{ cf } \f$.
      */
    std::unique_ptr< ChannelFunction > function;

}; // VolumeRenderings :: MIP :: Channel



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_CHANNEL_H_6014714286
