#!/bin/bash

cd qt
. myconfigure
cd ..

for i in  advance \
barcode \
dbf \
flbase \
flmail \
kudesigner \
kugar \
libmysql \
libpq \
lrelease \
plugins \
qsa \
sqlite \
teddy 
do
	cd $i
	. ../limpiar
	cd ..
done

