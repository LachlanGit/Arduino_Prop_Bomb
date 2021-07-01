int settingsScreen, settingsMessage = 0;
String inputString;
long inputInt;


void mode_config() {
  u8g2.drawRFrame(0, 0, 128, 64, 5);
  
  if (redButton.uniquePress() && settingsScreen > 0) {
    settingsScreen--;
    clearInput();
    settingsMessage = 0;
  }

  if (greenButton.uniquePress() && settingsScreen < 9) {
    settingsScreen++;
    clearInput();
    settingsMessage = 0;
  }

  if (settingsScreen < 9) {
    digitalWrite(GLED, HIGH);
  } else {
    digitalWrite(GLED, LOW);
  }

  if (settingsScreen > 0) {
    digitalWrite(RLED, HIGH);
  } else {
    digitalWrite(RLED, LOW);
  }

  u8g2.drawHLine(0, 25, 128);
  drawStrMiddle(0, 49, 128, "* = Clear, # = Save");
  u8g2.drawHLine(0, 50, 128);

  switch (settingsScreen) {
    case 0:
      {
        u8g2.drawStr(6, 12, "Game mode");
        u8g2.drawStr(6, 24, "0: SND, 1: DOM");

        if (handleInput(0, 1)) {
          EEPROM.updateByte(addStateMain, byte(inputInt));
        }
      }
      break;

    case 1:
      {
        u8g2.drawStr(6, 12, "Buzzer Volume");
        u8g2.drawStr(6, 24, "0 (off) to 10 (max)");

        if (handleInput(0, 10)) {
          EEPROM.updateByte(addVolume, byte(inputInt));
        }
      }
      break;

    case 2:
      {
        u8g2.drawStr(6, 12, "Snd Initialise Time");
        u8g2.drawStr(6, 24, "1 to 15 seconds");

        if (handleInput(1, 15)) {
          EEPROM.updateLong(addSndPrimingTime, long(inputInt) * 1000);
        }
      }
      break;

    case 3:
      {
        u8g2.drawStr(6, 12, "Snd Code Length");
        u8g2.drawStr(6, 24, "3 to 8 digits");

        if (handleInput(3, 8)) {
          EEPROM.updateByte(addSndCodeLength, byte(inputInt));
        }
      }
      break;

    case 4:
      {
        u8g2.drawStr(6, 12, "Code Display Time (ms)");
        u8g2.drawStr(6, 24, "0 = Auto, up to 5000");

        if (handleInput(0, 5000)) {
          EEPROM.updateInt(addSndCodeDisplayTime, int(inputInt));
        }
      }
      break;

    case 5:
      {
        u8g2.drawStr(6, 12, "Snd Countdown Time");
        u8g2.drawStr(6, 24, "10 to 600 seconds");

        if (handleInput(10, 600)) {
          EEPROM.updateLong(addSndCountdownTime, long(inputInt) * 1000);
        }
      }
      break;

    case 6:
      {
        u8g2.drawStr(6, 12, "Enable Penalty?");
        u8g2.drawStr(6, 24, "0 = False, 1 = True");

        if (handleInput(0, 1)) {
          EEPROM.updateByte(addSndPenaltyMode, byte(inputInt));
        }
      }
      break;

    case 7:
      {
        u8g2.drawStr(6, 12, "Wrong Code Penalty");
        u8g2.drawStr(6, 24, "5 to 60 seconds");

        if (handleInput(5, 60)) {
          EEPROM.updateLong(addSndPenaltyApplied, long(inputInt) * 1000);
        }
      }
      break;

    case 8:
      {
        u8g2.drawStr(6, 12, "Dom capture time");
        u8g2.drawStr(6, 24, "1 to 30 seconds");

        if (handleInput(1, 30)) {
          EEPROM.updateLong(addDomPrimingTime, long(inputInt) * 1000);
        }
      }
      break;

    case 9:
      {
        u8g2.drawStr(6, 12, "Dom countdown time");
        u8g2.drawStr(6, 24, "10 to 600 seconds");

        if (handleInput(10, 600)) {
          EEPROM.updateLong(addDomCountdownTime, long(inputInt) * 1000);
        }
      }
      break;
  }

  switch (settingsMessage) {
    case 0: //No message
      {
        drawStrMiddle(0, 62, 128, "Enter value...");
      }
      break;

    case 1: //Setting saved
      {
        drawStrMiddle(0, 62, 128, "Setting saved!");
      }
      break;

    case 2: //Setting out of bounds
      {
        drawStrMiddle(0, 62, 128, "Value out of bounds!");
      }
      break;
  }
}



bool handleInput(long lowerBound, long upperBound) {
  char key = keypad.getKey();

  if (key >= '0' && key <= '9' && inputString.length() < 5) {
    inputString += key;
  }

  //u8g2.drawStr(0, 30, "Value: ");
  String valueText = "Value: ";
  String blankText = "_";
  valueText += inputString;
  for (int i = 0; i < 5 - inputString.length(); i++) {
    valueText += blankText;
  }
  u8g2.drawStr(31, 37, valueText.c_str());

  if (key == '*') {
    inputString = "";
    settingsMessage = 0;
  }

  if (key == '#') {
    inputInt = inputString.toInt();

    if (inputInt < lowerBound || inputInt > upperBound) {
      settingsMessage = 2;
    } else {
      //In range
      //EEPROM.writeLong(address, long(inputInt));
      settingsMessage = 1;
      return true;
    }
  }
  return false;
}



void clearInput() {
  inputString = "";
}
