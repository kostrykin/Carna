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

#ifndef VISUALIZATIONENVIRONMENT_H_6014714286
#define VISUALIZATIONENVIRONMENT_H_6014714286

/** \file   VisualizationEnvironment.h
  * \brief  Defines \ref Carna::base::VisualizationEnvironment.
  */

#include <Carna/Carna.h>
#include <Carna/base/Transformation.h>
#include <Carna/base/noncopyable.h>
#include <QLineEdit>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VisualizationEnvironment
// ----------------------------------------------------------------------------------
    
/** \brief
  * Provides interface that allows visualizations to interact with their environment.
  *
  * Such an environment may be a Qt based user interface as well as a testing environment.
  * The environment must have a unique GL context that can be activated via \ref activateContext.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13 - 19.3.13
  */
class CARNA_EXPORT VisualizationEnvironment
{

    NON_COPYABLE

public:

    /** \brief  Program menu abstraction interface
      *
      * \author Leonid Kostrykin
      * \date   5.3.13 - 19.3.13
      */
    class CARNA_EXPORT MenuBuilder
    {

        NON_COPYABLE

    public:

        /** \brief  Identifies a menu item.
          */
        typedef unsigned int ActionID;

        /** \brief  Indicates a non-existing menu item.
          */
        const static ActionID NULL_ACTION_ID;

        /** \brief  Does nothing.
          */
        virtual ~MenuBuilder()
        {
        }

        /** \brief  Adds new menu item and returns it's identifier.
          */
        virtual ActionID addAction
            ( const QString& text
            , QObject* receiver
            , const char* slot ) = 0;
        
        /** \brief  Adds new menu item and returns it's identifier.
          *
          * \post
          * <code>addAction(text) != NULL_ACTION_ID</code>
          */
        virtual ActionID addAction( const std::string& text ) = 0;

        /** \brief  Adds separating menu item.
          */
        virtual void addSeparator() = 0;

        /** \brief  Presents the menu and requests a decision upon one of it's items from the environment.
          *
          * Returns either \c %NULL_ACTION_ID if no item was selected, or the ID of the selected item.
          */
        virtual ActionID present
            ( int x
            , int y ) = 0;

    }; // VisualizationEnvironment::MenuBuilder


    /** \brief  Does nothing.
      */
    virtual ~VisualizationEnvironment()
    {
    }

    // ------------------------------------------------------------------------------
    // Core Interface
    // ------------------------------------------------------------------------------

    /** \brief  Creates a new menu and returns an interface for handling it.
      *
      * Example:
      *
      * \code
      * using Carna::base::VisualizationEnvironment::MenuBuilder;
      *
      * const std::unique_ptr< MenuBuilder > menu( environment.createMenu() );
      *
      * const MenuBuilder::ActionID item1 = menu->addAction( "Item 1" );
      * const MenuBuilder::ActionID item2 = menu->addAction( "Item 2" );
      *
      * const MenuBuilder::ActionID result = menu->present( x, y );
      *
      * if( result == item1 )
      * {
      *     // item 1 selected
      * }
      * else
      * if( result == item2 )
      * {
      *     // item 2 selected
      * }
      * \endcode
      */
    virtual MenuBuilder* createMenu() = 0;

    /** \brief  Configures the mouse cursor upon the environment.
      */
    virtual void setViewCursor( Qt::CursorShape ) = 0;

    /** \brief  Requests a text input from the environment.
      */
    virtual QString promptText
        ( const QString& title
        , const QString& text
        , QLineEdit::EchoMode echoMode
        , const QString& value
        , bool& ok ) = 0;

    /** \brief  Activates the GL context.
      */
    virtual void activateContext() const = 0;

    /** \brief
      * Paints the given \a text at the given \a position with respect to the currently set OpenGL matrices.
      *
      * \since \ref v_2_1
      */
    virtual void paintText( const Vector& position, const std::string& text ) = 0;

}; // VisualizationEnvironment



}  // namespace Carna :: base

}  // namespace Carna

#endif // VISUALIZATIONENVIRONMENT_H_6014714286
