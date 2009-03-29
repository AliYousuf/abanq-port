/***************************************************************************
                         FLScriptEditor.cpp
                         -------------------
begin                : mie mar 3 2004
copyright            : (C) 2004-2005 by InfoSiAL S.L.
email                : mail@infosial.com
***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
   Este  programa es software libre. Puede redistribuirlo y/o modificarlo
   bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
   versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include "FLScriptEditor.h"
#include "FLApplication.h"
#include "FLSqlDatabase.h"
#include "FLManagerModules.h"

#ifdef FL_DEBUGGER
#include "../qsa-debugger/src/ide/replacetext.h"
#include "../qsa-debugger/src/ide/preferencescontainer.h"
#include "../qsa-debugger/tools/designer/editor/preferences.h"
#else
#include "../qsa/src/ide/replacetext.h"
#include "../qsa/src/ide/preferencescontainer.h"
#include "../qsa/tools/designer/editor/preferences.h"
#endif

static QString skelCode( "/***************************************************************************\n"
                         "                                description\n"
                         "                             -------------------\n"
                         "    begin                :\n"
                         "    copyright            : InfoSiAL S.L.\n"
                         "    email                : mail@infosial.com\n"
                         " ***************************************************************************/\n"
                         "/***************************************************************************\n"
                         " *                                                                         *\n"
                         " *   This program is free software; you can redistribute it and/or modify  *\n"
                         " *   it under the terms of the GNU General Public License as published by  *\n"
                         " *   the Free Software Foundation; either version 2 of the License, or     *\n"
                         " *   ( at your option ) any later version.                                 *\n"
                         " *                                                                         *\n"
                         " ***************************************************************************/\n"
                         "/** @file */\n"
                         "/** @class_declaration interna */\n"
                         "////////////////////////////////////////////////////////////////////////////\n"
                         "//// DECLARACION ///////////////////////////////////////////////////////////\n"
                         "////////////////////////////////////////////////////////////////////////////\n"
                         "//////////////////////////////////////////////////////////////////\n"
                         "//// INTERNA /////////////////////////////////////////////////////\n"
                         "class interna {\n"
                         "  var ctx:Object;\n"
                         "  function interna(context) {\n"
                         "    this.ctx = context;\n"
                         "  }\n"
                         "  function main() {\n"
                         "    this.ctx.interna_main();\n"
                         "  }\n"
                         "  function init() {\n"
                         "    this.ctx.interna_init();\n"
                         "  }\n"
                         "}\n"
                         "//// INTERNA /////////////////////////////////////////////////////\n"
                         "//////////////////////////////////////////////////////////////////\n"
                         "/** @class_declaration head */\n"
                         "/////////////////////////////////////////////////////////////////\n"
                         "//// DESARROLLO /////////////////////////////////////////////////\n"
                         "class head extends oficial {\n"
                         "  function head(context) {\n"
                         "    oficial(context);\n"
                         "  }\n"
                         "}\n"
                         "//// DESARROLLO /////////////////////////////////////////////////\n"
                         "/////////////////////////////////////////////////////////////////\n"
                         "/** @class_declaration ifaceCtx*/\n"
                         "/////////////////////////////////////////////////////////////////\n"
                         "//// INTERFACE  /////////////////////////////////////////////////\n"
                         "class ifaceCtx extends head {\n"
                         "  function ifaceCtx(context) {\n"
                         "   head(context);\n"
                         "  }\n"
                         "}\n"
                         "const iface = new ifaceCtx(this);\n"
                         "//// INTERFACE  /////////////////////////////////////////////////\n"
                         "/////////////////////////////////////////////////////////////////\n"
                         "/** @class_definition interna */\n"
                         "////////////////////////////////////////////////////////////////////////////\n"
                         "//// DEFINICION ////////////////////////////////////////////////////////////\n"
                         "////////////////////////////////////////////////////////////////////////////\n"
                         "//////////////////////////////////////////////////////////////////\n"
                         "//// INTERNA /////////////////////////////////////////////////////\n"
                         "function interna_init() {}\n"
                         "function interna_main() {}\n"
                         "//// INTERNA /////////////////////////////////////////////////////\n"
                         "/////////////////////////////////////////////////////////////////\n" );

