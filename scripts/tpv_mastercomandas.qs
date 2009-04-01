/***************************************************************************
                 tpv_mastercomandas.qs  -  description
                             -------------------
    begin                : mar nov 15 2005
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
	var tbnBlocDesbloc:Object;
	var tdbRecords:FLTableDB;
	var ckbSoloHoy:Object;
	var ckbSoloPV:Object;
	function oficial( context ) { interna( context ); } 
	function abrirComanda_clicked() {
		return this.ctx.oficial_abrirComanda_clicked();
	}
	function abrirComanda(idComanda:String):Boolean {
		return this.ctx.oficial_abrirComanda(idComanda);
	}
	function eliminarFactura(idFactura:Number):Boolean {
		return this.ctx.oficial_eliminarFactura(idFactura);
	}
	function imprimir_clicked(){
		return this.ctx.oficial_imprimir_clicked();
	}
	function imprimirTiqueComanda(codComanda:String):Boolean{
		return this.ctx.oficial_imprimirTiqueComanda(codComanda);
	}
	function imprimirFactura_clicked():Boolean{
		return this.ctx.oficial_imprimirFactura_clicked();
	}
	function abrirCajon_clicked() {
		return this.ctx.oficial_abrirCajon_clicked();
	}
	function imprimirQuick_clicked(){
		return this.ctx.oficial_imprimirQuick_clicked();
	}
	function abrirCajon( impresora:String ) {
		return this.ctx.oficial_abrirCajon( impresora );
	}
	function imprimirQuick( codComanda:String, impresora:String ) {
		return this.ctx.oficial_imprimirQuick( codComanda, impresora );
	}
	function filtrarVentas() {
		return this.ctx.oficial_filtrarVentas();
	}
	function filtroVentas():String {
		return this.ctx.oficial_filtroVentas();
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
	function pub_imprimirTiqueComanda(codComanda:Stirng):Boolean{
		return this.imprimirTiqueComanda(codComanda);
	}
	function pub_abrirCajon( impresora:String ) {
		return this.abrirCajon( impresora );
	}
	function pub_imprimirQuick( codComanda:String, impresora:String ) {
		return this.imprimirQuick( codComanda, impresora );
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
function interna_init()
{
	this.iface.tbnBlocDesbloc = this.child("tbnBlocDesbloc");
	this.iface.tdbRecords = this.child("tableDBRecords");
	this.iface.ckbSoloHoy = this.child("ckbSoloHoy");
	this.iface.ckbSoloPV = this.child("ckbSoloPV");

	connect(this.iface.tbnBlocDesbloc, "clicked()", this, "iface.abrirComanda_clicked()");
	connect(this.child("toolButtonPrint"),"clicked()", this, "iface.imprimir_clicked()");
	connect( this.child( "tbnPrintQuick" ), "clicked()", this, "iface.imprimirQuick_clicked()" );
	connect( this.child( "tbnOpenCash" ), "clicked()",  this, "iface.abrirCajon_clicked()" );
	connect( this.child( "tbnImprimirFactura" ), "clicked()",  this, "iface.imprimirFactura_clicked()" );
	connect( this.child( "ckbSoloHoy" ), "clicked()",  this, "iface.filtrarVentas()" );
	connect( this.child( "ckbSoloPV" ), "clicked()",  this, "iface.filtrarVentas()" );

	this.iface.filtrarVentas();
}
//// INTERNA /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
/** \D
Abre una comanda
No se podrá abir una comanda si su arqueo está cerrado
@return Boolean, devuelve true si todo se ha ejecutado correctamente y false si hay algún error
*/
function oficial_abrirComanda_clicked():Boolean
{
	var util:FLUtil = new FLUtil();
	var cursor:FLSqlCursor = this.cursor();
	var idFactura:Number = cursor.valueBuffer("idfactura");
	var idComanda:Number = cursor.valueBuffer("idtpv_comanda");
	if (!idComanda)
		return false;
	
	if (cursor.valueBuffer("editable") == true) {
		MessageBox.warning(util.translate("scripts", "La venta ya está abierta"), MessageBox.Ok, MessageBox.NoButton, MessageBox.NoButton);
		return true;
	}
	
	/*
	if (!util.sqlSelect("tpv_arqueos", "abierta", "idtpv_arqueo = '" + cursor.valueBuffer("idtpv_arqueo") + "'")) {
		MessageBox.warning(util.translate("scripts", "No pueden abrirse ventas asociadas a un arqueo cerrado"), MessageBox.Ok, MessageBox.NoButton, MessageBox.NoButton);
		return true;
	}
	*/
	
	var res:Number = MessageBox.warning(util.translate("scripts", "Va a abrir la venta seleccionada"), MessageBox.Ok, MessageBox.Cancel);
	if (res != MessageBox.Ok)
		return true;
	
	cursor.transaction(false);
	try {
		if (this.iface.abrirComanda(idComanda))
			cursor.commit();
		else {
			cursor.rollback();
			return false;
		}
	}
	catch (e) {
		cursor.rollback();
		MessageBox.critical(util.translate("scripts", "Hubo un error en la apertura de la venta:") + "\n" + e, MessageBox.Ok, MessageBox.NoButton);
	}
	
	this.iface.tdbRecords.refresh();
	
	return true;
}

