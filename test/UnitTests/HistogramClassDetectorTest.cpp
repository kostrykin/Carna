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

#include "HistogramClassDetectorTest.h"
#include <Carna/base/Histogram.h>
#include <Carna/base/HistogramClassDetector.h>
#include <QFile>
#include <QTextStream>
#include <QRegExp>



// ----------------------------------------------------------------------------------
// NullScalarField
// ----------------------------------------------------------------------------------

struct NullScalarField
{

    typedef Carna::base::Histogram< Carna::base::IndexedCounter< signed short >, NullScalarField, 1 > Histogram;

    signed short operator()( const Histogram::Vector& )
    {
        return 0;
    }

};  // NullScalarField



// ----------------------------------------------------------------------------------
// HistogramClassDetectorTest
// ----------------------------------------------------------------------------------


void HistogramClassDetectorTest::initTestCase()
{
    disableQDebug();
}


void HistogramClassDetectorTest::cleanupTestCase()
{
}


void HistogramClassDetectorTest::init()
{
}


void HistogramClassDetectorTest::cleanup()
{
}


void HistogramClassDetectorTest::test()
{
    try
    {
        QFETCH( QString, histogram_file_name );
        QFETCH( QString, expected_histogram_classes );

        QFile histogram_file( histogram_file_name );
        histogram_file.open( QIODevice::ReadOnly );
        QTextStream histogram_file_in( &histogram_file );

        NullScalarField field;
        NullScalarField::Histogram histogram( field, NullScalarField::Histogram::Vector( 1 ) );

     // read histogram data

        const static QRegExp regex( "([-]?[0-9]+)[^=]*= ([0-9]+)" );
        while( !histogram_file_in.atEnd() )
        {
            const QString line = histogram_file_in.readLine();
            const int pos = regex.indexIn( line );
            if( pos == -1 )
            {
                continue;
            }

            const QString huv_str = regex.cap( 1 );
            const QString frequency_str = regex.cap( 2 );

            const signed int huv = huv_str.toInt();
            const unsigned int frequency = frequency_str.toUInt();

            histogram.counter[ huv ] = frequency;
        }

        histogram_file.close();

     // test 'done' and 'reset' on initial state

        typedef Carna::base::HistogramClassDetector< NullScalarField::Histogram > HistogramClassDetector;
        HistogramClassDetector class_detector( histogram );

        QVERIFY( !class_detector.done() );

        class_detector.reset();

        QVERIFY( !class_detector.done() );

     // test 'result' on initial state

        try
        {
            class_detector.result();
            QFAIL( "HistogramClassDetector::result did not raise an AssertionFailure as expected." );
        }
        catch( const Carna::base::AssertionFailure& )
        {
        }

     // compute classes and test 'done' afterwards

        class_detector.compute( HistogramClassDetector::Configuration( -1024, 3071, 50u, 100u, 0.2 ) );

        QVERIFY( class_detector.done() );

     // test 'result'

        QString actual_histogram_classes;
        for( auto class_itr = class_detector.result().begin(); class_itr != class_detector.result().end(); ++class_itr )
        {
            if( !actual_histogram_classes.isEmpty() )
            {
                actual_histogram_classes += ", ";
            }

            actual_histogram_classes += "[" + QString::number( class_itr->first ) + "; " + QString::number( class_itr->last ) + "]";
            if( class_itr->typical )
            {
                actual_histogram_classes += " typical";
            }
        }

        QCOMPARE( actual_histogram_classes, expected_histogram_classes );

     // test 'reset' after 'compute'

        class_detector.reset();

        QVERIFY( !class_detector.done() );
    }
    catch( const Carna::base::AssertionFailure& ex )
    {
        QFAIL( ex.details.c_str() );
    }
    catch( const std::out_of_range& ex )
    {
        QFAIL( ex.what() );
    }
}


void HistogramClassDetectorTest::test_data()
{
    QTest::addColumn< QString >( "histogram_file_name" );
    QTest::addColumn< QString >( "expected_histogram_classes" );

 // data

    QTest::newRow( "07.04.18-07.04.34-DST-1.3.12.2.1107.5.1.4.60025" )
        << ":/histograms/07.04.18-07.04.34-DST-1.3.12.2.1107.5.1.4.60025"
        << "[-1024; -783] typical, [-782; -157], [-156; 713] typical, [714; 2968], [2969; 3071] typical";

    QTest::newRow( "DCMCDW-3057454334" )
        << ":/histograms/DCMCDW-3057454334"
        << "[-1024; -654] typical, [-653; -297], [-296; -21] typical, [-20; 375] typical, [376; 2635], [2636; 3071] typical";
}
