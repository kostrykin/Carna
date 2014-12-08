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

#ifndef SCENE_DETAILS_H_6014714286
#define SCENE_DETAILS_H_6014714286

/** \file   SceneDetails.h
  * \brief  Defines \ref Carna::base::model::Scene::Details.
  */

#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Object3DEvent.h>
#include <Carna/base/Flags.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Scene :: Details
// ----------------------------------------------------------------------------------

class Scene::Details : public QObject
{

    Q_OBJECT

    NON_COPYABLE

    Scene& self;

public:

    typedef std::deque< BaseObject3D* > ObjectVector;


    Details( Scene& self, Association< Volume >* volume, float spacingX, float spacingY, float spacingZ );

    const std::unique_ptr< Association< Volume > > volume;

    std::unique_ptr< BufferedMaskAdapter > mask;


    const float spacingX;

    const float spacingY;

    const float spacingZ;


    signed int windowingLevel;

    signed int windowingWidth;


    signed int recommendedVoidThreshold;


    ObjectVector objects;


public slots:

    void postScheduledObject3DEvents();

    void scheduleObject3DEvent( const Carna::base::model::Object3DEvent& );


private:

    static const Flags< Object3DEvent::Feature > nullScheduledObject3DEvents;

    Flags< Object3DEvent::Feature > scheduledObject3DEvents;

}; // Scene :: Details



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // SCENE_DETAILS_H_6014714286
