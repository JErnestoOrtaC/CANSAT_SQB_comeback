
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
int x = 0;

#include "Main.h"


void setup() {
  

  Begins();
  Sensorcheck();
  datos.Pz = Get_Pz();
  alpha = Aim();
  delay(200);
  
}

void loop() {
  Get_Sensors();
  float v = Aim();
  Serial.print("Coordenadas rover: ");  Serial.print(gps.location.lat(), 7);  Serial.println(gps.location.lng(), 7);
  Serial.print("Distancia: ");  Serial.println( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) );
  Serial.print("teta: ");  Serial.println(Get_heading());
  Serial.print("Alpha: ");  Serial.println( alpha );

  
  ComeBack1();
  
  delay(500);

}


