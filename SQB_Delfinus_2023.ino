
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
  Serial.print("teta: ");
  Serial.println(Get_heading());
  Serial.print("Coordenadas rover: ");
  Serial.print(gps.location.lat());
  Serial.println(gps.location.lng());

  if(bmp.readAltitude(datos.Pz) > 150){
    nrpFlag = 1;
  }
  
  if( nrpFlag ==1 && bmp.readAltitude(datos.Pz) < 20 ){
    Landing();
  }
  
  if(cmbkFlag == 1){
    ComeBack1();
  }
  Serial.print("Distancia: ");
  Serial.println( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) );
  ComeBack1();
  Aim();
  delay(500);

}