/** \D Abre la comanda
@param	idComanda: identificador de la comanda
@return	true si la comanda se abre correctamente, false en caso contrario
\end */
function oficial_abrirComanda(idComanda:String):Boolean
{
	var util:FLUtil = new FLUtil();
	var curComanda:FLSqlCursor = new FLSqlCursor("tpv_comandas");
	curComanda.select("idtpv_comanda = " + idComanda);
	if (!curComanda.first())
		return false;
	curComanda.setUnLock("editable", true)
	
	curComanda.select("idtpv_comanda = " + idComanda);
	if (!curComanda.first())
		return false;
		
	curComanda.setModeAccess(curComanda.Edit);
	curComanda.refreshBuffer();
	curComanda.setValueBuffer("estado","Abierta");
	if (!curComanda.commitBuffer())
		return false;
		
	return true;
}
/** \D
Elimina los el pago, recibo y factura que corresponden a la comanda
@param idFactura identificador de la factura a borrar
@return Boolean, devuelve true si todo se ha ejecutado correctamente y fasle si hay algún error
*/
function oficial_eliminarFactura(idFactura:Number):Boolean 
{
	var util:FLUtil = new FLUtil();
	var curFactura:FLSqlCursor = new FLSqlCursor("facturascli");
	curFactura.select("idfactura = " + idFactura);
	if(!curFactura.first())
		return false;
	var codRecibo = curFactura.valueBuffer("codigo") + "-01";
	if(util.sqlSelect("reciboscli","estado","codigo = '" + codRecibo + "'") == "Pagado"){
		var idrecibo:Number = util.sqlSelect("reciboscli","idrecibo","codigo = '" + codRecibo + "'");
		var curPagos:FLSqlCursor = new FLSqlCursor("pagosdevolcli");
		curPagos.select("idrecibo = " + idrecibo);
		if(!curPagos.first())
			return false; 
		curPagos.setModeAccess(curPagos.Del);
		curPagos.refreshBuffer();
		if(!curPagos.commitBuffer())
			return false;
		var curRecibos:FLSqlCursor = new FLSqlCursor("reciboscli");
		curRecibos.select("idrecibo = " + idrecibo);
		if(!curRecibos.first())
			return false; 
		curRecibos.setModeAccess(curRecibos.Edit);
		curRecibos.refreshBuffer();
		curRecibos.setValueBuffer("estado","Emitido");
		if(!curRecibos.commitBuffer())
			return false;
		curFactura.setUnLock("editable",true);
	}	
	curFactura.setModeAccess(curFactura.Del);
	curFactura.refreshBuffer();
	if(!curFactura.commitBuffer())
		return false;
	return true;
}

/** \D
Abre una transacción y llama a la función ImprimirTiqueComanda
*/
function oficial_imprimir_clicked()
{
	var cursor:FLSqlCursor = this.cursor();
	var codComanda:String = cursor.valueBuffer("codigo");
	if (!codComanda)
		return false;
	
	if (!this.iface.imprimirTiqueComanda(codComanda)){
		return false;
	}

	this.iface.tdbRecords.cursor().refresh();
}

