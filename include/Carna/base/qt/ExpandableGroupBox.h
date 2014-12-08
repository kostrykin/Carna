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

#ifndef EXPANDABLEGROUPBOX_H_6014714286
#define EXPANDABLEGROUPBOX_H_6014714286

/** \file   ExpandableGroupBox.h
  * \brief  Defines \ref Carna::base::qt::ExpandableGroupBox.
  */

#include <Carna/Carna.h>
#include <QFrame>
#include <memory>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ExpandableGroupBox
// ----------------------------------------------------------------------------------

/** \brief  Widget container that is capable of hiding it's contents in order to save UI place.
  *
  * \image html ExpandableGroupBox01.png
  *
  * \ingroup    Widgets
  *
  * The child, used by \c %ExpandableGroupBox by default, is provided with a \c QVBoxLayout.
  *
  * Usage example:
  * \code
  * ExpandableGroupBox* const gb_options = new ExpandableGroupBox( "Options" );
  * gb_options->child()->layout()->addWidget( some_widget1 );
  * gb_options->child()->layout()->addWidget( some_widget2 );
  * \endcode
  *
  * Another example:
  * \code
  * ExpandableGroupBox* const gb_options = new ExpandableGroupBox( "Options" );
  * QFormLayout* const options = new QFormLayout();
  * gb_options->child()->setLayout( options );
  * gb_options->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
  * options->addRow( "Option 1", some_widget1 );
  * options->addRow( "Option 2", some_widget2 );
  * \endcode
  *
  * \author Leonid Kostrykin
  * \date   12.9.12
  */
class CARNA_EXPORT ExpandableGroupBox : public QFrame
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      *
      * The argument \a expended specifies whether the group box is initially expended or not (collapsed).
      */
    explicit ExpandableGroupBox( const QString& text = "", bool expended = false, QWidget* parent = nullptr );

    /** \brief  Releases acquired resources.
      */
    virtual ~ExpandableGroupBox();


    /** \brief  Tells the group box' current label text.
      *
      * \see    \ref setText
      */
    const QString& text() const;

    /** \brief  Tells whether the group box is currently expended.
      *
      * \see    \ref isCollapsed, \ref expend, \ref collapse
      */
    bool isExpended() const;
    
    /** \brief  Tells whether the group box is currently collapsed (not expended).
      *
      * \see    \ref isExpended, \ref expend, \ref collapse
      */
    bool isCollapsed() const;

    /** \brief  Removes this group box' contents.
      *
      * The ownership of the \c QWidget representing the contents is transfered to the caller of this method,
      * so you will have to either put the returned widget in some other layout or release it's memory:
      * 
      * \code
      * ExpandableGroupBox* const my_group_box;
      *
      * // ...
      *
      * delete my_group_box->takeChild();
      * \endcode
      *
      * The returned value is \c nullptr if the group box does not have a child.
      *
      * \see    \ref child, \ref setChild
      */
    QWidget* takeChild();

    /** \brief  References the contents of this group box.
      *
      * \see    \ref setChild, \ref takeChild
      */
    QWidget* child() const;


signals:

    /** \brief  The group box has been expended.
      *
      * \see    \ref expend
      */
    void expended();
    
    /** \brief  The group box has been collapsed.
      *
      * \see    \ref collapse
      */
    void collapsed();


public slots:

    /** \brief  Sets the group box' label text.
      *
      * \see    \ref text
      */
    void setText( const QString& );

    /** \brief  Expends or collapses the group box.
      *
      * Emits either the \ref expended or the \ref collapsed signal.
      *
      * \see    \ref expend, \ref collapse, \ref toggle, \ref isExpended, \ref isCollapsed
      */
    void setExpended( bool );

    /** \brief  Expends the group box.
      *
      * If the group box is expended already, nothing happens.
      *
      * Emits the \ref expended signal.
      *
      * \see    \ref isExpended, \ref setExpended, \ref collapse, \ref toggle
      */
    void expend();
    
    /** \brief  Collapses the group box.
      *
      * If the group box is collapsed already, nothing happens.
      *
      * Emits the \ref collapsed signal.
      *
      * \see    \ref isCollapsed, \ref setExpended, \ref expend, \ref toggle
      */
    void collapse();

    /** \brief  Switches the group box from 'collapsed' into 'expended' state and vice-versa.
      *
      * Emits either the \ref expended or the \ref collapsed signal.
      */
    void toggle();

    /** \brief  Sets this group box' child.
      *
      * If the group box already has a child, it will be released first.
      *
      * \see    \ref takeChild, \ref child
      */
    void setChild( QWidget* );


private:

    class Details;
    std::unique_ptr< Details > pimpl;


private slots:

    void toggle( const QString& );

}; // ExpandableGroupBox



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // EXPANDABLEGROUPBOX_H_6014714286
