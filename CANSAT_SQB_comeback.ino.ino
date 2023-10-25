
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
  PacageTelemetry();
  SerialDisplay();
  LoraSend(mensaje);  
  delay(500);

}


