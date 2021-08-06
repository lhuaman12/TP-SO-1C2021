# -*- MakeFile -*-
 
discordiador_path := Discordiador/Debug
iMongoStore_path := I-MongoStore/Debug
MiRamHQ_path := Mi-RamHQ/Debug
 
all: default
 
default: clean general
 
.PHONY: general
.SILENT: general
general:
	make -s d r m
	echo "\nMi primer compilacion del proyecto completo <3"
.PHONY: d
.SILENT: d
d:
	echo "\nSe inicia compilacion del Discordiador"
	make -sC $(discordiador_path)
	echo "Se compilo correctamente el Discordiador"
.PHONY: r
.SILENT: r
r:
	echo "\nSe inicia compilacion de Mi-RAM HQ"
	make -sC $(MiRamHQ_path) all
	echo "Se compilo correctamente Mi-RAM HQ"
.PHONY: m
.SILENT: m
m:
	echo "\nSe inicia compilacion del i-MongoStore"
	make -sC $(iMongoStore_path)
	echo "Se compilo correctamente el i-MongoStore"

.SILENT: clean
.PHONY: clean
clean:
	make -sC $(discordiador_path) clean
	make -sC $(MiRamHQ_path) clean
	make -sC $(iMongoStore_path) clean