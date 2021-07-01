//### TEAM DEFINITIONS ###
#define RED 0
#define GRN 1
#define NONE 2

//### DOM STATE DEFINITIONS ###
#define DOM_IDLE 0  //Ready to capture (instruction to hold your teams button)
#define DOM_CAPTURING 1   //Currently holding one of the teams buttons
#define DOM_HOLDING 2   //Shows progress screen, currently holding team, time left for each team
#define DOM_END 3     //Shows winning team and instruction to reset

int stateDom = 0;

unsigned long captureStart;

byte holdingTeam = NONE;
byte winningTeam = NONE;

long timeEndRed, timeEndGrn, timeLeftRed, timeLeftGrn, timeLeftRedPaused, timeLeftGrnPaused;
Timer domRedTimer(0, true);
Timer domGrnTimer(0, true);
Timer domPrimingTimer(0, true);



//===== Sound effect states =====
bool domSirenPlayed = false;



//===== Main dominiation mode loop =====
void mode_dom() {
  u8g2.drawRFrame(0, 0, 128, 64, 5);

  switch (stateDom) {
    case DOM_IDLE:
      {
        drawStrMiddle(0, 26, 128, "HOLD YOUR");
        drawStrMiddle(0, 36, 128, "COLOUR");
        drawStrMiddle(0, 46, 128, "TO CAPTURE");

        if (flashState(500)) {
          u8g2.drawTriangle(2, 32, 9, 25, 9, 39);
          u8g2.drawTriangle(126, 32, 119, 25, 119, 39);
        }

        digitalWrite(RLED, flashState(500));
        digitalWrite(GLED, flashState(500));

        if (!digitalRead(RBUTTON) || !digitalRead(GBUTTON)) {
          stateDom = DOM_CAPTURING;
          domPrimingTimer.setLength(domPrimingTime);
          domPrimingTimer.start();

          domRedTimer.setLength(domCountdownTime);
          domGrnTimer.setLength(domCountdownTime);
          domRedTimer.pause();
          domGrnTimer.pause();
        }
      }
      break;



    case DOM_CAPTURING:
      {
        drawStrMiddle(0, 12, 128, "CAPTURING");

        if (!digitalRead(RBUTTON) && digitalRead(GBUTTON) && holdingTeam != RED) {
          digitalWrite(RLED, HIGH);
          digitalWrite(GLED, LOW);

          domLoadingBar(domPrimingTimer, "RED");

          if (domPrimingTimer.timeLeft() < 0) {
            domRedTimer.start();
            domGrnTimer.pause();

            holdingTeam = RED;
            stateDom = DOM_HOLDING;
            toneAC();
          }


        } else if (!digitalRead(GBUTTON) && digitalRead(RBUTTON) && holdingTeam != GRN) {
          digitalWrite(RLED, LOW);
          digitalWrite(GLED, HIGH);

          domLoadingBar(domPrimingTimer, "GREEN");

          if (domPrimingTimer.timeLeft() < 0) {
            domRedTimer.pause();
            domGrnTimer.start();

            holdingTeam = GRN;
            stateDom = DOM_HOLDING;
            toneAC();
          }


        } else if (!digitalRead(GBUTTON) && !digitalRead(RBUTTON)) {
          drawStrMiddle(0, 32, 128, "ONLY HOLD");
          drawStrMiddle(0, 42, 128, "ONE BUTTON!");

          digitalWrite(RLED, flashState(500));
          digitalWrite(GLED, !flashState(500));

          domPrimingTimer.setLength(domPrimingTime);

          toneAC();
        } else {
          if (holdingTeam == NONE) {
            stateDom = DOM_IDLE;
          } else {
            stateDom = DOM_HOLDING;
          }
          toneAC();
        }
      }
      break;



    case DOM_HOLDING:
      {
        u8g2.drawVLine(63, 0, 64);
        u8g2.drawVLine(64, 0, 64);

        if (holdingTeam == RED) {
          u8g2.drawRBox(5, 5, 54, 38, 8);     //Red box
          u8g2.drawRFrame(69, 5, 54, 38, 8);  //Green frame
        } else {
          u8g2.drawRFrame(5, 5, 54, 38, 8);   //Red frame
          u8g2.drawRBox(69, 5, 54, 38, 8);    //Green box
        }

        u8g2.setDrawColor(2);
        u8g2.setFontMode(1);

        drawStrMiddle(0, 18, 64, "RED");
        drawStrMiddle(0, 28, 64, "TEAM");

        drawStrMiddle(64, 18, 64, "GREEN");
        drawStrMiddle(64, 28, 64, "TEAM");

        resetFont();


        if ((holdingTeam == RED && !digitalRead(GBUTTON)) || (holdingTeam == GRN && !digitalRead(RBUTTON))) {
          stateDom = DOM_CAPTURING;
          domPrimingTimer.setLength(domPrimingTime);
          toneAC();
        }

        timeLeftRed = domRedTimer.timeLeft();
        timeLeftGrn = domGrnTimer.timeLeft();

        if (holdingTeam == RED) {
          beepCountdown(timeLeftRed);

          digitalWrite(RLED, HIGH);
          digitalWrite(GLED, LOW);

        } else if (holdingTeam = GRN) {
          beepCountdown(timeLeftGrn);

          digitalWrite(RLED, LOW);
          digitalWrite(GLED, HIGH);
        }

        u8g2.setDrawColor(2);
        u8g2.setFontMode(1);
        displayDomCountdown(0, 40, 64, domRedTimer);

        u8g2.setDrawColor(2);
        u8g2.setFontMode(1);
        displayDomCountdown(64, 40, 64, domGrnTimer);

        resetFont();


        //Red bar
        float redProgress = (domCountdownTime - timeLeftRed) / float(domCountdownTime);
        u8g2.drawFrame(4, 48, 56, 12);
        u8g2.drawBox(4, 48, 56 * redProgress, 12);

        //Green bar
        float grnProgress = (domCountdownTime - timeLeftGrn) / float(domCountdownTime);
        u8g2.drawFrame(68, 48, 56, 12);
        u8g2.drawBox(68, 48, 56 * grnProgress, 12);

        u8g2.setDrawColor(2);
        u8g2.setFontMode(1);
        drawStrMiddle(4, 60, 56, "RED");
        drawStrMiddle(68, 60, 56, "GREEN");
        resetFont();

        if (timeLeftRed < 0) {
          winningTeam = RED;
          stateDom = DOM_END;
          domSirenPlayed = false;
          toneAC();
        }

        if (timeLeftGrn < 0) {
          winningTeam = GRN;
          stateDom = DOM_END;
          domSirenPlayed = false;
          toneAC();
        }
      }
      break;



    case DOM_END:
      {
        if (winningTeam == RED) {       //RED message
          drawStrMiddle(0, 12, 128, "RED");
        } else if (winningTeam = GRN) { //GREEN message
          drawStrMiddle(0, 12, 128, "GREEN");
        }

        drawStrMiddle(0, 23, 128, "WIN!");
        drawStrMiddle(0, 53, 128, "PRESS BOTH BUTTONS");
        drawStrMiddle(0, 64, 128, "TO RESET");

        if (redButton.isPressed() && greenButton.isPressed()) {
          stateDom = DOM_IDLE;
          holdingTeam = NONE;
          winningTeam = NONE;
          while (redButton.isPressed() && greenButton.isPressed());
        }

        digitalWrite(RLED, flashState(500));
        digitalWrite(GLED, flashState(500));

        if (!domSirenPlayed) {
          u8g2.sendBuffer();
          beepSiren();
          domSirenPlayed = true;
        }
      }
      break;
  }
}



//===== Domination loading bar =====
void domLoadingBar(Timer timer, char* text) {
  float loadingBarProgress = constrain((domPrimingTime - timer.timeLeft()) / float(domPrimingTime), 0, 1);

  u8g2.drawFrame(4, 26, 120, 12);
  u8g2.drawBox(4, 26, 120 * loadingBarProgress, 12);

  u8g2.setDrawColor(2);
  u8g2.setFontMode(1);
  drawStrMiddle(0, 38, 128, text);
  resetFont();

  //Beep sound effect
  primingBeep(timer.timeLeft(), domPrimingTime);
}



//===== Domination countdown display =====
void displayDomCountdown(int x, int y, int w, Timer timer) {
  String timeLeftStr = timer.string();

  u8g2.setFont(u8g2_font_dseg7_classic_regular_all);
  u8g2.drawStr(x + ((w - u8g2.getStrWidth(timeLeftStr.c_str())) / 2), y, timeLeftStr.c_str());
  resetFont();
}
