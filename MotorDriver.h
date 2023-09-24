
#include <ESP32Servo.h>

Servo servoIzq;  // Servo para la rueda izquierda
Servo servoDer;  // Servo para la rueda derecha


void Motor_Stop() {
  int stopPosition = 90;  // Puedes ajustar esto según la posición de "detención" del servo (90 grados es el centro)
  servoIzq.write(stopPosition);
  servoDer.write(stopPosition);
}

void Turn_Left() {
  Motor_Stop();
  servoDer.write(0);  // Gira el servo derecho en sentido horario
}

void Turn_Right() {
  int cwPositionIzq = 0;  // Cambia esto según la posición en la que la rueda izquierda gire en sentido horario
  servoIzq.write(cwPositionIzq);
  int ccwPositionDer = 90;  // Cambia esto según la posición en la que la rueda derecha gire en sentido antihorario
  servoDer.write(ccwPositionDer);
}

void Move_Front() {
  int cwPositionIzq = 0;  // Cambia esto según la posición en la que la rueda izquierda gire en sentido horario
  int cwPositionDer = 0;  // Cambia esto según la posición en la que la rueda derecha gire en sentido horario
  servoIzq.write(cwPositionIzq);
  servoDer.write(cwPositionDer);
}

void Move_Back() {
  Motor_Stop();
  servoIzq.write(0);    // Gira el servo izquierdo en sentido antihorario
  servoDer.write(180);  // Gira el servo derecho en sentido antihorario
}












