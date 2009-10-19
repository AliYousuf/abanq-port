/***************************************************************************
                 tpv_lineasvale.qs  -  description
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
	function validateForm():String {
		return this.ctx.interna_validateForm();
	}
	function calculateField(fN:String):String {
		return this.ctx.interna_calculateField(fN);
	}
		/////////////////////////////////////////
	function cerosIzquierda(numero:String, totalCifras:Number):String{
		return this.ctx.oficial_cerosIzquierda(numero, totalCifras);
	}
	function cerosDerecha(numero:String, totalCifras:Number):String{
		return this.ctx.oficial_cerosDerecha(numero, totalCifras);
	}

	function espaciosDerecha(cad:String, totalCifras:Number):String {
		return this.ctx.oficial_espaciosDerecha( cad, totalCifras );
	}
	function enviarLinea() {
		return this.ctx.oficial_enviarLinea( );
	}

	////////////////////////////////////////
}
//// INTERNA /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/** @class_declaration oficial */
//////////////////////////////////////////////////////////////////
//// OFICIAL /////////////////////////////////////////////////////
class oficial extends interna {
	var tdbVentas:Object;
	function oficial( context ) { interna( context ); }
	function bufferChanged(fN:String) {
		return this.ctx.oficial_bufferChanged(fN);
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
/** \C Las l�neas de val permiten introducir art�culos que el cliente devuelve y que, por tanto, pueden volver a integrarse en el stock. El almac�n al que se asociar�n por defecto los art�culos devueltos ser� el asociado al punto de venta actual
*/
function interna_init()
{
	var util:FLUtil = new FLUtil();
	var cursor:FLSqlCursor = this.cursor();

	var idComanda:String = util.sqlSelect("tpv_vales", "idtpv_comanda", "referencia = '" + cursor.valueBuffer("refvale") + "'");
	this.child("fdbIdLinea").setFilter("idtpv_comanda = " + idComanda);
	
	switch (cursor.modeAccess()) {
		case cursor.Insert: {
			var codTerminal:String = util.readSettingEntry("scripts/fltpv_ppal/codTerminal");
			if (codTerminal) {
				this.child("fdbCodAlmacen").setValue(util.sqlSelect("tpv_puntosventa", "codalmacen", "codtpv_puntoventa ='" + codTerminal + "'"));
			}
			break;
		}
		case cursor.Edit: {
			this.child("fdbCodAlmacen").setDisabled(true);
			break;
		}
	}
	
	connect(cursor, "bufferChanged(QString)", this, "iface.bufferChanged");
}

function interna_validateForm():Boolean
{
	var util:FLUtil = new FLUtil();
	var cursor:FLSqlCursor = this.cursor();

	/** \C Se avisa al usuario si la cantidad devuelta es superior a la cantidad de la correspondiente l�nea de venta
	\end */
	var idLinea:String = cursor.valueBuffer("idtpv_linea");
	if (idLinea && idLinea != "") {
		var cantidadLinea:Number = util.sqlSelect("tpv_lineascomanda", "cantidad", "idtpv_linea = " + idLinea);
		if (cantidadLinea) {
			if (parseFloat(cursor.valueBuffer("cantidad")) > parseFloat(cantidadLinea)) {
				var res:Number = MessageBox.warning(util.translate("scripts", "La cantidad devuelta supera la cantidad de la correspondiente l�nea de venta (%1).\n�Desea continuar?").arg(cantidadLinea), MessageBox.No, MessageBox.Yes);
				if (res != MessageBox.Yes)
					return false;
			}
		}
	}
	this.iface.enviarLinea();
	
	return true;
}

function interna_calculateField(fN:String):String
{
	var valor:String;
	var util:FLUtil = new FLUtil;
	var cursor:FLSqlCursor = this.cursor();
	switch (fN) {
		case "pvptotal": {
			var idLinea:String = cursor.valueBuffer("idtpv_linea");
			var porIva:Number = util.sqlSelect("tpv_lineascomanda", "iva", "idtpv_linea = " + idLinea);
			if (!porIva || isNaN(porIva))
				porIva = 0;
			var netoTotal:Number = util.sqlSelect("tpv_lineascomanda", "pvptotal", "idtpv_linea = " + idLinea);
			if (!netoTotal || isNaN(netoTotal))
				netoTotal = 0;
			valor = parseFloat(netoTotal) * (100 + parseFloat(porIva)) / 100;
			valor = util.roundFieldValue(valor, "tpv_lineasvale", "pvptotal");
			break;
		}
		case "pvpunitario": {
			var idLinea:String = cursor.valueBuffer("idtpv_linea");

			var porIva:Number = util.sqlSelect("tpv_lineascomanda", "pvpunitario", "idtpv_linea = " + idLinea);
			valor = porIva.toString();
			
			break;
		}
		case "iva": {
			
			var idLinea:String = cursor.valueBuffer("idtpv_linea");

			valor = util.sqlSelect("tpv_lineascomanda", "codimpuesto", "idtpv_linea = " + idLinea);
			
			break;
			
		}
		case "descripcion": {

			var idLinea:String = cursor.valueBuffer("idtpv_linea");

			valor = util.sqlSelect("tpv_lineascomanda", "descripcion", "idtpv_linea = " + idLinea);

			break;
		}

		case "cantidad": {

			var idLinea:String = cursor.valueBuffer("idtpv_linea");

			var porIva:Number = util.sqlSelect("tpv_lineascomanda", "cantidad", "idtpv_linea = " + idLinea);
			valor = porIva.toString();
			
			break;
		}
		
	}
	return valor;
}

/** @class_definition oficial */
/////////////////////////////////////////////////////////////////
//// OFICIAL ////////////////////////////////////////////////////
function oficial_bufferChanged(fN:String)	
{
	var cursor:FLSqlCursor = this.cursor();
	switch (fN) {
		case "idtpv_linea": {
			this.child("fdbPvpTotal").setValue(this.iface.calculateField("pvptotal"));
			break;
		}

	}
}

function oficial_enviarLinea()
{
	var fis:FLFiscalBixolon;
	var error:Number;
	var status:Number;
	var port:String = "COM1";
	var imp:String;
	var cn:Number;
	var price:Number;
	var cante:String;
	var cantd:String;
	var prie:String;
	var prid:String;
	var cmd:String;

	fis.openPort(port);
	
	if(this.iface.calculateField("iva") == "IVAE")
		imp = "0";
	else if(this.iface.calculateField("iva") == "IVA12") 
		imp = "1";
	else if(this.iface.calculateField("iva") == "TASA2") 
		imp = "2";
	else if(this.iface.calculateField("iva") == "TASA3") 
		imp = "3";

	var desc:String = this.iface.calculateField("descripcion");
	var descrip:String;

	if(this.iface.calculateField("iva") == "IVAE")
		desc = desc + " (E)";
	else 
		desc = desc + " (G)";

	if (desc.length < 39){
		descrip = this.iface.espaciosDerecha(desc, 39);
	}else if (desc.length > 39){
		descrip = desc.mid(0, 39);
	}else {
		descrip = desc;
	}

	
	cn = parseFloat(this.iface.calculateField("cantidad"));

	price = parseFloat(this.iface.calculateField("pvpunitario"));

	var tmpC:String = cn.toString();

	var tmpP:String = price.toString();

	var posCn:Number = tmpC.search(".");

	var posPri:Number = tmpP.search(".");

	if (posCn >= 0){

		
		cante =  this.iface.cerosIzquierda(tmpC.mid(0 , posCn), 5);

		cantd =  this.iface.cerosDerecha(tmpC.mid(posCn+1, 3), 3);
	}
	else{
		
		cante =  this.iface.cerosIzquierda(tmpC, 5);
		cantd =  this.iface.cerosDerecha(0, 3);

	}

	if (posPri >= 0){

		prie =  this.iface.cerosIzquierda(tmpP.mid(0 , posPri), 8);
		prid =  this.iface.cerosDerecha(tmpP.mid(posPri+1, 2), 2);
	}
	else{
		
		prie =  this.iface.cerosIzquierda(tmpP, 8);
		prid =  this.iface.cerosDerecha(0, 2);

	}
	
	cmd = "d" + imp + prie + prid + cante + cantd + descrip;
	fis.sendCmd(status, error, cmd);
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
