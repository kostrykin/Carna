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

#ifndef OFFSCREENVISUALIZATIONENVIRONMENT_H_6014714286
#define OFFSCREENVISUALIZATIONENVIRONMENT_H_6014714286

/** \file   OffscreenVisualizationEnvironment.h
  * \brief  Defines \ref Carna::base::OffscreenVisualizationEnvironment.
  */

#include <Carna/base/VisualizationEnvironment.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// OffscreenVisualizationEnvironment
// ----------------------------------------------------------------------------------

/** \brief
  * Represents the visualization environment for offscreen rendering.
  *
  * \see    Refer to \ref OffscreenDisplay for details on offscreen rendering.
  *
  * The environment can operate both, providing an exclusive OpenGL context or sharing
  * the context with any other \ref VisualizationEnvironment, such as that of a
  * regular \ref qt::Display.
  *
  * \since  \ref v_2_1_3
  * \author Leonid Kostrykin
  * \date   29.4.13 - 22.5.13
  */
class CARNA_EXPORT OffscreenVisualizationEnvironment : public VisualizationEnvironment
{

public:
    
    /** \brief  Instantiates using an exclusive OpenGL context.
      */
    OffscreenVisualizationEnvironment();

    /** \brief  Instantiates using the OpenGL context from the supplied \a environment.
      */
    explicit OffscreenVisualizationEnvironment( VisualizationEnvironment& environment );

    /** \brief  Releases acquired resources.
      */
    virtual ~OffscreenVisualizationEnvironment();

 // ---------------------------------------------------------------------------------
 // Core Interface
 // ---------------------------------------------------------------------------------

    /** \brief  This method is currently unimplemented and returns \c nullptr.
      */
    virtual MenuBuilder* createMenu() override;

    /** \brief  Sets the value returned by subsequent calls to \ref viewCursor.
      */
    virtual void setViewCursor( Qt::CursorShape ) override;

    /** \brief  Returns the string supplied most lately by \ref pushTextPromptResult
      *         according to the FILO-principle (first-in-last-out) and removes it.
      */
    virtual QString promptText
        ( const QString& title
        , const QString& text
        , QLineEdit::EchoMode echoMode
        , const QString& value
        , bool& ok ) override;

    virtual void activateContext() const override;

    /** \brief  Currently not implemented.
      */
    virtual void paintText( const Vector& position, const std::string& text ) override;

 // ----------------------------------------------------------------------------------

    /** \brief  Returns the value supplied by \ref setViewCursor most lately.
      */
    Qt::CursorShape viewCursor() const;

    /** \brief  Sets the string to be returned by \ref promptText next.
      */
    void pushTextPromptResult( const std::string& );

    /** \brief  Removes the string supplied by \ref pushTextPromptResult most lately.
      */
    void popTextPromptResult();
    
    /** \brief  Returns the string supplied most lately by \ref pushTextPromptResult
      *         according to the FILO-principle (first-in-last-out) but leaves it on the stack.
      */
    const std::string& textPromptResult() const;

    /** \brief  Returns the number of strings currently supplied by previous \ref pushTextPromptResult calls.
      */
    unsigned int countTextPromptResults() const;

 // ----------------------------------------------------------------------------------

private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // OffscreenVisualizationEnvironment



}  // namespace Carna :: base

}  // namespace Carna

#endif // OFFSCREENVISUALIZATIONENVIRONMENT_H_6014714286
