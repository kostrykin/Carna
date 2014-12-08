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

#ifndef VOLUMERENDERERSINGLEPASSMODE_H_6014714286
#define VOLUMERENDERERSINGLEPASSMODE_H_6014714286

/** \file   VolumeRendererSinglePassMode.h
  * \brief  Defines \ref Carna::VolumeRenderings::VolumeRendererSinglePassMode.
  */

#include <Carna/VolumeRenderings/VolumeRendererMode.h>
#include <Carna/base/view/ShaderProgram.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: VolumeRendererSinglePassMode
// ----------------------------------------------------------------------------------

/** \brief  Abstract implementation of VolumeRendererMode, suitable for all further
  *         implementations which do require only a single rendering pass for the
  *         volume evaluation with some given shader.
  *
  * \par Limitations
  *
  * In contrast to \ref VolumeRendererSinglePassMode::renderColor "renderColor",
  * for \ref VolumeRendererSinglePassMode::renderDepth "renderDepth" only a
  * null-implementation is provided.
  *
  * \par Usage
  *
  * When using this class, you must provide an implementation for
  * \ref VolumeRendererSinglePassMode::configureShader "configureShader".
  * 
  *
  * \author Leonid Kostrykin
  * \date   8.8.11 - 26.8.11
  */
class CARNA_EXPORT VolumeRendererSinglePassMode : public VolumeRendererMode
{

public:

    /** \brief  Instantiates.
      *
      * \param  shader_id   specifies the shader to use.
      * \param  name        names this mode.
      *
      */
    VolumeRendererSinglePassMode
        ( const std::string& shader_id
        , const std::string& name )

        : VolumeRendererMode( name )
        , shaderID( shader_id )
        , paintingObjects( true )
    {
    }

    /** \brief	Does nothing.
      */
    virtual ~VolumeRendererSinglePassMode()
    {
    }


    virtual void renderColor( RayMarching& ) override;

    /** \brief  Does nothing.
      *
      * \see    \ref VolumeRendererMode::renderDepth
      */
    virtual void renderDepth( RayMarching& ) override;

    /** \brief  Holds which shader is invoked when rendering pass.
      */
    const std::string shaderID;


    /** \brief	Sets whether Object3D instances are to be painted on top of the scene.
      */
    void setObjectPainting( bool paintObjects )
    {
        this->paintingObjects = paintObjects;
    }

    /** \brief	Tells whether Object3D instances are painted on top of the scene.
      */
    bool isPaintingObjects() const
    {
        return paintingObjects;
    }


protected:

    /** \brief  Supplies custom arguments to the shader.
      */
    virtual void configureShader( RayMarching&, base::view::ShaderProgram::Binding& ) = 0;


private:

    bool paintingObjects;

}; // VolumeRenderings :: VolumeRendererSinglePassMode



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // VOLUMERENDERERSINGLEPASSMODE_H_6014714286
