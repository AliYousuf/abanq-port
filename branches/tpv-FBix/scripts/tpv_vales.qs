/***************************************************************************
                 tpv_vales.qs  -  description
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
	function calculateField(fN:String):String {
		return this.ctx.interna_calculateField(fN);
	}
}
//// INTERNA /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/** @class_declaration oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
class oficial extends interna {
	var tdbPagosComanda:Object;
	var tdbLineasVale:Object;
	function oficial( context ) { interna( context ); } 
	function bufferChanged(fN:String) {
		return this.ctx.oficial_bufferChanged(fN);
	}
	function calcularTotales() {
		return this.ctx.oficial_calcularTotales();
	}
	/////////////////////////////////////////
	function cerosIzquierda(numero:String, totalCifras:Number):String{
		return this.ctx.oficial_cerosIzquierda(numero, totalCifras);
	}
	function cerosDerecha(numero:String, totalCifras:Number):String{
		return this.ctx.oficial_cerosDerecha(numero, totalCifras);
	}
	function encontrarSerial():String {
		return this.ctx.oficial_encontrarSerial();
	}
	function espaciosDerecha(cad:String, totalCifras:Number):String {
		return this.ctx.oficial_espaciosDerecha( cad, totalCifras );
	}
	function enviarNotaCredito() {
		return this.ctx.oficial_enviarNotaCredito( );
	}
	////////////////////////////////////////
}
//// OFICIAL /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/** @class_declaration head */
/////////////////////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////
class head extends oficial {
	function head( context ) { oficial ( context ); }
	///////////////////////////////////
	function pub_calculateField(fN:String):String {
		return this.calculateField(fN);
	}
	function pub_encontrarSerial():String {
		return this.encontrarSerial();
	}
	///////////////////////////////////
}
//// DESARROLLO /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_declaration ifaceCtx */
/////////////////////////////////////////////////////////////////
//// INTERFACE  /////////////////////////////////////////////////
class ifaceCtx extends head {
    function ifaceCtx( context ) { head( context ); }
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

	this.iface.tdbPagosComanda = this.child("tdbPagosComanda");
	this.iface.tdbPagosComanda.setReadOnly(true);
	this.iface.tdbLineasVale = this.child("tdbLineasVale");
	

	connect(cursor, "bufferChanged(QString)", this, "iface.bufferChanged");
	connect(this.iface.tdbLineasVale.cursor(), "bufferCommited()", this, "iface.calcularTotales()");

	//////////////////////////////////////////////////
	/*if(1){
		MessageBox.warning(util.translate("scripts", "valor1 " + parseFloat(this.iface.calculateField("comanda"))), MessageBox.Ok, MessageBox.NoButton);
		MessageBox.warning(util.translate("scripts", "valor2 " + this.iface.calculateField("nombrecliente")), MessageBox.Ok, MessageBox.NoButton);
		MessageBox.warning(util.translate("scripts", "valor3 " + this.iface.calculateField("cedrif")), MessageBox.Ok, MessageBox.NoButton);
		MessageBox.warning(util.translate("scripts", "valor4 " + this.iface.calculateField("fecha")), MessageBox.Ok, MessageBox.NoButton);
		//MessageBox.warning(util.translate("scripts", "valor5 " + this.iface.enviarNotaCredito()), MessageBox.Ok, MessageBox.NoButton);
	}
	MessageBox.warning(util.translate("scripts", "valor6 " + this.iface.encontrarSerial()), MessageBox.Ok, MessageBox.NoButton);*/

	this.iface.enviarNotaCredito();

	/////////////////////////////////////////////////
	
	switch (cursor.modeAccess()) {
		case cursor.Insert: {
			this.child("fdbReferencia").setValue(this.iface.calculateField("referencia"));
			this.iface.bufferChanged("fechaemision"); 
			break;
		}
		case cursor.Edit: {
			break;
		}
	}
}

