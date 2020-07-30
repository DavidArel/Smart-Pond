


void initPH(){
  //inisialisasi pin yg dipakai
}
float nilaiPH(){ 
    /*static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U)  //time interval: 1s
    {
      timepoint = millis();
      voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
      //temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
      phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
      //Serial.print("temperature:");
      //Serial.print(temperature,1);
      //Serial.print("^C  pH:");
      //Serial.println(phValue,2);
    }
    ph.calibration(voltage,temperature);  // calibration process by Serail CMD
    */
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float phValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      phValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  
 float PH = phValue;
  return PH;
}

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}