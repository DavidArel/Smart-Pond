
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