function interna_calculateField(fN:String):String
{
	var util:FLUtil = new FLUtil();
	var valor:String;
	var cursor:FLSqlCursor = this.cursor();

	switch (fN) {
		/** \C
		La --fechacaducidad-- es la fecha de emisión más el período de validez del vale establecido en el formulario de datos generales
		*/
		case "fechacaducidad": {
			var diasValidez:String = util.sqlSelect("tpv_datosgenerales", "diasvalidezvale", "1 = 1");
			if (!diasValidez) {
				MessageBox.warning(util.translate("scripts", "No tiene establecido el parámetro Días de validez de los vales en el formulario de datos generales.\nDebe establecer este valor para poder calcular la fecha de caducidad en función de la de emisión"), MessageBox.Ok, MessageBox.NoButton);
				break;
			}
			valor = util.addDays(cursor.valueBuffer("fechaemision"), diasValidez);
			break;
		}
		case "saldo": {
			valor = cursor.valueBuffer("importe");
			break;
		}
		/** \C La --referencia-- del vale se construye como el código de la vena asociada más un secuencial
		\end */
		case "referencia": {
			var idComanda:String = cursor.valueBuffer("idtpv_comanda");
			if (!idComanda)
				break;
			var numRefs:Number = util.sqlSelect("tpv_vales", "COUNT(referencia)", "idtpv_comanda = " + idComanda);
			if (!numRefs)
				numRefs = 0;
			numRefs++;
			valor = util.sqlSelect("tpv_comandas", "codigo", "idtpv_comanda = " + idComanda) + "-" + flfacturac.iface.cerosIzquierda(numRefs, 3);
			break;
		}
		////////////////////////////////////////////////////////
		case "comanda": {
			var idComanda:String = cursor.valueBuffer("idtpv_comanda");
			if (!idComanda)
				break;
			valor = util.sqlSelect("tpv_comandas", "codigo", "idtpv_comanda = " + idComanda);
			break;
		}

		case "nombrecliente": {
			var idComanda:String = cursor.valueBuffer("idtpv_comanda");
			if (!idComanda)
				break;
			valor = util.sqlSelect("tpv_comandas", "nombrecliente", "idtpv_comanda = " + idComanda);
			break;
		}
		case "cedrif": {
			var idComanda:String = cursor.valueBuffer("idtpv_comanda");
			if (!idComanda)
				break;
			var codcli:String = util.sqlSelect("tpv_comandas", "codcliente", "idtpv_comanda = " + idComanda);
			if (!codcli)
				break;

			valor = util.sqlSelect("clientes", "cifnif", "codcliente = '" + codcli + "'");
			break;
		}
		case "fecha": {
			var idComanda:String = cursor.valueBuffer("idtpv_comanda");
			if (!idComanda)
				break;
			valor = util.sqlSelect("tpv_comandas", "fecha", "idtpv_comanda = " + idComanda);
			break;
		}
		case "hora": {
			var idComanda:String = cursor.valueBuffer("idtpv_comanda");
			if (!idComanda)
				break;
			valor = util.sqlSelect("tpv_comandas", "hora", "idtpv_comanda = " + idComanda);
			break;
		}
		/////////////////////////////////////////////////////////
	}
	return valor;
}

//// INTERNA /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
function oficial_bufferChanged(fN:String)
{
	var util:FLUtil = new FLUtil();
	var cursor:FLSqlCursor = this.cursor();
	switch (fN) {
		/** \C
		Al cambiar la --fechaemision-- se calcula la --fechacaducidad-- 
		*/
		case "fechaemision": {
			this.child("fdbFechaCaducidad").setValue(this.iface.calculateField("fechacaducidad"));
			break;
		}
		/** \C
		El --saldo-- por defecto es igual al --importe-- 
		*/
		case "importe": {
			this.child("fdbSaldo").setValue(this.iface.calculateField("saldo"));
			break;
		}
		case "idtpv_comanda": {
			this.child("fdbReferencia").setValue(this.iface.calculateField("referencia"));
			break;
		}
	}
}

