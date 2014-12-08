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

#include <Carna/base/Parallelization.h>
#include <QDebug>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ParallelizationUI
// ----------------------------------------------------------------------------------

ParallelizationUI::ParallelizationUI
    ( const QString& hint
    , int min
    , int max
    , QWidget* parent )

    : totalProgress( 0 )
    , progressDlg( hint
    , "Abort"
    , min
    , max
    , parent )
{
    connect( &progressDlg, SIGNAL( canceled() ), this, SLOT( cancel() ) );

    progressDlg.setAutoReset( false );
    progressDlg.setWindowModality( Qt::WindowModal );
    progressDlg.setWindowTitle( QApplication::applicationName() );
}


void ParallelizationUI::exec()
{
    progressDlg.exec();
}


bool ParallelizationUI::wasCanceled() const
{
    return progressDlg.wasCanceled();
}


void ParallelizationUI::progress( int amount )
{
    totalProgress += amount;

    progressDlg.setValue( totalProgress );
    progressDlg.update();
}


void ParallelizationUI::cancel()
{
    emit canceled();
}


void ParallelizationUI::dismiss()
{
    progressDlg.accept();
}



}  // namespace Carna :: base :: qt



// ----------------------------------------------------------------------------------
// IncrementalCounter
// ----------------------------------------------------------------------------------

IncrementalCounter::IncrementalCounter( unsigned int threshold )
    : threshold( threshold )
    , current( 0 )
{
    CARNA_ASSERT( threshold >= 1 );
}


void IncrementalCounter::increment()
{
    ++current;

    qDebug() << "IncrementalCounter::increment:" << current << "/" << threshold;

    if( current == threshold )
    {
        emit thresholdReached();

        current = 0;
    }
}



}  // namespace Carna :: base

}  // namespace Carna
