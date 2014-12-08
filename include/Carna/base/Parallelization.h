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

#ifndef PARALLELIZATION_H_6014714286
#define PARALLELIZATION_H_6014714286

/** \file   Parallelization.h
  *
  * \brief  Defines \ref Carna::base::Parallelization,
  *                 \ref Carna::base::qt::ParallelizationUI
  */

#include <string>
#include <QThread>
#include <QApplication>
#include <Carna/base/ParallelizationSettings.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/qt/CarnaProgressDialog.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ParallelizationUI
// ----------------------------------------------------------------------------------

/** \brief  \ref Parallelization to \ref CarnaProgressDialog adapter.
  *
  * \ingroup ParallelizationTools
  *
  * \see Refer to the \ref ParallelizationExample "Parallelization Example".
  *
  * \author Leonid Kostrykin
  * \date   2.8.11 - 3.5.13
  */
class CARNA_EXPORT ParallelizationUI : public QObject
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      *
      * Lets assume that \f$ J = \{ s_1, s_2, \dots, s_n \}\f$ is the job which we
      * want to perform parallelized, with \f$ s_i \f$ as discrete steps, i.e.
      * iterations. Lets further assume that we want to use \f$ m \f$ threads, which
      * means that each thread will be performing \f$ \frac{ n }{ m } \f$ steps
      * in average. Every time a certain thread has completed \f$ k \f$ steps, it will
      * notify the \c %ParallelizationUI instance by invoking ParallelizationUI::progress
      * with \f$ k \f$ as argument. The instantiation we would require would take
      * \f$ 1 \f$ for \a min and \f$ n \f$ for \a max.
      */
    ParallelizationUI
        ( const QString& hint
        , int min
        , int max
        , QWidget* parent = nullptr );


    /** \brief  Returns whether user did hit the abort button.
      */
    bool wasCanceled() const;


public slots:

    /** \brief  Updates the logical state's current progress.
      *
      * Given \a amount must not exceed the \a min / \a max pair which had been told
      * to the constructor.
      */
    void progress( int amount );

    /** \brief
      * Presents the dialog and runs an event loop.
      * Returns not before either the user aborts the job or the progressed work amount reaches the set maximum.
      *
      * \since \ref v_2_1_4
      */
    void exec();

    /** \brief  Indicates that the processing has ended successfully.
      *
      * Closes the dialog and \ref exec "returns from event loop".
      *
      * \since \ref v_2_1_4
      */
    void dismiss();


signals:

    /** \brief  Emitted when the user has requested to cancel the computation.
      */
    void canceled();


private slots:

    void cancel();


private:

    /** \brief  Holds the logical state's current progress.
      */
    int totalProgress;

    /** \brief  Holds the visual state.
      */
    qt::CarnaProgressDialog progressDlg;

}; // ParallelizationUI



}  // namespace Carna :: base :: qt



// ----------------------------------------------------------------------------------
// IncrementalCounter
// ----------------------------------------------------------------------------------

/** \brief  Provides cyclic incremental counter that can be controlled by signals and slots.
  *
  * \since  \ref v_2_1_4
  *
  * \ingroup ParallelizationTools
  *
  * The counter is initialized with 0 and any maximum value \em threshold \f$\geq 1\f$.
  * The value is incremented by invoking \ref increment.
  * As soon as the value reaches the preset \em threshold,
  * it is reset to 0 and the \ref thresholdReached signal is emitted.
  *
  * \author Leonid Kostrykin
  * \date   2.5.13 - 8.5.13
  */
class CARNA_EXPORT IncrementalCounter : public QObject
{

    Q_OBJECT

public:

    /** \brief  Instantiates with \a threshold and value 0.
      */
    IncrementalCounter( unsigned int threshold );


public slots:

    /** \brief  Increments the current value.
      *
      * If the value reaches the preset threshold,
      * it is reset to 0 and the \ref thresholdReached signal is emitted.
      */
    void increment();


signals:

    /** \brief  Emitted by \ref increment when the preset threshold was reached.
      */
    void thresholdReached();


private:

    const unsigned int threshold;

    unsigned int current;

};



// ----------------------------------------------------------------------------------
// Parallelization
// ----------------------------------------------------------------------------------

/** \brief
  * Performs a parallel job execution using specified worker class and user interface.
  *
  * \ingroup ParallelizationTools
  *
  * \see Refer to the \ref ParallelizationExample "Parallelization Example".
  *
  * The second template argument is optional. It defines the class that is instantiated
  * for user interaction purposes, such as reporting progress or getting notified when
  * the execution is aborted. There are several requirements upon the provided class'
  * public interface:
  *
  * - The method \c wasCanceled must tell whether the execution has been canceled by the
  *   user (\c true) or not (\c false).
  * - The slot \c progress must take an amount (<code>int</code>) of accomplished work.
  * - Invocation of the slot \c exec must not return until the execution is finished.
  * - The slot \c dismiss must successfully finish the execution prematurely.
  * - The signal \c canceled must be emitted when the execution is to be aborted.
  *
  * Furthermore the supplied class must provide a constructor, that takes the following arguments:
  *
  * -# <code>const QString& hint</code> describes the job being done to the user.
  * -# <code>int min</code> describes the amount of work already done initially. Ususually this is 0.
  * -# <code>int max</code> describes the amount of work that is to be done in order to finish the execution successfully.
  * -# <code>QWidget* parent = nullptr</code> is the modal parent, if modality is supported.
  *
  * \author Leonid Kostrykin
  * \date   2.8.11 - 22.5.13
  */
