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

#include <Carna/base/view/glew.h>
#include <Carna/base/OffscreenVisualizationEnvironment.h>
#include <Carna/base/CarnaException.h>
#include <QGLPixelBuffer>
#include <stack>

namespace Carna
{

namespace base
{

namespace OffscreenVisualizationEnvironmentDetails
{



// ----------------------------------------------------------------------------------
// OffscreenVisualizationEnvironmentDetails :: Context
// ----------------------------------------------------------------------------------

class Context
{

public:

    virtual ~Context()
    {
    }

    virtual void activate() = 0;

}; // OffscreenVisualizationEnvironmentDetails :: Context



// ----------------------------------------------------------------------------------
// OffscreenVisualizationEnvironmentDetails :: SharedContext
// ----------------------------------------------------------------------------------

class SharedContext : public Context
{

    VisualizationEnvironment& environment;

public:

    SharedContext( VisualizationEnvironment& environment );

    virtual void activate() override;

}; // OffscreenVisualizationEnvironmentDetails :: SharedContext


SharedContext::SharedContext( VisualizationEnvironment& environment )
    : environment( environment)
{
    this->activate();
}


void SharedContext::activate()
{
    environment.activateContext();
}



// ----------------------------------------------------------------------------------
// OffscreenVisualizationEnvironmentDetails :: ExclusiveContext
// ----------------------------------------------------------------------------------

class ExclusiveContext : public Context
{

    const std::unique_ptr< QGLPixelBuffer > pbuffer;

    const static QGLFormat format;

public:

    ExclusiveContext();

    virtual void activate() override;

}; // OffscreenVisualizationEnvironmentDetails :: ExclusiveContext


const QGLFormat ExclusiveContext::format = []()->QGLFormat
    {
        QGLFormat format = QGLFormat::defaultFormat();
        format.setDoubleBuffer( false );
        return format;
    }
();


ExclusiveContext::ExclusiveContext()
    : pbuffer( new QGLPixelBuffer( 8, 8, format ) )
{
    this->activate();

    CARNA_GLEW_INIT;
}


void ExclusiveContext::activate()
{
    pbuffer->makeCurrent();
}



}  // namespace OffscreenVisualizationEnvironmentDetails



// ----------------------------------------------------------------------------------
// OffscreenVisualizationEnvironment :: Details
// ----------------------------------------------------------------------------------

class OffscreenVisualizationEnvironment::Details
{

public:

    Details( OffscreenVisualizationEnvironmentDetails::Context* );


    const std::unique_ptr< OffscreenVisualizationEnvironmentDetails::Context > context;

    Qt::CursorShape cursor;

    std::stack< std::string > textPromptResults;

}; // OffscreenVisualizationEnvironment :: Details


OffscreenVisualizationEnvironment::Details::Details( OffscreenVisualizationEnvironmentDetails::Context* context )
    : context( context )
    , cursor( Qt::ArrowCursor )
{
}



// ----------------------------------------------------------------------------------
// OffscreenVisualizationEnvironment
// ----------------------------------------------------------------------------------

OffscreenVisualizationEnvironment::OffscreenVisualizationEnvironment()
    : pimpl( new Details( new OffscreenVisualizationEnvironmentDetails::ExclusiveContext() ) )
{
}


OffscreenVisualizationEnvironment::OffscreenVisualizationEnvironment( VisualizationEnvironment& environment )
    : pimpl( new Details( new OffscreenVisualizationEnvironmentDetails::SharedContext( environment ) ) )
{
}


OffscreenVisualizationEnvironment::~OffscreenVisualizationEnvironment()
{
}


VisualizationEnvironment::MenuBuilder* OffscreenVisualizationEnvironment::createMenu()
{
    /* Not supported yet.
     */

    return nullptr;
}


void OffscreenVisualizationEnvironment::setViewCursor( Qt::CursorShape cursor )
{
    pimpl->cursor = cursor;
}


QString OffscreenVisualizationEnvironment::promptText
    ( const QString& title
    , const QString& text
    , QLineEdit::EchoMode echoMode
    , const QString& defaultValue
    , bool& ok )
{
    CARNA_ASSERT( countTextPromptResults() > 0 );

    QString result = QString::fromStdString( textPromptResult() );
    popTextPromptResult();

    return result;
}


void OffscreenVisualizationEnvironment::activateContext() const
{
    pimpl->context->activate();
}


void OffscreenVisualizationEnvironment::paintText( const Vector& position, const std::string& text )
{
    /* Not supported yet.
     */
}


Qt::CursorShape OffscreenVisualizationEnvironment::viewCursor() const
{
    return pimpl->cursor;
}


void OffscreenVisualizationEnvironment::pushTextPromptResult( const std::string& textPromptResult )
{
    pimpl->textPromptResults.push( textPromptResult );
}


void OffscreenVisualizationEnvironment::popTextPromptResult()
{
    pimpl->textPromptResults.pop();
}


const std::string& OffscreenVisualizationEnvironment::textPromptResult() const
{
    return pimpl->textPromptResults.top();
}


unsigned int OffscreenVisualizationEnvironment::countTextPromptResults() const
{
    return pimpl->textPromptResults.size();
}



}  // namespace Carna :: base

}  // namespace Carna
