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

#include <Carna/base/qt/DisplayVisualizationEnvironment.h>
#include <Carna/base/qt/Display.h>
#include <Carna/base/CarnaException.h>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QPoint>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DefaultMenuBuilder
// ----------------------------------------------------------------------------------

class DefaultMenuBuilder : public VisualizationEnvironment::MenuBuilder
{

public:

    DefaultMenuBuilder( Display& visualization )
        : visualization( visualization )
        , menu( new QMenu( &visualization ) )
        , nextActionID( NULL_ACTION_ID + 1 )
    {
    }


    virtual ActionID addAction
        ( const QString& text
        , QObject* receiver
        , const char* slot ) override
    {
        QAction* const action = menu->addAction( text, receiver, slot );
        actionIdByAction[ action ] = nextActionID;
        return nextActionID++;
    }

    virtual ActionID addAction( const std::string& text ) override
    {
        QAction* const action = new QAction( QString::fromStdString( text ), &visualization );
        menu->addAction( action );
        actionIdByAction[ action ] = nextActionID;

        const ActionID result = nextActionID++;

        CARNA_ASSERT( result != NULL_ACTION_ID );

        return result;
    }

    virtual void addSeparator() override
    {
        menu->addSeparator();
    }

    virtual ActionID present
        ( int x
        , int y ) override
    {
        QAction* const result = menu->exec( visualization.mapToGlobal( QPoint( x, y ) ) );
        if( result == nullptr )
        {
            return NULL_ACTION_ID;
        }
        else
        {
            return actionIdByAction[ result ];
        }
    }


private:

    Display& visualization;
    QMenu* const menu;

    ActionID nextActionID;
    std::map< QAction*, ActionID > actionIdByAction;

}; // DefaultMenuBuilder



// ----------------------------------------------------------------------------------
// DisplayVisualizationEnvironment
// ----------------------------------------------------------------------------------

DisplayVisualizationEnvironment::DisplayVisualizationEnvironment( Display& display )
    : display( display )
{
}


VisualizationEnvironment::MenuBuilder* DisplayVisualizationEnvironment::createMenu()
{
    return new DefaultMenuBuilder( display );
}


void DisplayVisualizationEnvironment::setViewCursor( Qt::CursorShape cursor )
{
    display.setCursor( cursor );
}


QString DisplayVisualizationEnvironment::promptText
    ( const QString& title
    , const QString& text
    , QLineEdit::EchoMode echoMode
    , const QString& defaultValue
    , bool& ok )
{
    return QInputDialog::getText
        ( &display
        , title
        , text
        , echoMode
        , defaultValue
        , &ok );
}


void DisplayVisualizationEnvironment::activateContext() const
{
    display.makeCurrent();
}


void DisplayVisualizationEnvironment::paintText( const Vector& position, const std::string& text )
{
    display.renderText
        ( position.x()
        , position.y()
        , position.z()
        , QString::fromStdString( text ) );
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
