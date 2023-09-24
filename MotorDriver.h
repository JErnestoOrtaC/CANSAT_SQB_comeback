
#include <ESP32Servo.h>

Servo servoIzq;  // Servo para la rueda izquierda
Servo servoDer;  // Servo para la rueda derecha

int servoSpeed = 100;  // Velocidad deseada en grados por segundo

void Motor_Stop() {
  int stopPosition = 90;  // Puedes ajustar esto según la posición de "detención" del servo (90 grados es el centro)
  servoIzq.write(stopPosition);
  servoDer.write(stopPosition);
}

void Turn_Left() {
  int servoSpeed = 20;  // Velocidad deseada en grados por segundo

  unsigned long currentTime = millis();
  static unsigned long lastMoveTime = 0;
  
  // Calcula el tiempo transcurrido desde la última llamada
  unsigned long deltaTime = currentTime - lastMoveTime;
  
  // Calcula la cantidad de grados a mover en función de la velocidad
  float degreesToMove = (servoSpeed / 1000.0) * deltaTime;
  
  // Actualiza la posición del servo izquierdo
  int cwPositionIzq = 0;  // Gira el servo izquierdo en sentido horario
  servoIzq.write(cwPositionIzq);
  
  // Actualiza el tiempo de la última llamada
  lastMoveTime = currentTime;
}

void Turn_Right() {
  int servoSpeed = 20;

  unsigned long currentTime = millis();
  static unsigned long lastMoveTime = 0;
  
  // Calcula el tiempo transcurrido desde la última llamada
  unsigned long deltaTime = currentTime - lastMoveTime;
  
  // Calcula la cantidad de grados a mover en función de la velocidad
  float degreesToMove = (servoSpeed / 1000.0) * deltaTime;
  
  // Actualiza la posición del servo derecho
  int ccwPositionDer = 0;  // Gira el servo derecho en sentido horario
  servoDer.write(ccwPositionDer);
  
  // Actualiza el tiempo de la última llamada
  lastMoveTime = currentTime;
}

void Move_Front() {
  int servoSpeed = 100;  // Velocidad deseada en grados por segundo
  unsigned long currentTime = millis();
  static unsigned long lastMoveTime = 0;
  
  // Calcula el tiempo transcurrido desde la última llamada
  unsigned long deltaTime = currentTime - lastMoveTime;
  
  // Calcula la cantidad de grados a mover en función de la velocidad
  float degreesToMove = (servoSpeed / 1000.0) * deltaTime;
  
  // Actualiza la posición de los servos
  int cwPositionIzq = 0;  // Gira el servo izquierdo en sentido horario
  int cwPositionDer = 0;  // Gira el servo derecho en sentido horario
  servoIzq.write(cwPositionIzq);
  servoDer.write(cwPositionDer);
  
  // Actualiza el tiempo de la última llamada
  lastMoveTime = currentTime;
}

void Move_Back() {
  servoIzq.write(180);    // Gira el servo izquierdo en sentido antihorario
  servoDer.write(180);  // Gira el servo derecho en sentido antihorario
}












