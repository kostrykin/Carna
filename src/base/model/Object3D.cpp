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

#include <Carna/base/model/Object3D.h>
#include <Carna/base/model/Object3DDetails.h>
#include <Carna/base/model/Object3DEvent.h>
#include <Carna/base/view/gl.h>
#include <Carna/base/CarnaException.h>
#include <sstream>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// createNullIcon
// ----------------------------------------------------------------------------------

static QIcon createNullIcon()
{
    QPixmap pixmap( 10, 10 );
    pixmap.fill( qRgba( 0, 0, 0, 0 ) );
    return QIcon();
}



// ----------------------------------------------------------------------------------
// Object3D :: Details
// ----------------------------------------------------------------------------------

Object3D::Details::Details( Scene& model, const std::string& name )
    : position( model )
    , name( name )
{
}


Object3D::Details::Details( const Position& position, const std::string& name )
    : position( position )
    , name( name )
{
}



// ----------------------------------------------------------------------------------
// Object3D
// ----------------------------------------------------------------------------------

Object3D::Object3D( Scene& model, const std::string& name )
    : Scene::BaseObject3D( model )
    , pimpl( new Details( model, name ) )
{
    CARNA_ASSERT( this->name() == name );
    CARNA_ASSERT( this->position().toVolumeUnits() == Vector( 0, 0, 0 ) );
}


Object3D::Object3D( Scene& model, const Position& position, const std::string& name )
    : Scene::BaseObject3D( model )
    , pimpl( new Details( position, name ) )
{
    CARNA_ASSERT( this->name() == name );
    CARNA_ASSERT( this->position() == position );
}


Object3D::~Object3D()
{
    emit destroyed( *this );
}


std::string Object3D::createDefaultName()
{
    static unsigned int nextInstanceNumber = 1;

    std::stringstream ss;
    ss << "object #" << ( nextInstanceNumber++ );
    return ss.str();
}


void Object3D::paintFalseColor( const view::Renderer& renderer, const QColor& color ) const
{
    paintFalseColor( renderer, Vector3ui( color.red(), color.green(), color.blue() ) );
}


const QIcon& Object3D::icon() const
{
    const static QIcon nullIcon = createNullIcon();

    return nullIcon;
}


const Position& Object3D::position() const
{
    return pimpl->position;
}


void Object3D::setPosition( const Position& pos )
{
    pimpl->position = pos;

    invalidateObjects3D( Object3DEvent( Object3DEvent::position ) );

    emit moved();
}


void Object3D::move( const Position& pos_delta )
{
    setPosition( pimpl->position + pos_delta );
}


const Transformation& Object3D::rotation() const
{
    static Transformation nullTransformation;

    return nullTransformation;
}


void Object3D::setRotation( const Transformation& )
{
}


void Object3D::setName( const std::string& newName )
{
    pimpl->name = newName;

    invalidateObjects3D( Object3DEvent( Object3DEvent::name ) );

    emit renamed();
    emit renamed( pimpl->name );
    emit renamed( QString::fromStdString( pimpl->name ) );

    CARNA_ASSERT( name() == newName );
}


const std::string& Object3D::name() const
{
    return pimpl->name;
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
