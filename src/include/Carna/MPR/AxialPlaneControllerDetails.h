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

#ifndef MPR_AXIALPLANECONTROLLER_DETAILS_H_6014714286
#define MPR_AXIALPLANECONTROLLER_DETAILS_H_6014714286

#include <Carna/MPR/AxialPlaneController.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneController :: Details
// ----------------------------------------------------------------------------------

class AxialPlaneController :: Details : public QObject
{

    NON_COPYABLE

    Q_OBJECT

    AxialPlaneController& self;

public:

    explicit Details( AxialPlaneController& );

    virtual ~Details();


    std::unique_ptr< base::model::Position > clickedPosition;

    bool parentHasContextMenu;


public slots:

    void spawnPoint();

}; // MPR :: AxialPlaneController :: Details



}  // namespace MPR

}  // namespace Carna

#endif // MPR_AXIALPLANECONTROLLER_DETAILS_H_6014714286
