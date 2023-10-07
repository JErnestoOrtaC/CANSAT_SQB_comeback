/*
    En este header file se declaran y definen los sistemas principales y de iniciación del programa principal
*/
#include "Telemetry.h"
#include "Lora.h"
#include "Compas.h"
#include "MotorDriver.h"

bool nrpFlag=0;
bool cmbkFlag =0;
double Q = 1000000000;
float alpha;


void Begins(){
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(motorBPin1, OUTPUT);
  pinMode(motorBPin2, OUTPUT);
  Motor_Stop();

  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, 4, 32);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  Wire.begin();
  sensor_t sensor;

  delay(150);
  LoraConfig();
}

void EnviarInfo(){
  SerialDisplay();
  LoraSend(mensaje);
}


void Landing(){
  //Abre el paracaidas
  if( bmp.readAltitude(datos.Pz) < 1 ){
    nrpFlag =0; //Reseteamos la nrpflag para que no abre y corte el paracaidas cuado ya este en el suelo
    cmbkFlag =1; //Activamos el sistema de comeback
    //Separa el paracaidas
  }
}

float Aim(){
  double y = gps.distanceBetween(gps.location.lat(), home_long, home_lat, home_long);
  double x = gps.distanceBetween(home_lat, gps.location.lng(), home_lat, home_long);
  
  //El obj esta en el sur.
  if( gps.location.lat() > home_lat ){
    y *= -1;
  }
  //El obj esta al oeste
  if( gps.location.lng() > home_long ){
    x *= -1;
  }

  // Asegúrate de que x y y sean positivos
  /*
  if (x < 0) {
    x = -x;
  }
  if (y < 0) {
    y = -y;
  }
  */
  // Calcular el ángulo de rumbo en radianes
  float alpha = atan2(x, y);

  // Convertir el ángulo de radianes a grados
  alpha = alpha * 180.0 / PI;

  // Asegúrate de que el ángulo esté en el rango de 0 a 360 grados
  if (alpha < 0) {
    alpha += 360.0;
  }
  

  
  return alpha;

}

void ComeBack1(){
  if( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) <= 5){
    Serial.println("Llegaste al destino");
    Motor_Stop();
    LoraSend("LLegaste al destino");
  }
  else{
    if( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) <= Q ){
      Serial.println("Apuntando");
      Motor_Stop();
      alpha = Aim();
      while (fabs(Get_heading() - alpha) > 5) {
        Serial.print("Teta: "); 
        Serial.println(Get_heading());
        Serial.print("Alpha: ");  
        Serial.println(alpha);

        float angleDifference = Get_heading() - alpha;

        // Ajusta la diferencia angular para que esté en el rango [-180, 180]
        if (angleDifference > 180) {
          angleDifference -= 360;
        } else if (angleDifference < -180) {
          angleDifference += 360;
        }

        if (angleDifference > 0) {
          // Gira a la derecha
          Turn_Right();
          Serial.println("<<<<<---------- Girando a la izq -<<<-----");
        } else {
          // Gira a la izquierda
          Turn_Left();
          Serial.println("----->>> Girando a la DERECHA -->>>>>>>>");
        }
        delay(200);
      }

      if( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) > 1){
        Q = gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) / 1.75 ;
      }
    }
    else{
    Serial.println("Movindome");
    Move_Front();
    }
  }
}

