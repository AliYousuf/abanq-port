/***************************************************************************
                 tpv_mastervales.qs  -  description
                             -------------------
    begin                : mie nov 15 2006
    copyright            : Por ahora (C) 2006 by InfoSiAL S.L.
    email                : mail@infosial.com
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
    function init() { 
		this.ctx.interna_init();
	}
}
//// INTERNA /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/** @class_declaration oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
class oficial extends interna {
	var tbnPrint:Object;
	function oficial( context ) { interna( context ); } 
	function imprimirClicked() {
		return this.ctx.oficial_imprimirClicked();
	}
	function imprimir(referencia:String) {
		return this.ctx.oficial_imprimir(referencia);
	}
	function imprimirKugar(referencia:String) {
		return this.ctx.oficial_imprimirKugar(referencia);
	}
	function imprimirESC(codTerminal:String, referencia:String) {
		return this.ctx.oficial_imprimirESC(codTerminal, referencia);
	}
	function cerosIzquierda(numero:String, totalCifras:Number):String{
		return this.ctx.oficial_cerosIzquierda(numero, totalCifras);
	}
	function cerosDerecha(numero:String, totalCifras:Number):String{
		return this.ctx.oficial_cerosDerecha(numero, totalCifras);
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
	function pub_imprimir(referencia:String) {
		return this.imprimir(referencia);
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
*/
function interna_init()
{
	var util:FLUtil = new FLUtil();
	var cursor:FLSqlCursor = this.cursor();

	this.iface.tbnPrint = this.child("toolButtonPrint");

	connect(this.iface.tbnPrint, "clicked()", this, "iface.imprimirClicked");
}

//// INTERNA /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
function oficial_imprimirClicked()
{
	var cursor:FLSqlCursor = this.cursor();
	var util:FLUtil = new FLUtil();
	var referencia:String = cursor.valueBuffer("referencia");
	if (!referencia)
		return false;

	this.iface.imprimir(referencia);

}

/** \D Llama a una función de impresión u otra en función del tipo de impresora asociada el punto de venta actual
@param	referencia: Referencia del vale a imprimir
\end */
function oficial_imprimir(referencia:String) 
{
	var util:FLUtil = new FLUtil();
	//////////////////////////////////////
	var fis:FLFiscalBixolon;
	var port:String = "COM1";
	var cante:String;
	var cantd:String;
	var error:Number;
	var status:Number;
	var cmd:String;
	fis.openPort(port);
	var valor:Number = util.sqlSelect("tpv_vales", "importe", "referencia = '" + referencia + "'");

	var importe:String = valor.toString();
	var posPri:Number = importe.search(".");

	if (posPri >= 0){

		
		cante =  this.iface.cerosIzquierda(importe.mid(0 , posPri), 10);

		cantd =  this.iface.cerosDerecha(importe.mid(posPri+1, 2), 2);
	}
	else{
		
		cante =  this.iface.cerosIzquierda(importe, 10);
		cantd =  this.iface.cerosDerecha(0, 2);

	}

	cmd = "f01" + cante + cantd;

	fis.sendCmd(status, error, cmd);

	fis.closedPort();
	//////////////////////////////////////////////
	var codTerminal:String = util.readSettingEntry("scripts/fltpv_ppal/codTerminal");
	if (!codTerminal)
		return this.iface.imprimirKugar(referencia);

	var tipoImpresora:String = util.sqlSelect("tpv_puntosventa", "tipoimpresora", "codtpv_puntoventa = '" + codTerminal + "'");
	if (tipoImpresora == "ESC-POS")
		return this.iface.imprimirESC(codTerminal, referencia);

	
	this.iface.imprimirKugar(referencia);
	

}

