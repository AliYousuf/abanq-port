/***************************************************************************
             mlabelobject.h  -  Kugar report label object
             -------------------
   begin     : Wed Aug 11 1999
   copyright : (C) 1999 by Mutiny Bay Software
   email     : info@mutinybaysoftware.com
   copyright : (C) 2002-2003 InfoSiAL S.L.
   email     : mail@infosial.com
***************************************************************************/

#ifndef MLABELOBJECT_H
#define MLABELOBJECT_H

#include <qstring.h>
#include <qfontmetrics.h>
#include <qpixmap.h>
#include <qdom.h>

#include <qsargument.h>
#include <FLApplication.h>

#include "mreportobject.h"

/**
Kugar report label object

@author Mutiny Bay Software
*/

class MLabelObject: public MReportObject {

public:

  /**
  Font weight constants
  */
  enum FontWeight {
    Light = 25,
    Normal = 50,
    DemiBold = 63,
    Bold = 75,
    Black = 87
  };

  /**
  Horizontal alignment constants
  */
  enum HAlignment {
    Left = 0,
    Center,
    Right
  };

  /**
  Vertial alignment constants
  */
  enum VAlignment {
    Top = 0,
    Middle,
    Bottom
  };

  /**
  Constructor
  */
  MLabelObject();

  /**
  Copy constructor
  */

  MLabelObject( const MLabelObject & mLabelObject );

  /**
  Assignment operator
  */
  MLabelObject operator=( const MLabelObject & mLabelObject );

  /**
  Destructor
  */
  virtual ~ MLabelObject();

protected:

  /**
  Label text
  */
  QString text;

  /**
  Label pixmap
  */
  QPixmap * pixmap;

  /**
  Label text font family
  */
  QString fontFamily;

  /**
  Label text font size in points
  */
  float fontSize;

  /**
  Label text font weight
  */
  int fontWeight;

  /**
  Label text font italic flag
  */
  bool fontItalic;

  /**
  Lable text horizontal alignment
  */
  int hAlignment;

  /**
  Label text vertical alignment
  */
  int vAlignment;

  /**
  Label text word wrap flag
  */
  bool wordWrap;

  /**
  Label function
  */
  QString labelFunction;

  /**
  Indicates whether the label's height (and its including section if necessary) can change or not, if the text is too long and the wordWrap flag is set
  */
  bool changeHeight;

  /**
  Paint function
  */
  QString paintFunction;

  /**
  DOM Node with auxiliar data.

  If the DOM Node is not null, it is passed on to the script functions 'labelFunction'
  and 'paintFunction' when this functions are defined
  */
  QDomNode domNodeData;

private:

  /**
  Label text horizontal margin spacing
  */
  const int xMargin;

  /**
  Label text vertical margin spacing
  */
  const int yMargin;

public:

  /**
  Sets the label's text string - default is an empty string
  */
  virtual void setText( const QString txt );

  /**
  Gets the label's text string - default is an empty string
  */
  virtual QString getText();

  /** Gets the label's text string */
  bool getChangeHeight();

  /**
  Sets the label's pixmap
  */
  virtual void setPixmap( const QPixmap & pix );

  /**
  Sets the label's text font - default is Times,10,Normal,false
  */
  void setFont( const QString family, float size, int weight, bool italic );

  /**
  Sets the label's horizontal alignment -default is Left
  */
  void setHorizontalAlignment( int a );

  /**
  Sets the label's vertical alignment - default is Top
  */
  void setVerticalAlignment( int a );

  /**
  Sets the label's word wrap flag - default is false
  */
  void setWordWrap( bool state );

  /**
  Sets the label's change height flag
  */
  void setChangeHeight( bool state );

  /**
  Sets the label's function
  */
  void setLabelFunction( const QString & lF );

  /**
  Sets paint function
  */
  void setPaintFunction( const QString & pF );

  /**
  Sets the data
  */
  virtual void setDomNodeData( const QDomNode & nD );

  /**
  Draws the label using the specificed painter & x/y-offsets
  */
  int draw( FLStylePainter * p );

private:

  /**
  Copies member data from one object to another.

  Used by the copy constructor and assignment operator
  */
  void copy( const MLabelObject * mLabelObject );
};

#endif