function oficial_calcularTotales()
{
	var util:FLUtil = new FLUtil;
	var cursor:FLSqlCursor = this.cursor();

	var totalVale:Number = util.sqlSelect("tpv_lineasvale", "SUM(pvptotal)", "refvale = '" + cursor.valueBuffer("referencia") + "'");
	if (!totalVale || isNaN(totalVale))
		totalVale = 0;
	totalVale = util.roundFieldValue(totalVale, "tpv_vales", "importe");

	this.child("fdbImporte").setValue(totalVale);
}

function oficial_encontrarSerial():String
{
	var fis:FLFiscalBixolon;
	var error:Number;
	var status:Number;
	var port:String = "COM1";
	var cmd:String = "S1";
	var temporal = System.getenv( "TMP" );
	if ( temporal.isEmpty() )
		temporal = System.getenv( "TMPDIR" );
	if ( temporal.isEmpty() )
		temporal = System.getenv( "HOME" );
	if ( temporal.isEmpty() )
		temporal = sys.installPrefix() + "/share/facturalux/tmp";

	fis.openPort(port);

	var arch:String = temporal + "\\status.txt";

	fis.uploadStatus(status, error, cmd, arch);

	fis.closedPort();

	var archivo = new File(arch);
	archivo.open( File.ReadOnly );
	var linea:String = archivo.read();
	archivo.close();

	var serial:String = linea.mid(66,10);
	return serial;
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

function oficial_enviarNotaCredito()
{
	var fis:FLFiscalBixolon;
	var error:Number;
	var status:Number;
	var port:String = "COM1";
	var cmd1:String;
	var cmd2:String;
	var cmd3:String;
	var cmd4:String;
	var cmd5:String;
	fis.openPort(port);
	var c1:String = "i01Nombre: " + this.iface.calculateField("nombrecliente");
	if (c1.length < 39){
		cmd1 = this.iface.espaciosDerecha(c1, 39);
	}else if (c1.length > 39){
		cmd1 = c1.mid(0, 39);
	}else {
		cmd1 = c1;
	}

	fis.sendCmd(status, error, cmd1);

	var c2:String = "i02RIF/Ced.: " + this.iface.calculateField("cedrif");
	if (c2.length < 39){
		cmd2 = this.iface.espaciosDerecha(c2, 39);
	}else if (c2.length > 39){
		cmd2 = c2.mid(0, 39);
	}else {
		cmd2 = c2;
	}

	fis.sendCmd(status, error, cmd2);

	var fact:Number = parseFloat(this.iface.calculateField("comanda"));

	var factura:String = this.iface.cerosIzquierda(fact.toString(),8);

	var c3:String = "i03Nro.Fact.Devol:  " + factura;
	if (c3.length < 39){
		cmd3 = this.iface.espaciosDerecha(c3, 39);
	}else if (c3.length > 39){
		cmd3 = c3.mid(0, 39);
	}else {
		cmd3 = c3;
	}

	fis.sendCmd(status, error, cmd3);

	fis.closedPort();

	var c4:String = "i04Nro.Reg.Maquina:  " + this.iface.encontrarSerial();
	if (c4.length < 39){
		cmd4 = this.iface.espaciosDerecha(c4, 39);
	}else if (c4.length > 39){
		cmd4 = c4.mid(0, 39);
	}else {
		cmd4 = c4;
	}
	fis.openPort(port);
	fis.sendCmd(status, error, cmd4);
	var c5:String = "i05Fecha:  " + this.iface.calculateField("fecha") + " Hora: " + this.iface.calculateField("hora");
	if (c5.length < 39){
		cmd5 = this.iface.espaciosDerecha(c5, 39);
	}else if (c5.length > 39){
		cmd5 = c5.mid(0, 39);
	}else {
		cmd5 = c5;
	}
	fis.sendCmd(status, error, cmd5);
	fis.closedPort();
}

function oficial_espaciosDerecha(cad:String, totalCifras:Number):String
{
	var ret:String = cad.toString();
	var numEsp:Number = totalCifras - ret.length;
	for ( ; numEsp > 0 ; --numEsp)
		ret = ret + " ";
		return ret;
}

//// OFICIAL /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition head */
/////////////////////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////

//// DESARROLLO /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
