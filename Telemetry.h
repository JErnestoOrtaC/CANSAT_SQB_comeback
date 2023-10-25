
/*
    En este header file se declaran y definen las funciones referentes a la telemetría
    Asi como la construcción de los objetos de cada sensor y estructura de datos
    Se empleo una estructura de datos para guardar los datos de telemetría
*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <TinyGPS++.h>
#include <Adafruit_HMC5883_U.h>
#include <math.h>

const double home_lat =22.3234415;
const double home_long = -97.8788431;
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
  float humedad;
  float tin;
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

Adafruit_MPU6050 mpu;
Adafruit_BME280 bme;
Adafruit_BMP085 bmp;
TinyGPSPlus gps;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


Telemetria datos;
GPSData data;



float Get_Pz(){
  delay(150);
    return ( bmp.readPressure());
}

void Sensorcheck(){
  unsigned status;
  status = bme.begin(0X76);
  if( !status ){
    Serial.println("Error sensor bme280");
    Serial.print("ID: 0x");
    Serial.println( bme.sensorID(), 16 );
  }

  // Inicializar el sensor MPU6050
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
    }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
    }

  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }

  Serial.begin(9600);
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

}

void BMPSensor() {
  datos.temperatura = bmp.readTemperature();
  datos.presion = bmp.readPressure() ;
  datos.altura = bmp.readAltitude(datos.Pz);
}

void BMESensor() {
  datos.temperatura = bme.readTemperature();
  datos.presion = bme.readPressure() ;
  datos.altura = bmp.readAltitude(datos.Pz);
}

void IMU() {
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  datos.ax = a.acceleration.z;
  datos.ay = a.acceleration.x;
  datos.az = a.acceleration.y;

  datos.gx = g.gyro.z;
  datos.gy = g.gyro.x;
  datos.gz = g.gyro.y;

  datos.tin = temp.temperature;
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
  float declinationAngle = .22;
  _heading += declinationAngle;

  // Correct for when signs are reversed.
  if(_heading < 0)
    _heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(_heading > 2*PI)
    _heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  datos.heading = _heading * 180/M_PI; 
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
  Serial.print("Temperatura Ext: ");
  Serial.println(datos.temperatura);

  Serial.println("Temperatura Int: " + String(datos.tin) );

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
  mensaje += "TEMP_EX:" + String(datos.temperatura) + ",";
  mensaje += "TEMP_INT:" + String(datos.tin) + ",";
  mensaje += "PRE:" + String(datos.presion);
  mensaje += "ALT:" + String(datos.altura);
  mensaje += "HUM:" + String(datos.tin) + ",";
  mensaje += "HDG:" + String(datos.heading) + ",";
  mensaje += "Ax:" + String(datos.ax) + ",";
  mensaje += "Ay:" + String(datos.ay) + ",";
  mensaje += "Az:" + String(datos.az) + ",";
  mensaje += "Gx:" + String(datos.gx) + ",";
  mensaje += "Gy:" + String(datos.gy) + ",";
  mensaje += "Gz:" + String(datos.gz) + ",";
  mensaje += "Vx:" + String(datos.vx) + ",";
  mensaje += "Vy:" + String(datos.vy) + ",";
  mensaje += "Vz:" + String(datos.vz) + ",";
  mensaje += "Vt:" + String(datos.vt) + ",";
  mensaje += "Hdg:" + String(datos.heading) + ",";
  mensaje += "LAT:" + String(datos.gpsdata.latitude) + ",";
  mensaje += "LONG:" + String(datos.gps.longitude) + ",";
  mensaje += "Dist:" + String( gps.distanceBetween(gps.location.lat(), gps.location.lng(), home_lat, home_long) );
}