/** \D
Si el módulo de informes no está cargado muestra un mensaje de aviso y si lo está lanza el informe correspondiente
@param codComanda codigo de la comanda a imprimir
@return true si se imprime correctamente y false si ha algún error
*/
function oficial_imprimirTiqueComanda(codComanda:String):Boolean
{
	if (sys.isLoadedModule("flfactinfo")) {
		if (!this.cursor().isValid())
			return;
		var curImprimir:FLSqlCursor = new FLSqlCursor("tpv_i_comandas");
		curImprimir.setModeAccess(curImprimir.Insert);
		curImprimir.refreshBuffer();
		curImprimir.setValueBuffer("descripcion", "temp");
		curImprimir.setValueBuffer("d_tpv__comandas_codigo", codComanda);
		curImprimir.setValueBuffer("h_tpv__comandas_codigo", codComanda);
		flfactinfo.iface.pub_lanzarInforme(curImprimir, "tpv_i_comandas");
	} else
		flfactppal.iface.pub_msgNoDisponible("Informes");
	
}

/** \D
Manda a imprimir directamente a la impresora la comanda actualmente seleccionada
*/
function oficial_imprimirQuick_clicked()
{
/*	
	if (!this.cursor().isValid())
		return;
	
	var util:FLUtil = new FLUtil();
	var pv:String = util.readSettingEntry( "scripts/fltpv_ppal/codTerminal" );

	if ( !pv )
			pv = util.sqlSelect( "tpv_puntosventa", "codtpv_puntoventa", "1=1") ;

	var codComanda:String = this.cursor().valueBuffer("idtpv_comanda");

	var qryTicket:FLSqlQuery = new FLSqlQuery("tpv_i_comandas");
	qryTicket.setWhere("tpv_comandas.idtpv_comanda = '" + codComanda + "'");
	if (!qryTicket.exec())
		return false;

	var impresora:String = util.sqlSelect( "tpv_puntosventa", "impresora","codtpv_puntoventa = '" + pv + "'") ;
	flfact_tpv.iface.establecerImpresora(impresora);

	var primerRegistro:Boolean = true;
	while (qryTicket.next()) {
		if (primerRegistro) {
			flfact_tpv.iface.imprimirDatos(qryTicket.value("empresa.nombre"));
			flfact_tpv.iface.impNuevaLinea();
			flfact_tpv.iface.imprimirDatos(qryTicket.value("empresa.direccion"));
			flfact_tpv.iface.impNuevaLinea();
			flfact_tpv.iface.imprimirDatos(qryTicket.value("empresa.ciudad"));
			flfact_tpv.iface.impNuevaLinea();
			flfact_tpv.iface.imprimirDatos("C.I.F. ");
			flfact_tpv.iface.imprimirDatos(qryTicket.value("empresa.cifnif"));
			flfact_tpv.iface.impNuevaLinea();
			flfact_tpv.iface.impResaltar(true);
			flfact_tpv.iface.imprimirDatos("Tiquet: " + qryTicket.value("tpv_comandas.codigo"));
			flfact_tpv.iface.impResaltar(false);
			flfact_tpv.iface.impAlinearH(2);
			flfact_tpv.iface.imprimirDatos(util.dateAMDtoDMA(qryTicket.value("tpv_comandas.fecha")));
			flfact_tpv.iface.impAlinearH(0);
			flfact_tpv.iface.impNuevaLinea();
			flfact_tpv.iface.impSubrayar(true);
			//flfact_tpv.iface.imprimirDatos("ESC:1B,52,07");
			flfact_tpv.iface.imprimirDatos("ESC:1B,74,19");
			flfact_tpv.iface.imprimirDatos("Camión", 20);
			flfact_tpv.iface.imprimirDatos("Ínigo, Pléyade, Cáscara");
			flfact_tpv.iface.impAlinearH(2);
			flfact_tpv.iface.imprimirDatos("PVP");
			flfact_tpv.iface.impSubrayar(false);
			flfact_tpv.iface.impNuevaLinea(5);
			flfact_tpv.iface.imprimirDatos("PVP");
			flfact_tpv.iface.impNuevaLinea();
		}
		primerRegistro = false;
	}
	flfact_tpv.iface.i();
	flfact_tpv.iface.flushImpresora();
	*/


	if (!this.cursor().isValid())
		return;
	
	var util:FLUtil = new FLUtil();
	var pv:String = util.readSettingEntry( "scripts/fltpv_ppal/codTerminal" );

	if ( !pv )
			pv = util.sqlSelect( "tpv_puntosventa", "codtpv_puntoventa", "1=1") ;
		
	var impresora:String = util.sqlSelect( "tpv_puntosventa", "impresora","codtpv_puntoventa = '" + pv + "'") ;	
	
	this.iface.imprimirQuick( this.cursor().valueBuffer( "codigo" ) , impresora );

}

