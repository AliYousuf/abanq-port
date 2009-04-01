/***************************************************************************
                 tpv_masterarqueos.qs  -  description
                             -------------------
    begin                : jue nov 17 2005
    copyright            : Por ahora (C) 2005 by InfoSiAL S.L.
    email                : lveb@telefonica.net
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/** @file */

/** @class_declaration interna */
////////////////////////////////////////////////////////////////////////////
//// DECLARACION ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//// INTERNA /////////////////////////////////////////////////////
class interna {
    var ctx:Object;
    function interna( context ) { this.ctx = context; }
    function init() { this.ctx.interna_init(); }
}
//// INTERNA /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/** @class_declaration oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
class oficial extends interna {
	var tdbRecords:FLTableDB;
	var ckbSoloPV:Object;
    function oficial( context ) { interna( context ); } 
	function abrirCerrarArqueo_clicked(){
		return this.ctx.oficial_abrirCerrarArqueo_clicked();
	}
	function abrirCerrarArqueo():Boolean{
		return this.ctx.oficial_abrirCerrarArqueo();
	}
	function imprimir_clicked(){
		return this.ctx.oficial_imprimir_clicked();
	}
	function imprimirTiqueArqueo(codArqueo:String):Boolean {
		return this.ctx.oficial_imprimirTiqueArqueo(codArqueo);
	}
	function abrirCerrarPagos(idArqueo:String, abrir:Boolean):Boolean {
		return this.ctx.oficial_abrirCerrarPagos(idArqueo, abrir);
	}
	function filtrarArqueos() {
		return this.ctx.oficial_filtrarArqueos();
	}
	function generarFacturasVentas(idArqueo:String):Boolean {
		return this.ctx.oficial_generarFacturasVentas(idArqueo);
	}
}
//// OFICIAL /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/** @class_declaration head */
/////////////////////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////
class head extends oficial {
    function head( context ) { oficial ( context ); }
}
//// DESARROLLO /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_declaration ifaceCtx */
/////////////////////////////////////////////////////////////////
//// INTERFACE  /////////////////////////////////////////////////
class ifaceCtx extends head {
    function ifaceCtx( context ) { head( context ); }
	function pub_abrirCerrarArqueo():Boolean{
		return this.abrirCerrarArqueo();
	}
	function pub_abrirCerrarPagos(idArqueo:String, abrir:Boolean):Boolean {
		return this.abrirCerrarPagos(idArqueo, abrir);
	}
}

const iface = new ifaceCtx( this );
//// INTERFACE  /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition interna */
////////////////////////////////////////////////////////////////////////////
//// DEFINICION ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//// INTERNA /////////////////////////////////////////////////////
/** \C
El botón abrir cerrar arqueo del formulario maestro permite abir o cerrar una arqueo.
Para poder cerrar una arqueo deben estar cerradas todas sus comandas
*/
function interna_init()
{
	this.iface.tdbRecords = this.child("tableDBRecords");
	this.iface.ckbSoloPV = this.child("ckbSoloPV");

	connect(this.child("tbnBlocDesbloc"), "clicked()", this, "iface.abrirCerrarArqueo_clicked()");
	connect(this.child("toolButtonPrint"),"clicked()", this, "iface.imprimir_clicked()");
	connect(this.child( "ckbSoloPV" ), "clicked()",  this, "iface.filtrarArqueos()");
}


//// INTERNA /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
/** \D
Abre una transacción y llama a la función abrirCerrarArqueo
*/
function oficial_abrirCerrarArqueo_clicked()
{
	var util:FLUtil = new FLUtil;
	var cursor:FLSqlCursor = this.cursor();
	
	cursor.transaction(false);
	try {
		if (this.iface.abrirCerrarArqueo())
			cursor.commit();
		else {
			cursor.rollback();
			return false;
		}
	}
	catch (e) {
		cursor.rollback();
		MessageBox.critical(util.translate("scripts", "Hubo un error en abrir o cerrar el arqueo:") + "\n" + e, MessageBox.Ok, MessageBox.NoButton);
	}
	cursor.commit();
	this.iface.tdbRecords.refresh();
}

