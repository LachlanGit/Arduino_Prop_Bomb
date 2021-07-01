//===== Fixed settings for sound effects =====
const int primingPitchStart = 1000;
const int primingPitchEnd = 2000;

//===== Variables =====
unsigned long primingNextBeep = 0;
int beepInterval;
unsigned long beepNext = 0;



//===== Sound effect for rising beep when priming =====
void primingBeep(unsigned long initLeft, unsigned long initLength) {
  int primingPitch = map(initLeft, initLength, 0, primingPitchStart, primingPitchEnd);

  if (millis() > primingNextBeep) {
    toneAC(primingPitch, volume, 25, true);
    primingNextBeep = millis() + 80;
  }
}



void beepGood() {
  toneAC(1400, volume);
  delay(50);
  toneAC(2000, volume);
  delay(50);
  toneAC();
}



void beepBad() {
  toneAC(600, volume);
  delay(75);
  toneAC(500, volume);
  delay(75);
  toneAC(300, volume);
  delay(75);
  toneAC();
}



void beepButton() {
  toneAC(1000, volume, 10);
}



void beepCountdown(long millisLeft) {
  if (millisLeft > 60000) {
    beepInterval = 1000;
  } else {
    beepInterval = constrain(map(millisLeft, 60000, 3000, 1000, 125), 125, 1000);
  }

  if (millis() > beepNext) {
    toneAC(4000, volume, 50);

    beepNext += beepInterval;

    if (millis() + beepInterval - 125 > beepNext) {
      beepNext = millis() + beepInterval;
    }
  }
}



void beepPowerDown() {
  unsigned long soundStart = millis();
  while (millis() < soundStart + 1000) {
    int pitch = map(millis() - soundStart, 0, 1000, 1000, 10);
    toneAC(pitch, volume);
  }

  toneAC();
}



void beepPowerUp() {
  unsigned long soundStart = millis();
  while (millis() < soundStart + 1000) {
    int pitch = map(millis() - soundStart, 0, 1000, 10, 1000);
    toneAC(pitch, volume);
  }

  toneAC();
}



void beepSiren() {
  unsigned long soundStart = millis();
  while (millis() < soundStart + 2000) {
    int pitch = 1000 + ((sin(((millis() - soundStart) / 300.00) * (PI * 2.00)) + 1) / 2) * 1000;
    toneAC(pitch, volume);
  }

  toneAC();
}



void beepVictory() {
  toneAC(784, volume);
  delay(200);
  toneAC(523 , volume);
  delay(100);
  toneAC(784 , volume);
  delay(100);
  toneAC(1047 , volume);
  delay(100);
  toneAC();
}
