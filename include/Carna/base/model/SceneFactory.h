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

#ifndef MODELFACTORY_H_6014714286
#define MODELFACTORY_H_6014714286

/** \file   SceneFactory.h
  * \brief  Defines \ref Carna::base::model::SceneFactory.
  */

#include <Carna/Carna.h>
#include <Carna/base/model/ScalarField.h>
#include <Carna/base/ParallelizationSettings.h>
#include <Carna/base/Vector3.h>
#include <QWidget>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// SceneFactory
// ----------------------------------------------------------------------------------

/** \brief  Offers \ref Scene instantiating with little effort.
  *
  * All factory methods do throw the same exceptions.
  *
  * \ref AssertionFailure is thrown if at least one of the following pre-conditions
  * is not met:
  *
  * - The requested number of parallelized workers is not supported by implementation.
  * - The requested target volume size is larger than the original data.
  * - The requested target volume size is lower than 2 pixels along at least one axis.
  *
  * \author Leonid Kostrykin
  * \date   26.10.11 - 21.2.13
  */
class CARNA_EXPORT SceneFactory : public QObject
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit SceneFactory
        ( QWidget* dialogParent = nullptr
        , const ParallelizationSettings& = ParallelizationSettings()
        , QObject* parent = nullptr );

    /** \brief  Sets the maximum allowed volume side length.
      *
      * Data that violates this constraint gets down-sampled automatically.
      *
      * \since  \ref v_2_5
      */
    void setMaximumAllowedSideLength( unsigned int max_allowed_side_length );
    
    /** \brief  Tells the maximum allowed volume side length.
      *
      * Data that violates this constraint gets down-sampled automatically.
      *
      * \since  \ref v_2_5
      */
    unsigned int maximumAllowedSideLength() const;

    /** \brief  Defines \f$\mathbb Z_0^3 \to \mathbb R\f$ volume mask.
      *
      * \see    \ref BinarizedScalarField, \ref createFromVolumeMasking
      */
    typedef ScalarField< float > FactorizedScalarField;

    /** \brief  Defines \f$\mathbb Z_0^3 \to \{0,1\}\f$ volume mask.
      *
      * \see    \ref FactorizedScalarField, \ref createFromVolumeMasking
      */
    typedef ScalarField< bool > BinarizedScalarField;

    /** \brief  Creates new model by multiplying the volume of an existing model with the given mask.
      */
    Scene* createFromVolumeMasking( const Scene& baseModel, const FactorizedScalarField& mask );

    /** \brief  Creates new model by multiplying the volume of an existing model with the given mask.
      */
    Scene* createFromVolumeMasking( const Scene& baseModel, const BinarizedScalarField& mask );


    /** \brief  Computes HUV 'void' threshold based on the average intensity of the outer voxels.
      *
      * \see \ref Scene::setRecommendedVoidThreshold
      *
      * \since \ref v_2_1_1
      */
    static int computeVoidThreshold( const Volume& );


protected:

    QWidget* const dialogParent;

    const ParallelizationSettings parallelizationSettings;

    unsigned int max_allowed_side_length;

    unsigned int getWorkerCount();

}; // SceneFactory



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // MODELFACTORY_H_6014714286
