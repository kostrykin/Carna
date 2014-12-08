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

#ifndef OBJECT3D_H_6014714286
#define OBJECT3D_H_6014714286

/** \file   Object3D.h
  * \brief  Defines \ref Carna::base::model::Object3D.
  */

#include <QIcon>
#include <Carna/base/model/Position.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/Vector3.h>
#include <Carna/base/Transformation.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Object3D
// ----------------------------------------------------------------------------------

/** \brief  Represents an object in 3D space.
  *
  * The way \c %Object3D instances are rendered is described \ref DisplayingCustom3D "here" in detail.
  *
  * Instances of this class are part of the data model and therefore composed by
  * \ref Scene objects. At the highest abstraction level, which this class describes,
  * every such object has the only attribute \em position.
  *
  * \see
  * Provided implementations are \ref view::Point3D, \ref view::Polyline3DVertex and the abstract \ref model::RotatableObject3D class.
  *
  * \section Object3D_Deriving Creating custom implementation
  *
  * In order to create a custom implementation you need to define two methods:
  *
  * - \ref paint performs all necessary GL drawing calls to render the object.
  * - \ref paintFalseColor renders the same geometry but in the specified color.
  *
  * The latter one is used for a simple picking implementation. Therefore it is
  * not just legitimate, but convenient to paint the geometry in reduced quality
  * in \ref paintFalseColor.
  *
  * Deriving from the abstract \c %Object3D class is as simple as:
  *
  * \code
  * class MyObject3D : public Carna::base::model::Object3D
  * {
  *
  * public:
  * 
  *     MyObject3D( Carna::base::model::Scene& model ) : Carna::base::model::Object3D( model ){ }
  *
  *
  *     virtual void paint( const Carna::base::view::Renderer& ) const override;
  *
  *     virtual void paintFalseColor
  *         ( const Carna::base::view::Renderer&
  *         , const Carna::base::Vector3ui& ) const override;
  *
  * };
  * \endcode
  *
  * An implementation could look as follows. The code below produces a yellow
  * dot where the object is located:
  *
  * \code
  * using Carna::base::view::Renderer;
  * using Carna::base::Vector3ui;
  * 
  * void MyObject3D::paintFalseColor( const Renderer& renderer, const Vector3ui& color ) const
  * {
  *     glColor3ub( color.x, color.y, color.z );
  *     glBegin( GL_POINTS );
  *     glVertex3f
  *         ( position().toVolumeUnits()[ 0 ]
  *         , position().toVolumeUnits()[ 1 ]
  *         , position().toVolumeUnits()[ 2 ] );
  *     glEnd();
  * }
  *
  * void MyObject3D::paint( const Renderer& renderer ) const
  * {
  *     paintFalseColor( renderer, Vector3ui( 255, 255, 0 ) );
  * }
  * \endcode
  *
  * The utilized coordinate systems are documented \ref CoordinateSystems "here".
  *
  * \author Leonid Kostrykin
  * \date   2010 - 18.6.14
  */
class CARNA_EXPORT Object3D : public QObject, public Scene::BaseObject3D
{

    Q_OBJECT

public:

    /** \brief  Instantiates at position \f$ (0,0,0)^\mathrm T \f$ in \ref VolumeSpace "volume space coordinates".
      *
      * \post   <code>name() == objectName</code>
      *
      * \post   <code>position().toVolumeUnits() == Carna::Vector( 0, 0, 0 )</code>
      */
    explicit Object3D
        ( Scene& model
        , const std::string& objectName = createDefaultName() );

    /** \brief  Instantiates at given position.
      *
      * \post   <code>name() == objectName</code>
      *
      * \post   <code>position() == objectPosition</code>
      */
    Object3D
        ( Scene& model
        , const Position& objectPosition
        , const std::string& objectName = createDefaultName() );

    /** \brief  Emits the \ref destroyed signals.
      */
    virtual ~Object3D();


    /** \brief  Returns the position of this object.
      */
    const Position& position() const;

    /** \brief  Sets the position of this object.
      *
      * Emits the \ref moved signal.
      *
      * Also the \ref Scene::objectsChanged signal is emitted, though maybe delayed.
      */
    virtual void setPosition( const Position& pos );


    /** \brief  Tells the local coordinate system of this object.
      *
      * The default implementation always returns identity:
      * Derive from \ref RotatableObject3D if you need to create an object that is rotatable.
      */
    virtual const Transformation& rotation() const;

    /** \brief  Defines the local coordinate system of this object.
      *
      * The default implementation does nothing:
      * Derive from \ref RotatableObject3D if you need to create an object that is rotatable.
      */
    virtual void setRotation( const Transformation& );


    /** \brief  Convenience function that invokes \ref setPosition internally.
      */
    void move( const Position& offset );


    /** \brief  Renames this object to the value of \a newName.
      *
      * Names are only relevant for object identification by the user.
      *
      * \post   <code>name() == newName</code>
      *
      * Emits the \ref renamed signals.
      *
      * Also the \ref Scene::objectsChanged signal is emitted, though maybe delayed.
      */
    void setName( const std::string& newName );

    /** \brief  Tells this object's name.
      */
    const std::string& name() const;


    /** \brief  Draws this object.
      */
    virtual void paint( const view::Renderer& ) const = 0;

    /** \brief  Draw this object using all-over the specified color.
      *
      * This method is supposed to be used by picking mechanisms. Therefore it is
      * legitimate to paint the geometry in reduced quality here.
      */
    virtual void paintFalseColor( const view::Renderer&, const QColor& ) const;
    
    /** \brief  Draw this object using all-over the specified color.
      *
      * This method is supposed to be used by picking mechanisms. Therefore it is
      * legitimate to paint the geometry in reduced quality here.
      */
    virtual void paintFalseColor( const view::Renderer&, const Vector3ui& color ) const = 0;


    /** \brief  References an icon representation of this object.
      *
      * Underlying default implementation gives an empty \f$ 10 \times 10 \f$ icon.
      */
    virtual const QIcon& icon() const;


signals:

    /** \brief  This object has been displaced.
      *
      * Emitted by \ref setPosition and \ref move.
      */
    void moved();

    /** \brief  This object has been rotated.
      */
    void rotated();

    /** \brief  This object has been renamed.
      */
    void renamed();
    
    /** \brief  This object has been renamed to the given name.
      */
    void renamed( const std::string& );
    
    /** \brief  This object has been renamed to the given name.
      */
    void renamed( const QString& );
    
    /** \brief  This object has been deleted.
      *
      * \note
      * An argument-less \c destroyed signal is inherited from \c QObject.
      *
      * The argument references this object.
      */
    void destroyed( Carna::base::model::Object3D& );


protected:

    /** \brief  Creates and returns a new default object name.
      */
    static std::string createDefaultName();


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // Object3D



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3D_H_6014714286