/** \D Función de impresión de vales para impresoras térmicas, a través de Kugar
@param	referencia: Referencia del vale a imprimir
\end */
function oficial_imprimirKugar(referencia:String)
{
	var cursor:FLSqlCursor = this.cursor();

	if (sys.isLoadedModule("flfactinfo")) {
		var curImprimir:FLSqlCursor = new FLSqlCursor("tpv_i_vales");
		curImprimir.setModeAccess(curImprimir.Insert);
		curImprimir.refreshBuffer();
		curImprimir.setValueBuffer("descripcion", "temp");
		curImprimir.setValueBuffer("i_tpv__vales_referencia", referencia);
		flfactinfo.iface.pub_lanzarInforme(curImprimir, "tpv_i_vales");
	} else
		flfactppal.iface.pub_msgNoDisponible("Informes");
}

/** \D Función de impresión de vales para impresoras ESC-POS
@param	referencia: Referencia del vale a imprimir
\end */
function oficial_imprimirESC(codTerminal:String, referencia:String)
{
	var util:FLUtil = new FLUtil;
	var cursor:FLSqlCursor = this.cursor();

	var impresora:String = util.sqlSelect("tpv_puntosventa", "impresora", "codtpv_puntoventa = '" + codTerminal + "'");
	if (!impresora)
		return;

	flfact_tpv.iface.establecerImpresora(impresora);

	var qryVale:FLSqlQuery = new FLSqlQuery("tpv_i_vales");
	qryVale.setWhere("tpv_vales.referencia = '" + referencia + "'");
	if (!qryVale.exec())
		return false;

	if (!qryVale.first())
		return false;

	flfact_tpv.iface.imprimirDatos(qryVale.value("empresa.nombre"));
	flfact_tpv.iface.impNuevaLinea();
	flfact_tpv.iface.imprimirDatos("R.I.F. ");
	flfact_tpv.iface.imprimirDatos(qryVale.value("empresa.cifnif"));
	flfact_tpv.iface.impNuevaLinea(2);
	flfact_tpv.iface.impAlinearH(1);
	flfact_tpv.iface.impResaltar(true);
	flfact_tpv.iface.imprimirDatos("*** " + util.translate("scripts", "VALE DE COMPRA") + " ***");
	flfact_tpv.iface.impNuevaLinea();
	flfact_tpv.iface.impResaltar(false);
	flfact_tpv.iface.impAlinearH(0);
	flfact_tpv.iface.imprimirDatos(util.translate("scripts", "Referencia: ") + qryVale.value("tpv_vales.referencia"));
	flfact_tpv.iface.impNuevaLinea();
	flfact_tpv.iface.imprimirDatos(util.translate("scripts", "Emisión: ") + util.dateAMDtoDMA(qryVale.value("tpv_vales.fechaemision")));
	flfact_tpv.iface.impNuevaLinea();
	flfact_tpv.iface.imprimirDatos(util.translate("scripts", "Caducidad: ") + util.dateAMDtoDMA(qryVale.value("tpv_vales.fechacaducidad")));
	flfact_tpv.iface.impNuevaLinea();
	flfact_tpv.iface.imprimirDatos(util.translate("scripts", "Importe: ") + util.roundFieldValue(qryVale.value("tpv_vales.saldo"), "tpv_vales", "importe"));
	flfact_tpv.iface.impNuevaLinea();
	flfact_tpv.iface.impResaltar(true);
	flfact_tpv.iface.imprimirDatos(util.translate("scripts", "Saldo: ") + util.roundFieldValue(qryVale.value("tpv_vales.saldo"), "tpv_vales", "saldo"));
	flfact_tpv.iface.impResaltar(false);
	flfact_tpv.iface.impNuevaLinea(8);
	flfact_tpv.iface.impCortar();
	flfact_tpv.iface.flushImpresora();
}



function oficial_cerosIzquierda(numero:String, totalCifras:Number):String
{
				var ret:String = numero.toString();
				var numCeros:Number = totalCifras - ret.length;
				for ( ; numCeros > 0 ; --numCeros)
					ret = "0" + ret;
				return ret;
}

function oficial_cerosDerecha(numero:String, totalCifras:Number):String
{
				var ret:String = numero.toString();
				var numCeros:Number = totalCifras - ret.length;
				for ( ; numCeros > 0 ; --numCeros)
					ret = ret + "0";
				return ret;
}

//// OFICIAL /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition head */
/////////////////////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////

//// DESARROLLO /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
