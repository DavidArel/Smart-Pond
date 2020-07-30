
void initTempr(){
  //inisialisasi pin yg dipakai
}
float nilaiTempr(){
  sensors.requestTemperatures(); 
  float Tempr = sensors.getTempCByIndex(0);
  //koding ngambil nilai ph dari sensor
  //return kedalam bentuk string
  //String Tempr = "888";
  return Tempr;
}


