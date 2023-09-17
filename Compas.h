
/*
Este header file contiene las funciones referentes a la orientacion del sistema con respecto al Norte
*/

const double home_lat =22.256297;
const double home_long = -97.869245;
const double tolerancia_latitud = 0.0003; // Rango de tolerancia de 1 metro en latitud


bool Head_North(float teta){
  if ((teta >= 354 && teta <= 360) || (teta >= 0 && teta <= 6)){
    return true;
  }
  else{
    return false;
  }
}

bool Head_East(float teta) {
  if (teta >= 80 && teta <= 100) {
    return true;
  } else {
    return false;
  }
}
