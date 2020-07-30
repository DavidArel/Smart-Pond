void draw(void) {
  valDO = nilaiDO();
  valPH = nilaiPH();
  valTEMPR = nilaiTempr();
  char buf1[9];
  char buf2[9];
  char buf3[9];
  char nilTempr = valTEMPR; //"Tempr: " + String(valTEMPR) ;
  char nilPH = valPH; //"DO: " + String(valDO) ;
  char nilDO = valDO; //"pH: " + String(valPH);
  
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.setFont(u8g_font_profont11);
  u8g.drawStr( 10, 10, "Smart Pond");
  //sprintf(buf1, "%d", valTEMPR);
  sprintf(buf1, "Temp: %d.%02d", (int)valTEMPR, (int)(valTEMPR*100)%100);
  sprintf(buf2, "pH: %d.%02d", (int)valPH, (int)(valPH*100)%100);
  //sprintf(buf2, "%f", valPH);
  sprintf(buf3, "DO: %u", valDO);
  u8g.drawStr( 10, 25, buf1);//char s[] = String(valTEMPR));
  u8g.drawStr( 10, 42, buf2);//char t[] = String(valDO));
  u8g.drawStr( 10, 57, buf3);//char u[] = String(valPH));
}

void initOLED(void) {
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void exOLED(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
}
