
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
  alpha = Aim();
  delay(200);
}

void loop() {
  delay(100);
  Get_Sensors();
  float v = Aim();
  Serial.print("Coordenadas rover: ");  Serial.print(gps.location.lat(), 7);  Serial.println(gps.location.lng(), 7);
  Serial.print("Distancia: ");  Serial.println( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) );
  Serial.print("teta: ");  Serial.println(Get_heading());
  Serial.print("Alpha: ");  Serial.println( v );
/*
  if(bmp.readAltitude(datos.Pz) > 150){
    nrpFlag = 1;
  }
  
  if( nrpFlag ==1 && bmp.readAltitude(datos.Pz) < 20 ){
    Landing();
  }
  
  if(cmbkFlag == 1){
    ComeBack1();
  }
  */
  
  //ComeBack1();
  delay(500);

}


