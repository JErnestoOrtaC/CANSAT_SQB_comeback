/*
    En este header file se declaran y definen los sistemas principales y de iniciación del programa principal
*/
#include "Telemetry.h"
#include "Lora.h"
#include "Compas.h"
#include "MotorDriver.h"

bool nrpFlag=0;
bool cmbkFlag =0;


void Begins(){

  PinMode();
  Motor_Stop();


  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, 32, 4);
  Wire.begin();
  delay(150);
  LoraConfig();
}

void EnviarInfo(){
  SerialDisplay();
  LoraSend(mensaje);
}

void ComeBack(){
 
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