FLScriptEditor::FLScriptEditor( const QString & nFS, QWidget *parent, const char *name )
    : FLWidgetScriptEditor( parent, name, true, Qt::WType_Dialog | Qt::WShowModal |
                            Qt::WStyle_Maximize | Qt::WStyle_SysMenu ) {

  FLApplication * flapp = static_cast<FLApplication *>( qApp );
  nameObject_ = !flapp->nameObjectScript( nFS ) ? QString::null : *( flapp->nameObjectScript( nFS ) );

  QString code( nameObject_.isEmpty() ? skelCode : flapp->db()->managerModules()->getScript( nFS, 0, false ) );

  ed = new QSEditor( frMain );
  ed->setText( code );
  ed->setInterpreter( flapp->project() ->interpreter(), nameObject_.isEmpty() ? 0 : flapp->project() ->object( nameObject_ ) );
  frMainLayout->addWidget( ed );

  ed->setFocus();

  connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );

  replaceText = new QSReplaceText( this, 0, FALSE );
  connect( pbReplace, SIGNAL( clicked() ), this, SLOT( showReplace() ) );
  connect( replaceText->pushReplace, SIGNAL( clicked() ), this, SLOT( editReplace() ) );
  connect( replaceText->pushReplaceAll, SIGNAL( clicked() ), this, SLOT( editReplaceAll() ) );

  preferencesContainer = new QSPreferencesContainer( this, 0 );
  QBoxLayout *preferencesLayout = new QBoxLayout( preferencesContainer->frame, QBoxLayout::Down );
  qsaEditorSyntax = new PreferencesBase( preferencesContainer->frame, "qsaeditor_syntax" );
  preferencesLayout->addWidget( qsaEditorSyntax );
  qsaEditorSyntax->setPath( QString::fromLatin1( "/Trolltech/QSAScriptEditor/" ) );
  connect( preferencesContainer->pushOk, SIGNAL( clicked() ), this, SLOT( savePreferences() ) );
  connect( pbPref, SIGNAL( clicked() ), this, SLOT( editPreferences() ) );
}

FLScriptEditor::~FLScriptEditor() {}

QString FLScriptEditor::code() const {
  if ( ed && !nameObject_.isEmpty() )
    return ed->text().remove( "var form = Application." + nameObject_ + ";\n" );
  else
    return QString::null;
}

void FLScriptEditor::setCode( const QString &c ) {
  if ( ed )
    ed->setText( c );
}

bool FLScriptEditor::find( const QString & expr ) {
  if ( expr.isEmpty() )
    return true;
  lastSearch = expr;
  if ( ed->find( expr, true, false, true, true ) )
    return true;
  if ( ed->find( expr, true, false, false, true ) )
    return true;
  return ed->find( expr, true, false, true, false );
}

bool FLScriptEditor::reFind() {
  return find( lastSearch );
}

void FLScriptEditor::gotoLine( const QString & line ) {
  ed->gotoLine( line.toInt() );
  QTextEdit *textEdit = ed->textEdit();
  textEdit->ensureCursorVisible();
  int para, index;
  textEdit->getCursorPosition( &para, &index );
  textEdit->setSelection( para, index, para, index + 1 );
}

void FLScriptEditor::showReplace() {
  replaceText->show();
}

void FLScriptEditor::editReplace() {
  editReplace( false );
}

void FLScriptEditor::editReplaceAll() {
  editReplace( true );
}

void FLScriptEditor::editReplace( bool all ) {
  QString findString = replaceText->comboFind->currentText();
  replaceText->comboFind->insertItem( findString );
  QString replaceString = replaceText->comboReplace->currentText();
  replaceText->comboFind->insertItem( replaceString );
  bool caseSensitive = replaceText->checkCase->isChecked();
  bool wholeWordsOnly = replaceText->checkWhole->isChecked();
  bool startAtCursor = !replaceText->checkStart->isChecked();
  bool forward = replaceText->radioForward->isChecked();
  ed->replace( findString, replaceString, caseSensitive, wholeWordsOnly, forward, startAtCursor, all );
}

void FLScriptEditor::editPreferences() {
  qsaEditorSyntax->reInit();
  preferencesContainer->show();
}

void FLScriptEditor::savePreferences() {
  qsaEditorSyntax->save();
  ed->readSettings();
}

void FLScriptEditor::keyPressEvent( QKeyEvent *e ) {
  if ( e->key() == Key_Escape ) {
    e->ignore();
    return ;
  }
  QDialog::keyPressEvent( e );
}
