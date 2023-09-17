
ejemplo de git

Este es el proyecto SQB del equipo delfinus para competir en los CNASAT.
En este repositorio se tiene el programa de la compuatdora de vuelo para el proyecto "Telemetry" y "Comeback"

Para este proeycto se usa el software ARDUINO IDE y un DOIT esp32 Dev KIT V1.

//Componenetes:
	MPU6050
	BME280
	GPS NODE
	LORA
	HMC5883L
	DOIT DEV KIT V1

//La carperta cuenta con los siguientes archivos:
	README.txt
	SQB_Delfinus_2023.ino
	Main.h
	Compas.h
	Lora.h
	telemetry.h

//Recomendaciones:


//README.txt :
	En este archivo se busca proporconar una descripción del proyecto, las metodologias, las herramientas que se usaron,
	las decisiones que se tomaron y los problemas que se presnetaron en la realización del proyecto.

	Cualquier instruccion o recomendacion respecto a los sensores usados, las librerias consultadas o los algoritmos implemnetados la encontaras
	en este documento.

//SQB_Delfinus_2023.ino :
	El archivo .ino es un archivo creado en el ARDUINO IDE es el programa que se compilara y subira a la tarjeta.

	Se busco mantener este documento lo mas simple sencillo y minimalista posible de tal forma que se pueda enternder el daigrama de flujo
	que se lleva en la inicilizacion y en el bucle principal.
	De esta forma solo se ven los llamados a las funciones que realizan las acciones, mediciones o decisiones que se encuentran en los 
	archivos de encabezados.

	Este metodo resulta ser ventajoso cuando se esta probando una cierta funcion de todo el proyecto, ya que por ejemplo si no necesito que
	se realizen las funciones de telemetria o comuncacion puedo simplemente comentar la funcion en donde se realiza sin necesidad de borrar
	o comentar mas de una linea.

	Ademas si es muy apreciable el orden que lleva el proyecto que una persona externa puede apreciar y comprender el flujo de trabajo que tiene
	el proyecto.

	Sin embargo una desventaja es que al momento de hacer un debbuging el llamado a tantas funciones y el encapsulamiento puede ser
	dificil de entender para personas no familiarizadas con las funciones.