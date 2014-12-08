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

#ifndef VISUALIZATION_H_6014714286
#define VISUALIZATION_H_6014714286

/** \file   Visualization.h
  * \brief  Defines \ref Carna::base::Visualization.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QObject>
#include <functional>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Visualization
// ----------------------------------------------------------------------------------

/** \brief
  * Combines one view (\ref view::Renderer "Renderer") and one controller (\ref controller::InteractionStrategy "InteractionStrategy") object through composition.
  *
  * When a visualization is instantiated, no renderer is assigned initially. The
  * creation of the renderer is performed later, when the \ref initialize method is
  * invoked. Any given visualization can only be initialized once.
  *
  * Hence, it is illegal to try to access the \ref renderer before the initialization
  * is complete: you may query the current state via \ref isInitialized. Nevertheless,
  * as the user of a particular visualization, it is often necessary to access the
  * renderer in order to set up it's initial state. For this purpose you should take
  * advantage of the \ref doAfterInitialization method: it takes any void-function as
  * an argument and invokes it as soon as the initialization is finished.
  *
  * \see
  * Refer to the \ref MinimalExample for an example on \ref doAfterInitialization usage.
  *
  * The controller's purpose is to react to the events it receives. In order to modify
  * the controller's behavior dynamically, especially to override or extend its behavior
  * for a limited time span, it is possible to install controller filters. One
  * visualization's filters are organized in a chain: whenever an event is received, it
  * is passed on from the filter that was added latest to the one that was added first.
  * If any of the filters accepts the event, the chain is no longer processed. If the
  * event reaches the end of the chain in ''unaccepted'' state, it is passed to the
  * ''raw'' controller that has been set via \ref setController.
  * 
  * \image html VisualizationStates.png
  *
  * The above state diagram illustrates the legality and illegality of interface
  * operations based on the visualization's current state.
  *
  * In order to implement your own visualization, inherit it from this class and
  * implement the \ref createRenderer method: you may assume that the proper GL
  * context is active when the method is invoked.
  *
  * \author Leonid Kostrykin
  * \date   8.6.11 - 18.11.13
  */
class CARNA_EXPORT Visualization : public QObject
{

    NON_COPYABLE

    Q_OBJECT

public:

    /** \brief  Instantiates using a new \ref controller::DefaultInteractionStrategy instance.
      */
    Visualization();
    
    /** \brief  Instantiates using the \a initialController.
      */
    explicit Visualization( controller::InteractionStrategy* initialController );


    /** \brief  Destroys the renderer and the controller.
      *
      * \attention
      * Controller filters are not destroyed.
      */
    virtual ~Visualization();


    /** \brief
      * Defines the function interface that is accepted by \ref doAfterInitialization.
      */
    typedef std::function< void() > PostInitializationCallback;

    /** \brief
      * Invokes the given function as soon as it is legal to access the \ref renderer.
      *
      * Trying to access the \ref renderer before the initialization is complete is
      * illegal. Nevertheless, as the user of a particular visualization, it is often
      * necessary to access the renderer in order to set up it's initial state. The
      * usual approach is to Write this type of renderer setup code as an anonymous
      * function (called lambda in C++11) and provide it to this function. Its
      * implementation will take care of invoking the supplied code as soon as the
      * initialization is finished and it becomes legal to access the renderer.
      *
      * \see
      * Refer to the \ref MinimalExample for an example on \ref doAfterInitialization usage.
      *
      * The invocation of the supplied function will be done immediately when the
      * initialization already is finished.
      */
    void doAfterInitialization( const PostInitializationCallback& );


    /** \brief  Tells whether the initialization is finished.
      */
    bool isInitialized() const;
    
    /** \brief  Initializes this visualization.
      *
      * The abstract method \ref createRenderer is invoked internally.
      *
      * \pre
      * <code>isInitialized() == false</code>
      *
      * \post
      * <code>isInitialized() == true</code>
      *
      * Also invokes all functions that have been supplied to \ref doAfterInitialization priorly.
      */
    void initialize( view::SceneProvider&, VisualizationEnvironment& );

    /** \brief  References the mutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      */
    view::Renderer& renderer();

    /** \brief  References the immutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      */
    const view::Renderer& renderer() const;

    /** \brief  References the mutable decorated controller.
      *
      * If no decorations are supplied currently, the raw controller is referenced.
      */
    controller::InteractionStrategy& controller();

    /** \brief  References the immutable decorated controller.
      *
      * If no decorations are supplied currently, the raw controller is referenced.
      */
    const controller::InteractionStrategy& controller() const;

    /** \brief  References the mutable raw controller.
      */
    controller::InteractionStrategy& rawController();

    /** \brief  References the immutable raw decorated controller.
      */
    const controller::InteractionStrategy& rawController() const;

    /** \brief
      * References the mutable visualization environment that was provided \ref initialize "during initialization".
      *
      * \pre
      * <code>isInitialized() == true</code> or invoked from within \ref createRenderer
      */
    VisualizationEnvironment& environment();

    /** \brief
      * References the immutable visualization environment that was provided \ref initialize "during initialization".
      *
      * \pre
      * <code>isInitialized() == true</code> or invoked from within \ref createRenderer
      */
    const VisualizationEnvironment& environment() const;


    /** \brief  Sets the raw controller.
      *
      * \pre
      * <code>controller != nullptr</code>
      *
      * \post
      * <code>&rawController() == controller</code>
      *
      * Note that <code>&controller() == controller</code> isn't a post-condition,
      * since \ref controller references the \em filtered controller. For
      * implementation reasons, <code>&controller() == controller</code> is false,
      * even if no filters supplied:
      *
      * \post
      * <code>&controller() != controller</code>
      *
      * Emits the \ref controllerExchanged signals.
      */
    void setController( controller::InteractionStrategy* controller );

    /** \brief  Puts the given controller to the beginning of the controller filters chain.
      */
    void addControllerFilter( controller::InteractionStrategy& );
    
    /** \brief  Removes the first occurrence of the given controller from the controller filters chain.
      *
      * \pre    The given controller has been installed via \ref addControllerFilter earlier at least once.
      */
    void removeControllerFilter( controller::InteractionStrategy& );


protected:

    /** \brief  Returns a renderer that the visualization is to take ownership of.
      *
      * The GL context must have been activated properly before this method is called.
      */
    virtual view::Renderer* createRenderer( view::SceneProvider& ) = 0;


signals:

    /** \brief  Emitted by \ref setController when a new raw controller is set.
      */
    void controllerExchanged();
    
    /** \brief  Emitted by \ref setController when a new raw controller is set.
      */
    void controllerExchanged( Carna::base::controller::InteractionStrategy& newController );


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // Visualization



}  // namespace Carna :: base

}  // namespace Carna

#endif // VISUALIZATION_H_6014714286
