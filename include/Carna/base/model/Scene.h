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

#ifndef SCENE_H_6014714286
#define SCENE_H_6014714286

/** \file   Scene.h
  * \brief  Defines \ref Carna::base::model::Scene.
  */

#include <Carna/base/Association.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/model/ScalarField.h>
#include <memory>
#include <deque>
#include <QObject>

class DicomSeries;  // from dicom-interface

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Scene
// ----------------------------------------------------------------------------------

/** \brief  Defines the data model.
  *
  * The model's main purpose is to provide access to the \em volume \em data: this data
  * consists of a scalar field, which is described by an associated \ref Volume object,
  * and is enriched with some extra informations by the model, that is required for it's
  * visualization, such as the distance between two neighbor voxels. These distances are
  * measured in millimeters and called \em spacing. Furthermore it is the purpose of the
  * data model to suggest a global windowing, which however does not have to be taken
  * into account by a specific view. A single \c %Scene instance can be used by any
  * number of views.
  *
  * Besides volume data, the spacing and windowing, the data model can be used to provide
  * additional, custom geometries: these are represented by a set of
  * \ref Object3D "3D objects".
  *
  * It is explicitly not possible to switch the model assigned to a view during the
  * view's lifetime. If this is necessary, the view must be re-instantiated.
  *
  * There are two ways of creating a \c %Scene instance:
  *
  * -# Either you can use one of the pre-defined routines of the \ref SceneFactory, or
  * -# you must use \ref Scene::Scene "the Scene's constructor" directly. In this case
  *    you will have to define the association with the volume data: particularly you
  *    will have to decide whether the data model will take ownership of the volume data
  *    (utilizing \ref Composition) or whether the model is only to reference the
  *    data (choosing \ref Aggregation). The latter case will allow the volume
  *    data to exist independently of the model's lifetime.
  *
  * \warning
  * You \em must initialize \c QApplication (or any of it's derivations, such as
  * \ref Application "Carna::Application") \em before the first usage of \c %Scene.
  * Speaking strictly, you must ensure that \c QApplication is instantiated by the time
  * that \ref view::SceneProvider::max3DTextureSize is invoked for the first time. Such
  * an invocation is performed by the \ref SceneFactory internally, for instance. If
  * you instantiate \c %Scene by yourself, using its constructor, you might be able to
  * prolong the instantiation of \c QApplication.
  *
  * Here are some instantiation examples:
  *
  * \code
  * using namespace Carna;
  * using namespace Carna::base;
  * using namespace Carna::base::model;
  *
  * const float spacingXY = 1.f;
  * const float spacingZ  = 1.f;
  *
  * // using the constructor (2)
  *
  * UInt16Volume volume( Vector3ui( 128, 128, 128 ) );
  *
  * Scene model1( new Aggregation< Volume >( volume )
  *             , spacingXY, spacingXY, spacingZ );
  *
  * Scene model2( new Composition< Volume >
  *         ( new UInt16Volume( Vector3ui( 128, 128, 128 ) ) )
  *         , spacingXY, spacingXY, spacingZ );
  *
  * // using SceneFactory from Carna-DICOM project (1)
  * // https://github.com/RWTHmediTEC/Carna-DICOM
  *
  * using Carna::dicom::DicomSceneFactory;
  * DicomSceneFactory modelFactory;
  *
  * std::unique_ptr< Scene > model3( modelFactory.createFromUserInput() );
  * \endcode
  *
  * \author Leonid Kostrykin
  * \date   26.10.11 - 15.3.13
  */
