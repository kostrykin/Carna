/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
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

#include <Carna/Carna.h>
#include <vector>

/** \file   Scene.h
  * \brief  Defines \ref Carna::base::view::Scene.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Scene
// ----------------------------------------------------------------------------------

class CARNA_LIB Scene
{

	std::vector< SceneProcessor* > myProcessors;
	Node* myRoot;

public:

	std::size_t processors() const;
	
	void appendProcessor( SceneProcessor* );
	
	void clearProcessors();

	void setRoot( Node& );
	
	Node& root();
	
	const Node& root() const;
	
	void render() const;

}; // Scene



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SCENE_H_6014714286
