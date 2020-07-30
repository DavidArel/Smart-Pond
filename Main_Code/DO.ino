
/*
#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution

//Single-point calibration Mode=0
//Two-point calibration Mode=1
#define TWO_POINT_CALIBRATION 0

#define READ_TEMP (nilaiTempr()) //Current water temperature ℃, Or temperature sensor function


//Single point calibration needs to be filled CAL1_V and CAL1_T
#define CAL1_V (200) //mv
#define CAL1_T (25)   //℃
//Two-point calibration needs to be filled CAL2_V and CAL2_T
//CAL1 High temperature point, CAL2 Low temperature point
#define CAL2_V (1300) //mv
#define CAL2_T (15)   //℃

const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};

uint8_t Temperaturet;
uint16_t ADC_Raw;
uint16_t DO;

int16_t readDO(uint16_t raw, uint8_t temperature)
{
#if TWO_POINT_CALIBRATION == 0
  uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature - (uint32_t)CAL1_T * 35;
  return (uint64_t(VREF) * DO_Table[temperature] * raw) / (uint32_t(ADC_RES) * V_saturation);
#else
  uint16_t V_saturation = (int16_t)((int8_t)temperature - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T)+CAL2_V;
  return (uint64_t(VREF) * DO_Table[temperature] * raw) / (uint32_t(ADC_RES) * V_saturation);
#endif
}

*/
void initDO(){
  //inisialisasi pin yg dipakai
}
int nilaiDO(){
  //koding ngambil nilai ph dari sensor
  //return kedalam bentuk string
  Temperaturet = nilaiTempr();
  ADC_Raw = analogRead(A5); //pin analog utk DOs
  DO = readDO(ADC_Raw, Temperaturet);
  delay(1000);
  return DO/1000;
}