/** \D
Abre o cierra el arqueo seleccionado
Para cerrar el arqueo debben estar cerradas todas sus comandas
Antes de cerrar el arqueo debemos establecer una fecha de cierre que será por defecto la fecha de la última comanda
La fecha de cierre debe ser siempre posterior a la fecha de inicio
El intervalo de fechas no puede solaparse al intervalo de otro arqueo ya existente
@return true si se ha abiert o cerrado correctamente y false si ha habido algún error
*/
function oficial_abrirCerrarArqueo():Boolean
{
	var util:FLUtil = new FLUtil();
	var diaHasta:String = this.cursor().valueBuffer("diahasta");
	var diaDesde:String = this.cursor().valueBuffer("diadesde");
	var ptoVenta:String = this.cursor().valueBuffer("ptoventa");
	var idArqueo:String = this.cursor().valueBuffer("idtpv_arqueo");
	
	var curArqueo:FLSqlCursor = new FLSqlCursor("tpv_arqueos");
	curArqueo.select("idtpv_arqueo = '" + idArqueo + "'");
	if (!curArqueo.first())
		return false;
	curArqueo.setModeAccess(curArqueo.Edit);
	curArqueo.refreshBuffer();
	if (curArqueo.valueBuffer("abierta")) {
		if (!curArqueo.valueBuffer("diahasta")) {
			var dialog:Dialog = new Dialog(util.translate ( "scripts", "Fecha de Cierre" ), 0, "fcierre");
			dialog.OKButtonText = util.translate ( "scripts", "Aceptar" );
			dialog.cancelButtonText = util.translate ( "scripts", "Cancelar" );
			
			var fecha:DateEdit= new DateEdit;
			fecha.date = util.sqlSelect("tpv_pagoscomanda","MAX(fecha)","idtpv_arqueo = '" + curArqueo.valueBuffer("idtpv_arqueo") + "'");
			if (!fecha.date)
				fecha.date = diaDesde;
			fecha.label = util.translate ( "scripts", "Fecha de Cierre:" );
			dialog.add( fecha );
		
			if ( !dialog.exec() )
				return true;
			if (!fecha.date)
				return false;
			diaHasta = fecha.date;
			curArqueo.setValueBuffer("diahasta", diaHasta);
		} else
			diaHasta = curArqueo.valueBuffer("diahasta");

		if (diaHasta < diaDesde) {
			MessageBox.warning(util.translate("scripts", "La fecha de cierre debe ser posterior a la fecha de inicio"),MessageBox.Ok, MessageBox.NoButton, MessageBox.NoButton);
			return false;
		}
		if (util.sqlSelect("tpv_arqueos", "diadesde", "idtpv_arqueo <> '" + idArqueo + "' AND ptoventa = '" + ptoVenta + "' AND ((diadesde >= '" + diaDesde + "' AND diahasta <= '" + diaDesde + "') OR (diadesde >= '" + diaHasta + "' AND diahasta <= '" + diaHasta + "'))")) {
			MessageBox.warning(util.translate("scripts", "Ya existe un arqueo para ese punto de venta que coincide con ese intervalo de fechas"), MessageBox.Ok, MessageBox.NoButton, MessageBox.NoButton);
			return false; 
		}
		var res:Number = MessageBox.warning(util.translate("scripts", "El arqueo se cerrará con fecha %1. ¿Desea continuar?").arg(util.dateAMDtoDMA(diaHasta)), MessageBox.Yes, MessageBox.No, MessageBox.NoButton);
		if (res != MessageBox.Yes)
			return false;
			
		if (!this.iface.abrirCerrarPagos(idArqueo, false))
			return false;
		curArqueo.setValueBuffer("abierta", false);
	} else {
		var res:Number = MessageBox.information(util.translate("scripts", "Se reabrirá el arqueo ¿Está seguro?"),MessageBox.Yes, MessageBox.No, MessageBox.NoButton);
		if (res != MessageBox.Yes)
			return false;

		if (!this.iface.abrirCerrarPagos(idArqueo, true))
			return false;
		curArqueo.setUnLock("abierta", true);
		curArqueo.select("idtpv_arqueo = '" + idArqueo + "'");
		if (!curArqueo.first())
			return false;
		curArqueo.setModeAccess(curArqueo.Edit);
		curArqueo.refreshBuffer();
		if (!flfact_tpv.iface.pub_borrarAsientoArqueo(curArqueo))
			return false;
		curArqueo.setNull("diahasta");
	}
	if (!curArqueo.commitBuffer())
		return false;
	return true;
}