template< typename Worker, typename UI = qt::ParallelizationUI >
class Parallelization : public noncopyable
{

public:

    /** \brief  Holds the value of the generic argument.
      */
    typedef Worker Worker;

 // ----------------------------------------------------------------------------------

    /** \brief  Instantiates with some explicit workers set.
      */
    template< typename WorkerIterator >
    Parallelization( const WorkerIterator& begin, const WorkerIterator& end );

    /** \brief  Instantiates with an empty workers set.
      */
    Parallelization();

    /** \brief  Releases all workers.
      */
    ~Parallelization();

 // ----------------------------------------------------------------------------------

    /** \brief  Extends the workers set.
      *
      * Taking \a worker ownership.
      */
    void putWorker( Worker* const worker );

    /** \brief  References some certain worker from the workers set.
      */
    Worker& workerByIndex( unsigned int index );

    /** \brief  References some certain worker from the workers set.
      */
    const Worker& workerByIndex( unsigned int index ) const;

    /** \brief  Tells how much workers are configured to be used.
      */
    unsigned int countWorkers() const;

 // ----------------------------------------------------------------------------------

    /** \brief  Performs the parallel job execution.
      *
      * The method returns as soon as the job is finished (\c true) or canceled (\c false).
      */
    bool run
        ( const std::string& hint
        , QThread::Priority priority
        , QWidget* parent = nullptr );

 // ----------------------------------------------------------------------------------

private:

    /** \brief  Holds the workers which are to be used for job performance.
      */
    std::vector< Worker* > workers;

}; // Parallelization


template< typename Worker, typename UI >
template< typename WorkerIterator >
Parallelization< Worker, UI >::Parallelization( const WorkerIterator& begin, const WorkerIterator& end )
{
    for( WorkerIterator it = begin; it != end; ++i )
    {
        workers.push_back( *it );
    }
}


template< typename Worker, typename UI >
Parallelization< Worker, UI >::Parallelization()
{
}


template< typename Worker, typename UI >
Parallelization< Worker, UI >::~Parallelization()
{
    for( unsigned int i = 0; i < workers.size(); ++i )
    {
        delete workers[ i ];
    }
}


template< typename Worker, typename UI >
void Parallelization< Worker, UI >::putWorker( Worker* const worker )
{
    workers.push_back( worker );
}


template< typename Worker, typename UI >
Worker& Parallelization< Worker, UI >::workerByIndex( unsigned int index )
{
    return *workers[ index ];
}


template< typename Worker, typename UI >
const Worker& Parallelization< Worker, UI >::workerByIndex( unsigned int index ) const
{
    return *workers[ index ];
}


template< typename Worker, typename UI >
bool Parallelization< Worker, UI >::run
    ( const std::string& hint
    , QThread::Priority priority
    , QWidget* parent )
{
    int work_amount = 0;
    for( auto it  = workers.begin(); it != workers.end(); ++it )
    {
        work_amount += ( **it ).getWorkAmount();
    }

    UI ui( QString::fromStdString( hint ), 0, work_amount, parent );

    QApplication::processEvents();

 // prepare progress tracking

    for( auto it  = workers.begin(); it != workers.end(); ++it )
    {
        QObject::connect
            ( *it
            , SIGNAL( progressed( int ) )
            , &ui
            , SLOT( progress( int ) ) );
    }

 // create threads

    std::vector< QThread* > threads;
    IncrementalCounter finishedThreadsCounter( workers.size() );
    for( auto it = workers.begin(); it != workers.end(); ++it )
    {
        QThread* const thread = new QThread();

        Worker* const worker = *it;
        worker->moveToThread( thread );

        QObject::connect( worker, SIGNAL( finished() ), thread, SLOT( quit() ) );
        QObject::connect( &ui   , SIGNAL( canceled() ), worker, SLOT( cancel() ) );
        QObject::connect( thread, SIGNAL( finished() ), thread, SLOT( deleteLater() ) );
        QObject::connect( worker, SIGNAL( finished() ), &finishedThreadsCounter, SLOT( increment() ) );

        thread->start( priority );

        threads.push_back( thread );
    }

    QObject::connect( &finishedThreadsCounter, SIGNAL( thresholdReached() ), &ui, SLOT( dismiss() ) );

 // run workers

    for( auto it = workers.begin(); it != workers.end(); ++it )
    {
        QMetaObject::invokeMethod( *it, "run", Qt::QueuedConnection );
    }

 // barrier

    /* The 'exec' method returns as soon as the user either has clicked the abort button,
     * the progressed amount has reached the set maximum, or the 'finishedThreadsCounter'
     * has reached the configured limit.
     */

    ui.exec();

 // finish

    return !ui.wasCanceled();
}


template< typename Worker, typename UI >
unsigned int Parallelization< Worker, UI >::countWorkers() const
{
    return workers.size();
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // PARALLELIZATION_H_6014714286
