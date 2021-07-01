//===== SND States =====
#define SND_IDLE 0
#define SND_CODE_DISP 1
#define SND_CODE_ENTRY 2
#define SND_COUNTDOWN 3
#define SND_END 4


byte stateSnd = SND_IDLE;
bool stateArm = false;



//===== Fixed settings =====
const unsigned long codeConfirmTime = 1500;



//===== Code generation =====
int generatedCode[8];
char generatedCodeChar[8 + 1];



//===== Code entry =====
#define CODE_WAITING 0
#define CODE_CORRECT 1
#define CODE_WRONG 2
byte codeEntryResult = CODE_WAITING;

char customKey;
bool wrongCode;
char enteredCodeStr[9];
int cursorPos;



//===== Sound effect states =====
bool powerEffectPlayed = false;
bool sndSirenPlayed = false;
bool victoryPlayed = false;



//===== Main loop =====
void mode_snd() {
  u8g2.drawRFrame(0, 0, 128, 64, 5);

  switch (stateSnd) {
    case SND_IDLE:
      {
        if (!digitalRead(RBUTTON) && !digitalRead(GBUTTON)) {
          primingLoadScreen();
        } else {
          sndPrimingTimer.setLength(sndPrimingTime);

          drawStrMiddle(0, 12, 128, "READY TO ARM");
          drawStrMiddle(0, 36, 128, "<HOLD BOTH BUTTONS>");

          toneAC();
          digitalWrite(RLED, flashState(500));
          digitalWrite(GLED, flashState(500));
        }
      }
      break;



    case SND_CODE_DISP:
      {
        sndCodeDispTimer.start();

        codeDisplay();

        if (sndCodeDispTimer.timeLeft() < 0) {
          codeEntryResult = CODE_WAITING;

          sndCodeResultTimer.pause();
          sndCodeResultTimer.setLength(1500);

          stateSnd = SND_CODE_ENTRY;
          cursorPos = 0;
        }
      }
      break;



    case SND_CODE_ENTRY:
      {
        if (stateArm) {
          sndCountdown(true);
        }

        switch (codeEntryResult) {
          case CODE_WAITING:
            {
              codeEntry();
            }
            break;

          case CODE_WRONG:
            {
              sndCodeResultTimer.start();

              digitalWrite(RLED, flashState(250));
              digitalWrite(GLED, !flashState(250));

              drawStrMiddle(0, 12, 128, "ERROR:");
              drawStrMiddle(0, 45, 128, "WRONG CODE!");

              u8g2.setFont(u8g2_font_unifont_t_0_77);
              drawGlyphMiddle(0, 32, 128, 9888);
              resetFont();

              if (sndCodeResultTimer.timeLeft() < 0) {
                sndPrimingTimer.setLength(sndPrimingTime);

                if (stateArm) {
                  stateSnd = SND_COUNTDOWN;
                } else {
                  stateSnd = SND_IDLE;
                }
              }
            }
            break;

          case CODE_CORRECT:
            {
              sndCodeResultTimer.start();

              digitalWrite(RLED, LOW);
              digitalWrite(GLED, LOW);

              drawStrMiddle(0, 12, 128, "CORRECT CODE");

              if (stateArm) {
                sndCountdownTimer.pause();

                drawStrMiddle(0, 36, 128, "DISARMED");

                if (!powerEffectPlayed) {
                  u8g2.sendBuffer();
                  beepPowerDown();
                  powerEffectPlayed = true;
                }
              } else {
                drawStrMiddle(0, 36, 128, "ARMED");

                if (!powerEffectPlayed) {
                  u8g2.sendBuffer();
                  beepPowerUp();
                  powerEffectPlayed = true;
                }
              }

              if (sndCodeResultTimer.timeLeft() < 0) {
                sndPrimingTimer.setLength(sndPrimingTime);
                
                if (stateArm) {
                  stateSnd = SND_END;
                  stateArm = false;
                } else {
                  stateSnd = SND_COUNTDOWN;
                  stateArm = true;

                  sndCountdownTimer.setLength(sndCountdownTime);
                  sndCountdownTimer.start();
                }
              }
            }
            break;
        }
      }
      break;



    case SND_COUNTDOWN:
      {
        if (!digitalRead(RBUTTON) && !digitalRead(GBUTTON)) {
          primingLoadScreen();
        } else {
          sndCountdown(false);

          sndPrimingTimer.setLength(sndPrimingTime);
        }
      }
      break;



    case SND_END:
      {
        drawStrMiddle(0, 53, 128, "PRESS BOTH BUTTONS");
        drawStrMiddle(0, 64, 128, "TO RESET");

        if (stateArm) {
          drawStrMiddle(0, 12, 128, "BOMB EXPLODED!");
          drawStrMiddle(0, 23, 128, "T's WIN!");

          if (!sndSirenPlayed) {
            u8g2.sendBuffer();
            beepSiren();
            sndSirenPlayed = true;
          }
        } else {
          drawStrMiddle(0, 12, 128, "DEFUSED");
          drawStrMiddle(0, 23, 128, "CT's WIN!");

          if (!victoryPlayed) {
            u8g2.sendBuffer();
            beepVictory();
            victoryPlayed = true;
          }
        }

        digitalWrite(RLED, flashState(500));
        digitalWrite(GLED, flashState(500));

        if (redButton.isPressed() && greenButton.isPressed()) {
          stateSnd = IDLE;
          stateArm = false;

          victoryPlayed = false;
          sndSirenPlayed = false;
          while (redButton.isPressed() && greenButton.isPressed());
        }
      }
      break;
  }
}



