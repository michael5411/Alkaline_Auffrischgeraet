void statusmeldungen(void) {
  for (int s = 0; s < 4; s++) {
    if (battU[s] == 0.0f) {
      strncpy(battstat[s], "Keine Batterie", sizeof(battstat[s]) - 1);
      battstat[s][sizeof(battstat[s]) - 1] = '\0';

    } else if (battU[s] < Ulimit && battU[s] > 0.2f && battload[s] > 0) {
      strncpy(battstat[s], "Frischt auf", sizeof(battstat[s]) - 1);
      battstat[s][sizeof(battstat[s]) - 1] = '\0';

    } else if (battload[s] <= 0) {
      strncpy(battstat[s], "***Beendet***", sizeof(battstat[s]) - 1);
      battstat[s][sizeof(battstat[s]) - 1] = '\0';

    } 
  }
}