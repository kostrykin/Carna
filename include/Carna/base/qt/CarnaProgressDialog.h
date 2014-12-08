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

#ifndef CARNAPROGRESSDIALOG_H_6014714286
#define CARNAPROGRESSDIALOG_H_6014714286

/** \file   CarnaProgressDialog.h
  * \brief  Defines \ref Carna::base::qt::CarnaProgressDialog.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QProgressDialog>
#include <QApplication>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// CarnaProgressDialog
// ----------------------------------------------------------------------------------

/** \brief  \c QProgressDialog specialization to the application's purposes.
  *
  * QProgressDialog specialization which provides recursion protection along with
  * two additional slots:
  *
  * - \ref setMaximum(unsigned int)
  * - \ref setValue(unsigned int)
  *
  * \author Leonid Kostrykin
  * \date   2010 - 2012
  */
class CARNA_EXPORT CarnaProgressDialog : public QProgressDialog
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    CarnaProgressDialog( const QString& labelText, const QString& cancelButtonText, int minimum, int maximum, QWidget* parent = nullptr );


public slots:

    /** \brief  Overloaded function
      *
      * \see    \c QProgressDialog::setMaxiumum( int )
      */
    void setMaximum( unsigned int maximum )
    {
        QProgressDialog::setMaximum( static_cast<signed int>( maximum ) );
    }

    /** \copydoc setValue(int)
      */
    void setValue( unsigned int value )
    {
        setValue( static_cast< int >( value ) );
    }

    /** \brief  Overloaded function
      *
      * Invokes \c QApplication::processEvents() after updating the value.
      *
      * \see    \c QProgressDialog::setValue( int )
      */
    void setValue( int value )
    {

     // We do require a recursion protection. Unwanted recursion may occur,
     // when an event is posted during the execution of this method and then
     // QApplication::processEvents is called, which again invokes this method:

        if( recursionLevel > 0 )
        {
            return;
        }

        ++recursionLevel;

        QProgressDialog::setValue( value );

      --recursionLevel;

    }


private:

    /** \brief  Used for suppression of unwanted recursion.
      *
      * \see    CarnaProgressDialog::setValue inline documentation
      */
    unsigned int recursionLevel;

}; // CarnaProgressDialog



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // CARNAPROGRESSDIALOG_H_6014714286
