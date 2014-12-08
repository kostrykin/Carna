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

#ifndef POINT3D_H_6014714286
#define POINT3D_H_6014714286

/** \file   Point3D.h
  * \brief  Defines \ref Carna::base::view::Point3D.
  */

#include <Carna/base/model/Object3D.h>
#include <Carna/base/qt/RotatingColor.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Point3D
// ----------------------------------------------------------------------------------

/** \brief  Represents a point in 3D space.
  *
  * Points do have the attributes \em color and \em size beside \em position.
  *
  * \author Leonid Kostrykin
  * \date   2010 - 21.3.13
  */
class CARNA_EXPORT Point3D : public model::Object3D
{

    Q_OBJECT

public:

    /** \brief  Determines the size to use when size argument is not specified.
      */
    static const float defaultSize;


    /** \brief  Instantiates with default size and position \f$ (0,0,0) \f$.
      *
      * \param  model           references the data model.
      * \param  rotatingColor   uses rotating colors if \c true is given.
      */
    explicit Point3D
        ( model::Scene& model
        , bool rotatingColor = true );

    /** \brief  Instantiates.
      *
      * \param  model           references the data model.
      * \param  position        specifies the position of this object.
      * \param  size            specifies the size of this object.
      * \param  rotatingColor   uses rotating colors if \c true is given.
      */
    Point3D
        ( model::Scene& model
        , const model::Position& position
        , float size = defaultSize
        , bool rotatingColor = true );

    /** \brief  Instantiates.
      *
      * \param  model       references the data model.
      * \param  position    specifies the position of this object.
      * \param  color       specifies the color of this object.
      * \param  size        specifies the size of this object.
      */
    Point3D
        ( model::Scene& model
        , const model::Position& position
        , const QColor& color
        , float size = defaultSize );

    /** \brief  Destroys itself.
      */
    virtual ~Point3D();


    virtual void paint( const Renderer& ) const override;

    virtual void paintFalseColor( const Renderer&, const Vector3ui& ) const override;


    virtual const QIcon& icon() const override;


    /** \brief  Tells the currently set color.
      */
    const QColor& color() const;

    /** \brief  Tells this point's current size.
      */
    float size() const;


public slots:

    /** \brief  Sets this point's size and emits the \ref sizeChanged signals.
      *
      * \post   <code>Carna::base::isEqual( size(), newSize )</code>
      *
      * Emits the \ref sizeChanged signals.
      */
    void setSize( float newSize );

    /** \copydoc setSize(float)
      */
    void setSize( double );

    /** \brief  Sets a color and emits the \ref colorChanged signals.
      *
      * \post   <code>color() == newColor</code>
      *
      * Emits the \ref colorChanged signals.
      */
    void setColor( const QColor& newColor );


signals:
    
    /** \brief  Emitted when this point's color has changed.
      */
    void colorChanged();

    /** \brief  Emitted when this point's color has changed to the given color.
      */
    void colorChanged( const QColor& );

    
    /** \brief  Emitted when this point's size has changed.
      */
    void sizeChanged();
    
    /** \brief  Emitted when this point's size has changed to the given size.
      */
    void sizeChanged( float );
    
    /** \brief  Emitted when this point's size has changed to the given size.
      */
    void sizeChanged( double );


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // Point3D



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // POINT3D_H_6014714286
