// Definición de los pines para MotorA
const int motorAPin1 = 13;
const int motorAPin2 = 12;

// Definición de los pines para MotorB
const int motorBPin1 = 14;
const int motorBPin2 = 27;

void motorPinSetup(){
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(motorBPin1, OUTPUT);
  pinMode(motorBPin2, OUTPUT);
}

void Turn_Left() {
  // Girar a la izquierda (MotorA hacia adelante, MotorB hacia atrás)
  digitalWrite(motorAPin1, HIGH);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin1, LOW);
  digitalWrite(motorBPin2, HIGH);
}

void Turn_Right() {
  // Girar a la derecha (MotorA hacia atrás, MotorB hacia adelante)
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, HIGH);
  digitalWrite(motorBPin1, HIGH);
  digitalWrite(motorBPin2, LOW);
}

void Move_Front() {
  // Avanzar (ambos motores hacia adelante)
  digitalWrite(motorAPin1, HIGH);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin1, HIGH);
  digitalWrite(motorBPin2, LOW);
}

void Move_Back() {
  // Retroceder (ambos motores hacia atrás)
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, HIGH);
  digitalWrite(motorBPin1, LOW);
  digitalWrite(motorBPin2, HIGH);
}

void Motor_Stop() {
  // Detener ambos motores
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin1, LOW);
  digitalWrite(motorBPin2, LOW);
}
