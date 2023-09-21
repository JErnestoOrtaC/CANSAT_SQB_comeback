
/*
    En este header file se declaran y definen las funciones referentes a la telemetría
    Asi como la construcción de los objetos de cada sensor y estructura de datos
    Se empleo una estructura de datos para guardar los datos de telemetría
*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <MPU6050.h>
#include <TinyGPS++.h>
#include <Adafruit_HMC5883_U.h>
#include <math.h>

const double home_lat =22.2558886;
const double home_long = -97.8582055;
const double tolerancia_latitud = 0.0002; // Rango de tolerancia de 1 metro en latitud
#define EARTH_RADIUS 6371.0

struct GPSData {
  double latitude;
  double longitude;
  double altitude;
};

struct Telemetria {
  float Pz=0;
  float temperatura;
  float presion;
  float altura;
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
  float vx, vy, vz, vt;
  float heading;
  GPSData gpsData;  // Agrega el miembro de tipo GPSData
};

String mensaje;

Adafruit_BMP085 bmp;
MPU6050 mpu;
TinyGPSPlus gps;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


Telemetria datos;
GPSData data;



float Get_Pz(){
  long double x=0;
  for(int i=0; i<31;i++){
    x += bmp.readPressure();
  }
    return (x/30);
}

void Sensorcheck(){
  if (!bmp.begin()) {
    Serial.println("No se pudo encontrar el sensor BMP180. Verifica las conexiones.");
    while (1);
  }

  // Inicializar el sensor MPU6050
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("No se pudo encontrar el sensor MPU6050. Verifica las conexiones.");
    while (1);
  }

  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
}

void BMPSensor() {
  datos.temperatura = bmp.readTemperature();
  datos.presion = bmp.readPressure();
  datos.altura = bmp.readAltitude(datos.Pz);
}

void IMU() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  datos.ax = az;
  datos.ay = ax;
  datos.az = ay;

  datos.gx = gx;
  datos.gy = gy;
  datos.gz = gz;
}

void getGPSData() {
  // Leer los datos disponibles desde el módulo GPS
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }
    // Obtener la latitud, longitud y altitud
    datos.gpsData.latitude = gps.location.lat();
    datos.gpsData.longitude = gps.location.lng();
}

void Get_vt() {
  // Integración de la aceleración lineal para obtener la velocidad
  datos.vx += (datos.ax - datos.gx) *3;
  datos.vy += (datos.ay - datos.gy)*3;
  datos.vz += (datos.az - datos.gz)*3;

  datos.vt = sqrt(datos.vx * datos.vx + datos.vy * datos.vy + datos.vz * datos.vz);

}

float Get_heading(){
  sensors_event_t event; 
  mag.getEvent(&event);
  float _heading = atan2(event.magnetic.y, event.magnetic.x);
    // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/  
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = .20;
  _heading += declinationAngle;

  // Correct for when signs are reversed.
  if(_heading < 0)
    _heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(_heading > 2*PI)
    _heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  return _heading * 180/M_PI; 
}

void Get_Sensors(){
  getGPSData();
  BMPSensor();
  IMU();
  Get_vt();
  datos.heading = Get_heading();
}

void SerialDisplay(){
  Serial.print("Temperatura: ");
  Serial.println(datos.temperatura);

  Serial.print("PZ:");
  Serial.println(datos.Pz);

  Serial.print("Presión: ");
  Serial.println(datos.presion);

  Serial.print("Altitud: ");
  Serial.println(datos.altura);

  Serial.print("Aceleración (x, y, z): ");
  Serial.print(datos.ax);
  Serial.print(", ");
  Serial.print(datos.ay);
  Serial.print(", ");
  Serial.println(datos.az);

  Serial.print("Giroscopio (x, y, z): ");
  Serial.print(datos.gx);
  Serial.print(", ");
  Serial.print(datos.gy);
  Serial.print(", ");
  Serial.println(datos.gz);

  Serial.print("Velocidad (x, y, z, total): ");
  Serial.print(datos.vx);
  Serial.print(", ");
  Serial.print(datos.vy);
  Serial.print(", ");
  Serial.print(datos.vz);
  Serial.print(", ");
  Serial.println(datos.vt);

  Serial.print("Datos GPS: ");
  Serial.print("Latitud: ");
  Serial.print(datos.gpsData.latitude,6 );
  Serial.print(", Longitud: ");
  Serial.println(datos.gpsData.longitude,6);

  Serial.print("Heading: ");
  Serial.println(datos.heading);
}

void PacageTelemetry(){
  mensaje = "";
  // Realiza lo mismo para cada variable, por ejemplo:
  mensaje += "Temperatura: " + String(datos.temperatura);
  mensaje += "Presión: " + String(datos.presion);
  mensaje += "Altura: " + String(datos.altura);
  mensaje += "Ax: " + String(datos.ax);
  mensaje += "Ay: " + String(datos.ay);
  mensaje += "Az: " + String(datos.az);
  mensaje += "Gx: " + String(datos.gx);
  mensaje += "Gy: " + String(datos.gy);
  mensaje += "Gz: " + String(datos.gz);
  mensaje += "Vx: " + String(datos.vx);
  mensaje += "Vy: " + String(datos.vy);
  mensaje += "Vz: " + String(datos.vz);
  mensaje += "Vt: " + String(datos.vt);
  mensaje += "Hdg:" + String(datos.heading);
  mensaje += "Dist: " + String( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) );
}