function oficial_imprimirQuick( codComanda:String, impresora:String )
{
	var q:FLSqlQuery = new FLSqlQuery( "tpv_i_comandas" );
	var util:FLUtil = new FLUtil();		
	
	q.setWhere( "codigo = '" + codComanda + "'" );
	if (q.exec() == false) {
		MessageBox.critical(util.translate("scripts", "Falló la consulta"), MessageBox.Ok, MessageBox.NoButton);
		return;
	} else {
		if (q.first() == false) {
			MessageBox.warning(util.translate("scripts", "No hay registros que cumplan los criterios de búsqueda establecidos"), MessageBox.Ok, MessageBox.NoButton);
			return;
		}
	}

	var tipoImpresora:String = util.sqlSelect("tpv_comandas c INNER JOIN tpv_puntosventa pv ON c.codtpv_puntoventa = pv.codtpv_puntoventa", "tipoimpresora", "c.codigo = '" + codComanda + "'", "tpv_comandas,tpv_puntosventa");

	var rptViewer:FLReportViewer = new FLReportViewer();
	rptViewer.setReportTemplate( "tpv_i_comandas" );
	rptViewer.setReportData( q );
	rptViewer.renderReport();
	if (tipoImpresora == "ESC-POS")
		rptViewer.setPrintToPos( true );
	rptViewer.setPrinterName( impresora );
	rptViewer.printReport();
}

/** \D
Abre el cajón del punto de venta actual
*/
function oficial_abrirCajon_clicked() {
		var util:FLUtil = new FLUtil();
		var pv:String = util.readSettingEntry( "scripts/fltpv_ppal/codTerminal" );
		
		if ( !pv )
				pv = util.sqlSelect( "tpv_puntosventa", "codtpv_puntoventa", "1=1") ;
		
		var impresora:String = util.sqlSelect( "tpv_puntosventa", "impresora","codtpv_puntoventa = '" + pv + "'") ;
		
		this.iface.abrirCajon( impresora );
}

/** \D
Abre el cajón portamonedas conectado a una impresora
@impresora Nombre de la impresora LPR donde está conectado el cajón
*/
function oficial_abrirCajon( impresora:String ) {
		var printer:FLPosPrinter = new FLPosPrinter();
		printer.setPrinterName( impresora );
		printer.send( "ESC:10,14,01,00,01" );
		printer.flush();
}

/** \D
Imprime la factura correspondiente a la venta seleccionada
*/
function oficial_imprimirFactura_clicked()
{
	var idFactura:String = this.cursor().valueBuffer("idfactura");
	if (!idFactura)
		return;
		
	var util:FLUtil = new FLUtil;
	var codFactura:String = util.sqlSelect("facturascli", "codigo", "idfactura = " + idFactura);
	if (codFactura)
		formfacturascli.iface.pub_imprimir(codFactura);
}

/** \D Activa o desactiva el filtro que muestra únicamente las últimas ventas o las del puesto por defecto. El filtro mejora el rendimiento
\end */
function oficial_filtrarVentas()
{
	var cursor:FLSqlCursor = this.cursor();
	var filtro:String = this.iface.filtroVentas();
	if (!filtro && filtro != "")
		return;

	cursor.setMainFilter(filtro);
	this.iface.tdbRecords.refresh();
}

function oficial_filtroVentas():String
{
	var filtro:String = "";
	var util:FLUtil = new FLUtil;
	if (this.iface.ckbSoloHoy.checked) {
		var hoy:Date = new Date;
		filtro = "fecha = '" + hoy.toString() + "'";
	}

	if (this.iface.ckbSoloPV.checked) {
		var codTerminal:String = util.readSettingEntry("scripts/fltpv_ppal/codTerminal");
		if (codTerminal) {
			if (filtro != "")
				filtro += " AND ";
			filtro += "codtpv_puntoventa = '" + codTerminal + "'";
		}
	}
	return filtro;
}
//// OFICIAL /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition head */
/////////////////////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////

//// DESARROLLO /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
