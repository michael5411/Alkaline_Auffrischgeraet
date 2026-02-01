void messungen(){  //Messungen der vier Batteriespannungen

  for(int k = 0; k < 4; k++){
    for (int i = 0; i < 10; i++) {
      battrawU[k] = analogRead(messPins[k]);
      battrawU[k] += battrawU[k];
      delay(2);
    }
    battU[k] = battrawU[k] * 5.64/2000 *corr[k]; //U-berechnung & Korr.
  }
}