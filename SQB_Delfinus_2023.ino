
/* 
  ******************************************************************
  *                                                                *
  *             CANSAT TELEMTRY & COMEBACK 2023                    *
  *                       EQUIPO DELPHINUS                         *
  *                            -SQB-
  *       Programa principal de la computadora de vuelo            *
  *                                                                *
  *                                                                *
  ******************************************************************
*/

#include "Main.h"


void setup() {
  

  Begins();
  
  Sensorcheck();
  datos.Pz = Get_Pz();
}

void loop() {
  Get_Sensors();
  SerialDisplay();

  if(bmp.readAltitude(datos.Pz) > 150){
    nrpFlag = 1;
  }
  
  if( nrpFlag ==1 && bmp.readAltitude(datos.Pz) < 20 ){
    Landing();
  }
  
  if(cmbkFlag == 1){
    ComeBack();
  }

  
}