/** \D Abre o cierra los pagos asociados a un arqueo
@param	idArqueo: Identificador del arqueo
@param	abrir: True indica que los pagos deben abrirse, false que deben cerrarse
@retuen	true si el proceso se realiza correctamente, false en caso contrario
\end */
function oficial_abrirCerrarPagos(idArqueo:String, abrir:Boolean):Boolean
{
	var util:FLUtil = new FLUtil;

	var mensaje:String;
	if (abrir)
		mensaje = util.translate("scripts", "Reabriendo arqueo %1...").arg(idArqueo);
	else
		mensaje = util.translate("scripts", "Cerrando arqueo %1...").arg(idArqueo);

	var qryPagos:FLSqlQuery = new FLSqlQuery;
	qryPagos.setTablesList("tpv_pagoscomanda");
	qryPagos.setSelect("idpago, idtpv_comanda");
	qryPagos.setFrom("tpv_pagoscomanda");
	qryPagos.setWhere("idtpv_arqueo = '" + idArqueo + "' ORDER BY idtpv_comanda");
	qryPagos.setForwardOnly(true);
	if (!qryPagos.exec())
		return false;
	
	var curPagos:FLSqlCursor = new FLSqlCursor("tpv_pagoscomanda");
	
	util.createProgressDialog(mensaje, qryPagos.size());
	var progreso:Number = 1;
	var idComanda:String;
	var idComandaPrevia:String = "";
	util.setProgress(progreso);
	while (qryPagos.next()) {
		idComanda = qryPagos.value("idtpv_comanda");
		curPagos.select("idpago = " + qryPagos.value("idpago"));
		if (!curPagos.first())
			return false;
		curPagos.setUnLock("editable", abrir);
		util.setProgress(progreso++);
	}
	util.destroyProgressDialog();
	if (!abrir) {
		if (!this.iface.generarFacturasVentas(idArqueo))
			return false;
	}
	return true;
}

function oficial_generarFacturasVentas(idArqueo:String):Boolean
{
	var util:FLUtil = new FLUtil;
	var curComandas:FLSqlCursor = new FLSqlCursor("tpv_comandas");
	curComandas.select("idfactura IS NULL AND idtpv_comanda IN (select idtpv_comanda from tpv_pagoscomanda where idtpv_arqueo = '" + idArqueo + "')");
	curComandas.setActivatedCommitActions(false);

	util.createProgressDialog(util.translate("scripts", "Generando facturas de venta"), curComandas.size());
	var progreso:Number = 1;
	while (curComandas.next()) {
		util.setProgress(progreso++);
		curComandas.setModeAccess(curComandas.Edit);
		curComandas.refreshBuffer()
		idFactura = flfact_tpv.iface.pub_crearFactura(curComandas);
		if (!idFactura) {
			util.destroyProgressDialog();
			return false;
		}
		curComandas.setValueBuffer("idfactura", idFactura);
		if (!flfact_tpv.iface.pub_generarRecibos(curComandas)) {
			util.destroyProgressDialog();
			return false;
		}
		if (!curComandas.commitBuffer()) {
			util.destroyProgressDialog();
			return false;
		}
	}
	util.destroyProgressDialog();
	return true;
}

/** \C
Imprime un tique para el arqueo seleccionado
*/
function oficial_imprimir_clicked()
{
	var cursor:FLSqlCursor = this.cursor();
	var codArqueo:String = cursor.valueBuffer("idtpv_arqueo");
	if(!codArqueo)
		return false;
	
	if (!this.iface.imprimirTiqueArqueo(codArqueo)){
		return false;
	}
}

/** \D Lanza la función imprimir con los datos del arqueo
Si no está cargado el módulo de informes mostrará un mensaje de aviso
@param codArqueo código del arqueo del que queremos sacar el tique
*/
function oficial_imprimirTiqueArqueo(codArqueo:String):Boolean
{
	if (sys.isLoadedModule("flfactinfo")) {
		if (!this.cursor().isValid())
			return;
		var curImprimir:FLSqlCursor = new FLSqlCursor("tpv_i_arqueos");
		curImprimir.setModeAccess(curImprimir.Insert);
		curImprimir.refreshBuffer();
		curImprimir.setValueBuffer("descripcion", "temp");
		curImprimir.setValueBuffer("d_tpv__arqueos_idtpv__arqueo", codArqueo);
		curImprimir.setValueBuffer("h_tpv__arqueos_idtpv__arqueo", codArqueo);
		flfactinfo.iface.pub_lanzarInforme(curImprimir, "tpv_i_arqueos");
	} else
		flfactppal.iface.pub_msgNoDisponible("Informes");
}

/** \D Activa o desactiva el filtro que muestra únicamente los arqueos del puesto por defecto.
\end */
function oficial_filtrarArqueos()
{
	var util:FLUtil = new FLUtil;
	var cursor:FLSqlCursor = this.cursor();
	var filtro:String = "";
	if (this.iface.ckbSoloPV.checked) {
		var codTerminal:String = util.readSettingEntry("scripts/fltpv_ppal/codTerminal");
		if (codTerminal) {
			filtro += "ptoventa = '" + codTerminal + "'";
		}
	}
	cursor.setMainFilter(filtro);
	this.iface.tdbRecords.refresh();
}

//// OFICIAL /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition head */
/////////////////////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////

//// DESARROLLO /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