class CARNA_EXPORT Scene : public QObject
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief
      * Base class of \ref Object3D "3D objects", giving them the opportunity of an exclusive communication with the data model.
      *
      * \warning
      * Do not inherit directly from this class! Instead, inherit from \ref Object3D.
      */
    class CARNA_EXPORT BaseObject3D
    {

        NON_COPYABLE

    public:

        /** \brief  Removes the object from the data model.
          */
        virtual ~BaseObject3D();


        /** \brief  References the data model this object belongs to.
          */
        Scene& model;


    protected:

        /** \brief  Puts the object into the data model.
          */
        explicit BaseObject3D( Scene& model );


        /** \brief
          * Notifies all objects connected to \ref Scene::objectsChanged that the state
          * of one or more \ref Object3D "3D objects" might have changed and objects
          * might have been added or removed.
          */
        void invalidateObjects3D( Object3DEvent& ev );

    }; // BaseObject3D

 // ----------------------------------------------------------------------------------

    /** \brief
      * Instantiates the data model.
      *
      * \pre    <code>volume != nullptr && volume->get() != nullptr</code>
      *
      * \pre    <code>( *volume )->size.x >= 2</code>
      *
      * \pre    <code>( *volume )->size.y >= 2</code>
      *
      * \pre    <code>( *volume )->size.z >= 2</code>
      *
      * \pre    <code>spacingX > 0</code>
      *
      * \pre    <code>spacingY > 0</code>
      *
      * \pre    <code>spacingZ > 0</code>
      *
      * \bug    Note that the volume's width must be even.
      *         This is a known bug that will be solved as soon as the volume concept is refactored.
      */
    Scene( Association< Volume >* volume, float spacingX, float spacingY, float spacingZ );

    /** \brief
      * Deletes all associated 3D objects.
      */
    ~Scene();

 // ----------------------------------------------------------------------------------

    /** \brief  References the associated volume data.
      */
    const Volume& volume() const;

 // ----------------------------------------------------------------------------------

    /** \brief  Tells the distance between two voxels in millimeters.
      *
      * \post
      * <code>spacingX() > 0</code>
      * 
      * The result is based on the value passed to the constructor.
      */
    float spacingX() const;

    /** \brief  Tells the distance between two voxels in millimeters.
      *
      * \post
      * <code>spacingY() > 0</code>
      * 
      * The result is based on the value passed to the constructor.
      */
    float spacingY() const;

    /** \brief  Tells the distance between two voxels in millimeters.
      *
      * \post
      * <code>spacingZ() > 0</code>
      * 
      * The result is based on the value passed to the constructor.
      */
    float spacingZ() const;

 // ----------------------------------------------------------------------------------

    /** \brief  Returns a guessed HU value, voxels below which may be treated as void.
      *
      * \post
      * <code>recommendedVoidThreshold() >= -1024 && recommendedVoidThreshold() <= 3071</code>
      * 
      * The result is based on the value passed to \ref setRecommendedVoidThreshold previously.
      */
    signed int recommendedVoidThreshold() const;

    /** \brief  Tells the recommended windowing level.
      *
      * \post
      * <code>recommendedWindowingLevel() >= -1024 && recommendedWindowingLevel() <= 3071</code>
      * 
      * The result is based on the value passed to \ref setRecommendedWindowingLevel previously.
      */
    signed int recommendedWindowingLevel() const;

    /** \brief  Tells the recommended windowing width.
      *
      * \post
      * <code>recommendedWindowingWidth() >= -1024 && recommendedWindowingWidth() <= 3071</code>
      * 
      * The result is based on the value passed to \ref setRecommendedWindowingWidth previously.
      */
    signed int recommendedWindowingWidth() const;

 // ----------------------------------------------------------------------------------

    /** \brief  Returns number of \ref Object3D "3D objects" associated with this model.
      */
    unsigned int objectsCount() const;

    /** \brief  References a particular 3D object.
      *
      * \pre
      * <code>index < objectsCount()</code>
      */
    Object3D& objectByIndex( unsigned int index );

    /** \brief  References a particular 3D object.
      *
      * \pre
      * <code>index < objectsCount()</code>
      */
    const Object3D& objectByIndex( unsigned int index ) const
    {
        return const_cast< Scene* >( this )->objectByIndex( index );
    }

 // ----------------------------------------------------------------------------------

    /** \brief  Sets scalar field of voxel factors.
      *
      * \since  \ref v_2_2_2
      *
      * Emits the \ref volumeMaskExchanged signal.
      */
    void setVolumeMask( BufferedMaskAdapter* );

    /** \brief  Tells whether any scalar field of voxel factors is currently set.
      *
      * \since  \ref v_2_2_2
      */
    bool hasVolumeMask() const;
    
    /** \brief  References currently set scalar field of voxel factors.
      *
      * \pre    <code>hasVolumeMask() == true</code>
      *
      * \since  \ref v_2_2_2
      */
    const BufferedMaskAdapter& volumeMask() const;

 // ----------------------------------------------------------------------------------

    /** \brief  Samples the volume intensity at \a position.
      *
      * Trilinear interpolation is used.
      *
      * Samples outside the volume are supposed to be \f$-1024\f$.
      *
      * \since  \ref v_2_2_2
      */
    double intensityAt( const Position& position ) const;

 // ----------------------------------------------------------------------------------

public slots:

    /** \brief  Sets the suggested windowing level.
      *
      * \pre
      * <code>huv >= -1024 && huv <= 3071</code>
      *
      * Emits the \ref recommendedWindowingChanged signal.
      */
    void setRecommendedWindowingLevel( signed int huv );

    /** \brief  Sets the suggested windowing width.
      *
      * \pre
      * <code>huv >= -1024 && huv <= 3071</code>
      *
      * Emits the \ref recommendedWindowingChanged signal.
      */
    void setRecommendedWindowingWidth( signed int huv );

    /** \brief  Sets the value that is to be returned by \ref recommendedVoidThreshold later.
      *
      * \pre
      * <code>huv >= -1024 && huv <= 3071</code>
      */
    void setRecommendedVoidThreshold( signed int huv );

    /** \brief  Releases mask if any is acquired, otherwise does nothing.
      *
      * Has the same effect as:
      * \code
      * setVolumeMask( nullptr );
      * \endcode
      *
      * \since  \ref v_2_2_2
      *
      * Emits the \ref volumeMaskExchanged signal.
      */
    void removeVolumeMask();

 // ----------------------------------------------------------------------------------

signals:

    /** \brief  The suggested windowing settings may have been changed.
      *
      * Emitted when either \ref setRecommendedWindowingLevel or \ref setRecommendedWindowingWidth has been called.
      */
    void recommendedWindowingChanged();

    /** \brief  The suggested void threshold may have been changed.
      *
      * Emitted when \ref setRecommendedVoidThreshold has been called.
      *
      * \since  \ref v_2_2
      */
    void recommendedVoidThresholdChanged();

    /** \brief  The 3D object set may have been modified.
      *
      * The detailed modification is described by the argument.
      */
    void objectsChanged( const Carna::base::model::Object3DEvent& );

    /** \brief  Emitted by \ref setVolumeMask when a new (or none) mask is set.
      *
      * \since  \ref v_2_2_2
      */
    void volumeMaskExchanged();

 // ----------------------------------------------------------------------------------

private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // Scene



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // SCENE_H_6014714286