//===== Priming screen for SND =====
void primingLoadScreen() {
  sndPrimingTimer.start();

  digitalWrite(RLED, HIGH);
  digitalWrite(GLED, HIGH);

  if (!stateArm) {
    drawStrMiddle(0, 12, 128, "ARMING");
  } else {
    drawStrMiddle(0, 12, 128, "DISARMING");
    sndCountdown(true);
  }

  float loadingBarProgress = 1 - (sndPrimingTimer.timeLeft() / float(sndPrimingTime));
  u8g2.drawFrame(4, 27, 120, 10);
  u8g2.drawBox(4, 27, 120 * loadingBarProgress, 10);

  primingBeep(sndPrimingTimer.timeLeft(), sndPrimingTime);

  if (sndPrimingTimer.timeLeft() < 0) {
    stateSnd = SND_CODE_DISP;
    sndCodeDispTimer.setLength(sndCodeDisplayTime);
    generateCode();

    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);

    toneAC();
  }
}



//===== Generate code =====
void generateCode() {
  for (int i = 0; i < sndCodeLength; i++) {
    generatedCode[i] = random(0, 10);
    generatedCodeChar[i] = generatedCode[i] + 48;
  }
}



//===== Display generated code =====
void codeDisplay() {
  u8g2.drawStr(47, 12, "CODE:");
  u8g2.setFont(u8g2_font_dseg14_cr);
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  drawStrMiddle(0, 42, 128, generatedCodeChar);
  resetFont();

  if (stateArm) {
    sndCountdown(true);
  }
}



//===== Code entry handler =====
int codeEntry() {
  //Check for key
  customKey = keypad.getKey();

  //Clear entered code if star is pressed
  if (customKey == '*') {
    for (int i = 0; i < sndCodeLength; i++) {
      enteredCodeStr[i] = NULL;
    }
    cursorPos = 0;
  }

  //Display code
  for (int i = 0; i < sndCodeLength; i++) {
    if (i >= cursorPos) {
      if (flashState(500) && cursorPos == i) {
        enteredCodeStr[i] = ' ';
      } else {
        enteredCodeStr[i] = '_';
      }
    }
  }

  u8g2.setFont(u8g2_font_dseg14_cr);
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  drawStrMiddle(0, 42, 128, enteredCodeStr);
  resetFont();
  drawStrMiddle(0, 22, 128, "* TO CLEAR");

  //Handle code input
  if (cursorPos < sndCodeLength) {  //Do this when empty spots remaining
    drawStrMiddle(0, 12, 128, "ENTER CODE");

    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);

    if (customKey >= '0' && customKey <= '9') {
      enteredCodeStr[cursorPos] = customKey;
      beepButton();
      cursorPos++;
    }
  } else {  //Do this when all numbers filled
    if (!stateArm) {
      digitalWrite(RLED, flashState(500));
      drawStrMiddle(0, 12, 128, "< PRESS RED");
    } else {
      digitalWrite(GLED, flashState(500));
      drawStrMiddle(0, 12, 128, "PRESS GREEN >");
    }

    if ((!stateArm && !digitalRead(RBUTTON)) || (stateArm && !digitalRead(GBUTTON))) {
      wrongCode = false;

      for (int i = 0; i < sndCodeLength; i++) {
        if (enteredCodeStr[i] - 48 != generatedCode[i]) {
          wrongCode = true;
        }
      }

      if (wrongCode) {
        beepBad();

        if (stateArm && sndPenaltyMode) {
          sndCountdownTimer.addTime(-sndPenaltyApplied);
        }

        codeEntryResult = CODE_WRONG;

      } else {
        beepGood();

        codeEntryResult = CODE_CORRECT;

        powerEffectPlayed = false;
      }
    }
  }
}



//===== Display countdown on screen =====
void sndCountdown(bool isSmall) {

  if (isSmall) { //Draw small version

    u8g2.setFont(u8g2_font_dseg7_classic_regular_all);
    drawStrMiddle(0, 61, 128, sndCountdownTimer.string().c_str());
    resetFont();

  } else { //Draw normal version

    u8g2.drawStr(12, 12, "ARMED, TIME LEFT:");
    u8g2.setFont(u8g2_font_dseg14_cr);
    drawStrMiddle(0, 42, 128, sndCountdownTimer.string().c_str());
    resetFont();
    drawStrMiddle(0, 64, 128, "< HOLD TO DISARM >");

    digitalWrite(RLED, flashState(500));
    digitalWrite(GLED, flashState(500));

  }

  if (!sndCountdownTimer.isPaused()) {
    beepCountdown(sndCountdownTimer.timeLeft());
  }

  if (sndCountdownTimer.timeLeft() < 0) {
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);

    stateSnd = SND_END;

    //sndSirenPlayed = false;
  }
}
