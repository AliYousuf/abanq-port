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
	function enviarBixolon():Boolean {
		return this.ctx.oficial_enviarBixolon();
	}
	function cerosIzquierda() {
		return this.ctx.oficial_cerosIzquierda();
	}
	function cerosDerecha() {
		return this.ctx.oficial_cerosDerecha();
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
/** \C Las líneas de val permiten introducir artículos que el cliente devuelve y que, por tanto, pueden volver a integrarse en el stock. El almacén al que se asociarán por defecto los artículos devueltos será el asociado al punto de venta actual
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

	this.iface.enviarBixolon();
}

function oficial_enviarBixolon():Boolean
{
		/* Pedazo de Software para enviar datos a la impresora*/
		   /* Variable para Instanciar la clase FLFiscalBixolon*/
			
		   	var cursor:FLSqlCursor = this.cursor();
		   	var fis:FLFiscalBixolon;
		  	var num:FLUtilInterface; 
		   	var imp:String;
		   	var cn:Double;
		   	var price:Double;
		   	var descrip:String;
		   	var desc:String;
		   	var cne:Number;
			var cnd:Number;
			var pricee:Number;
			var priced:Number;
			var cante:String;
			var cantd:String;
			var prie:String;
			var prid:String;
			var comando:String;
			var status:Number; 
		        var error:Number;
		        var comando:String;

			if(!fis.OpenPort("COM1")){
				return false;
			}
			if(this.iface.calculateField("iva") == 0){
				imp = " ";
			}
			else { 
				imp = "!";
			}
			
			cn = parseFloat(cursor.valueBuffer("cantidad"));
			price = parseFloat(this.iface.calculateField("pvptotal"));
			descrip = this.iface.calculateField("descripcion");

			if(descrip.length == 40)
				desc = descrip;
			else if(descrip.length < 40)
				desc = this.iface.espaciosDerecha(descrip, 40); 
			else if (descrip.length > 40)
				desc = descrip.mid(0,40);
				
			cne = num.partInteger(cn);
			cnd = num.partDecimal(cn);
			pricee = num.partInteger(price);
			priced = num.partDecimal(price);
			cante =  this.iface.cerosIzquierda(cne, 8);
			cantd =  this.iface.cerosDerecha(cnd, 3);
			prie =  this.iface.cerosIzquierda(pricee, 8);
			prid =  this.iface.cerosDerecha(priced, 2);
			
			comando = "d" + imp + prie + prid + cante + cantd + desc;
			
			if (fis.SendCmd(status, error, comando))
				return true;
			else
				return false;
		/* Hasta Aquí*/

	
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

function oficial_espaciosDerecha(cad:String, totalCifras:Number):String
{
				var ret:String = cad.toString();
				var numEsp:Number = totalCifras - ret.length;
				for ( ; numEsp > 0 ; --numEsp)
					ret = ret + " ";
				return ret;
}





function interna_validateForm():Boolean
{
	var util:FLUtil = new FLUtil();
	var cursor:FLSqlCursor = this.cursor();

	/** \C Se avisa al usuario si la cantidad devuelta es superior a la cantidad de la correspondiente línea de venta
	\end */
	var idLinea:String = cursor.valueBuffer("idtpv_linea");
	if (idLinea && idLinea != "") {
		var cantidadLinea:Number = util.sqlSelect("tpv_lineascomanda", "cantidad", "idtpv_linea = " + idLinea);
		if (cantidadLinea) {
			if (parseFloat(cursor.valueBuffer("cantidad")) > parseFloat(cantidadLinea)) {
				var res:Number = MessageBox.warning(util.translate("scripts", "La cantidad devuelta supera la cantidad de la correspondiente línea de venta (%1).\n¿Desea continuar?").arg(cantidadLinea), MessageBox.No, MessageBox.Yes);
				if (res != MessageBox.Yes)
					return false;
			}
		}
	}
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

		case "iva": {
			var idLinea:String = cursor.valueBuffer("idtpv_linea");
			var valor:Number = util.sqlSelect("tpv_lineascomanda", "iva", "idtpv_linea = " + idLinea);
			if (!porIva || isNaN(porIva))
				porIva = 0;
			break;
		}
		case "descripcion": {
			var idLinea:String = cursor.valueBuffer("idtpv_linea");
			var valor:Number = util.sqlSelect("tpv_lineascomanda", "descripcion", "idtpv_linea = " + idLinea);
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
//// OFICIAL /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/** @class_definition head */
/////////////////////////////////////////////////////////////////
//// DESARROLLO /////////////////////////////////////////////////

//// DESARROLLO /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
