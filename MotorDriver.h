
/*
Este header file contiene funciones referentes al uso del motorDriver
A es izq y B es der
*/
#define AF 14
#define AB 27
#define BF 26
#define BB 25

void PinMode(){
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
}

void Motor_Stop(){
  digitalWrite(AF, LOW);
  digitalWrite(AB, LOW);
  digitalWrite(BF, LOW);
  digitalWrite(BB, LOW);
}

void Move_Front(){
  digitalWrite(AF, HIGH);
  digitalWrite(AB, LOW);
  digitalWrite(BF, HIGH);
  digitalWrite(BB, LOW);
}

void Move_Back(){
  digitalWrite(AF, LOW);
  digitalWrite(AB, HIGH);
  digitalWrite(BF, LOW);
  digitalWrite(BB, HIGH);
}

void Turn_Right(){
  
  digitalWrite(AF, HIGH);
  digitalWrite(AB, LOW);
  digitalWrite(BF, LOW);
  digitalWrite(BB, LOW);
}

void Turn_Left(){
  
  digitalWrite(AF, LOW);
  digitalWrite(AB, LOW);
  digitalWrite(BF, HIGH);
  digitalWrite(BB, LOW);
}



