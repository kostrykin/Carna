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

#include <Carna/base/Releasable.h>
#include <Carna/base/GLContext.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Releasable :: Details
// ----------------------------------------------------------------------------------

struct Releasable::Details : public GLContext::OnGLShutdownListener
{
    Releasable& self;
    Details( Releasable& self );
    virtual void onGLShutdown( const GLContext& lastGLContext ) override;
};


Releasable::Details::Details( Releasable& self )
    : self( self )
{
}


void Releasable::Details::onGLShutdown( const GLContext& lastGLContext )
{
    self.release();
}



// ----------------------------------------------------------------------------------
// Releasable
// ----------------------------------------------------------------------------------

Releasable::Releasable()
    : pimpl( new Details( *this ) )
{
    GLContext::addOnGLShutdownListener( *pimpl );
}


Releasable::~Releasable()
{
    GLContext::removeOnGLShutdownListener( *pimpl );
}



}  // namespace Carna :: base

}  // namespace Carna
