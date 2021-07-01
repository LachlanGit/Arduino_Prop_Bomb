//Sets up and reads values from EEPROM
void setupEEPROM() {
  //EEPROM settings
  EEPROM.setMemPool(0, EEPROMSizeMega);
  EEPROM.setMaxAllowedWrites(50);

  //Address assignment
  addStateMain = EEPROM.getAddress(sizeof(stateMain));
  addVolume = EEPROM.getAddress(sizeof(volume));
  addSndPrimingTime = EEPROM.getAddress(sizeof(sndPrimingTime));
  addSndCodeLength = EEPROM.getAddress(sizeof(sndCodeLength));
  addSndCodeDisplayTime = EEPROM.getAddress(sizeof(sndCodeDisplayTime));
  addSndCountdownTime = EEPROM.getAddress(sizeof(sndCountdownTime));
  addSndPenaltyMode = EEPROM.getAddress(sizeof(sndPenaltyMode));
  addSndPenaltyApplied = EEPROM.getAddress(sizeof(sndPenaltyApplied));
  addDomPrimingTime = EEPROM.getAddress(sizeof(domPrimingTime));
  addDomCountdownTime = EEPROM.getAddress(sizeof(domCountdownTime));


  //Reset to defaults if button combo is held
  if (!digitalRead(RBUTTON) && !digitalRead(GBUTTON)) {
    EEPROM.updateByte(addStateMain, stateMain);
    EEPROM.updateByte(addVolume, volume);
    EEPROM.updateLong(addSndPrimingTime, sndPrimingTime);
    EEPROM.updateByte(addSndCodeLength, sndCodeLength);
    EEPROM.updateInt(addSndCodeDisplayTime, sndCodeDisplayTime);
    EEPROM.updateLong(addSndCountdownTime, sndCountdownTime);
    EEPROM.updateByte(addSndPenaltyMode, sndPenaltyMode);
    EEPROM.updateLong(addSndPenaltyApplied, sndPenaltyApplied);
    EEPROM.updateLong(addDomPrimingTime, domPrimingTime);
    EEPROM.updateLong(addDomCountdownTime, domCountdownTime);
  }


  //Read and set values from EEPROM
  stateMain = EEPROM.readByte(addStateMain);
  volume = EEPROM.readByte(addVolume);
  sndPrimingTime = EEPROM.readLong(addSndPrimingTime);
  sndCodeLength = EEPROM.readByte(addSndCodeLength);
  sndCodeDisplayTime = EEPROM.readLong(addSndCodeDisplayTime);
  sndCountdownTime = EEPROM.readLong(addSndCountdownTime);
  sndPenaltyMode = bool(EEPROM.readByte(addSndPenaltyMode));
  sndPenaltyApplied = EEPROM.readLong(addSndPenaltyApplied);
  domPrimingTime = EEPROM.readLong(addDomPrimingTime);
  domCountdownTime = EEPROM.readLong(addDomCountdownTime);
}



//Calculates an alternating value depending on specified time interval
bool flashState(unsigned int interval) {
  if (millis() % interval > interval / 2) {
    return false;
  } else {
    return true;
  }
}



//DEBUG: Used to determine how long code takes to execute
/*
long lastLoop = 0;
void loopTime() {
  Serial.println(millis() - lastLoop);
  
  lastLoop = millis();
}
*/
