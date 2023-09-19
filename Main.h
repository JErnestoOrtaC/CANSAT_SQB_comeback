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


void Begins(){

  PinMode();
  Motor_Stop();


  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, 32, 4);
  Wire.begin();
  Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
  sensor_t sensor;
  delay(150);
  LoraConfig();
}

void EnviarInfo(){
  SerialDisplay();
  LoraSend(mensaje);
}

void ComeBackMc(){
 
  //Avanza al norte para igualar la latitud  
  if (fabs(home_lat - datos.gpsData.latitude) > tolerancia_latitud) { //CHECAMOS QUE NO ESTEMOS YA EN LA MISMA LATITUD DENTRO DEL MARGEN DE ERROR
    //Apunta al norte
    while( !Head_North( Get_heading() ) ){ //CHECAMOS SI ESTAMOS APUNTANDO AL NORTE
      Serial.print("Hedaing: "); Serial.println( Get_heading() );

      if( Get_heading() < 180){
      Serial.println("Gira a la IZQUIERDA");
      LoraSend(",Msj: Girando");
      Turn_Left();
      }

      else{
      Serial.println("Gira a la DERECHA");
      LoraSend("Girando a la DERECHA para apuntar al norte");

      Turn_Right();
      }
    }
    Motor_Stop();

    //YA ESTAMOS APUNTANDO AL NORTE
    Serial.println("\nApuntando al norte");
    LoraSend("Apuntando al norte");

    if (home_lat < datos.gpsData.latitude) {
      // El carro se encuentra al sur de "home"
      Serial.println("Camina atras");
      LoraSend("Avanzando al sur");

      //MOVEMOS LOS 2 MOTORES HACIA ATRAS
      Move_Back();
    }
    else {
      // El carro se encuentra al norte de "home"
      Serial.println("Camina enfrente");
      LoraSend("Avanzando al norte");

      //MOVEMOS LOS 2 MOTORES HACIA ENFRENTE
      Move_Front();
    }

  }
  else{// YA ESTAMOS EN LA MISMA LATITUD
    Motor_Stop();

    //Igualaste la latitud
    Serial.println("Misma latitud");
    LoraSend("Misma latitud");

    if (fabs(home_lat - datos.gpsData.latitude) > tolerancia_latitud) { //CHECAMOS QUE NO ESTEMOS DENTRO DE LA MISMA LONGITUD
        //Apunta al este
      while( !Head_East( Get_heading() ) ){ 
        Serial.print("Hedaing: "); Serial.println( Get_heading() );
        if( Get_heading() > 90 && Get_heading() < 270  ){//GIRAMOS A LA IZQUIERDA UN MOTOR DESPACIO Y FRENAMOS EL OTRO
        Serial.println("Gira a la IZQUIERDA");
        LoraSend("Girando a la IZQUIERDA para apuntar al norte");

        Turn_Left();
        }
        else{//GIRAMOS A LA DERECHA UN MOTOR DESPACIO Y FRENAMOS EL OTRO
        Serial.println("Gira a la DERECHA");
        LoraSend("Girando a la DERECHA para apuntar al norte");

        Turn_Right();
        }
      }
      Motor_Stop();
      
      if (home_long < datos.gpsData.longitude) {
        // El carro se encuentra al oeste de "home"
        Serial.println("Camina atars");
        LoraSend("Avanzando al oeste");

        Move_Back();

      } else {
        // El carro se encuentra al este de "home"
        Serial.println("Camina deferente");
        LoraSend("Avanzando al este");

        Move_Front();
      }
    }
    else{
      Motor_Stop();
      while(1){
        Serial.println("\n!!!!!!!!!LLEGASTE!!!!!!!!!!!!!!!!");
        Motor_Stop();
      }
    }
  }
}

void Landing(){
  //Abre el paracaidas
  if( bmp.readAltitude(datos.Pz) < 1 ){
    nrpFlag =0; //Reseteamos la nrpflag para que no abre y corte el paracaidas cuado ya este en el suelo
    cmbkFlag =1; //Activamos el sistema de comeback
    //Separa el paracaidas
  }
}

bool Arrived(){
  if( fabs(home_lat - datos.gpsData.latitude) <= tolerancia_latitud ){
    if( fabs(home_lat - datos.gpsData.latitude) <= tolerancia_latitud ){
      return true;
    }
    else{
      return false;
    }
  }
  else{
    return false;
  }
  
}

void Aim(){
  double y = gps.distanceBetween(gps.location.lat(), home_long, home_lat, home_long);
  double x = gps.distanceBetween(home_lat, gps.location.lng(), home_lat, home_long);

  // Asegúrate de que x y y sean positivos
  if (x < 0) {
    x = -x;
  }
  if (y < 0) {
    y = -y;
  }

  // Calcular el ángulo de rumbo en radianes
  float alpha = atan2(x, y);

  // Convertir el ángulo de radianes a grados
  alpha = alpha * 180.0 / PI;

  // Asegúrate de que el ángulo esté en el rango de 0 a 360 grados
  if (alpha < 0) {
    alpha += 360.0;
  }
  Serial.print("alpha: ");
  Serial.println(alpha);

}

void ComeBack1(){
  Serial.print("Q: ");
  Serial.println(Q);
  if( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) <= 5){
    Serial.println("Llegaste al destino");
    Motor_Stop();
    LoraSend("LLegaste al destino");
  }
  
  if( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) <= Q ){
    Serial.println("Apuntando");
    Motor_Stop();
    Aim();
    if( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) > 1){
      Q = gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) / 3 ;
    }
  }else{
    Serial.println("Movindome");
    Move_Front();
  }
}

