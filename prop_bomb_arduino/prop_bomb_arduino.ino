/*
  Downloaded from and more information at: https://github.com/LachlanGit/Arduino_Prop_Bomb
  
  Code to run on prop bomb for use in airsoft/gel-blaster/nerf games
  Requires:
  - Arduino Mega2560
  - 128 x 64 SPI Monochrome LCD display
  - 3 x 4 Numerical keypad
  - Red + Green button
  - Red + Green LED or LED's in buttons
  - 3 position keyswitch (Off, On, Config) or 2 position keyswitch (normal, config) with seperate power toggle switch
  - Square wave buzzer
*/

//===== Libraries =====
#include <U8g2lib.h>

#include <Key.h>
#include <Keypad.h>

#include <Button.h>

#include <toneAC.h>

#include <EEPROMex.h>
#include <EEPROMVar.h>

#include <Timers.h>



//===== Other includes =====
#include "fonts.h"



//===== Pins =====
//Buttons
#define RBUTTON 47
#define RBUTTONGND 49
#define GBUTTON 48
#define GBUTTONGND 46
#define RLED 53
#define GLED 52

//Key switch
#define KEYNO 22
#define KEYGND 24

//Keypad
#define KEY1 19 //3 ON KEYPAD BOARD
#define KEY2 14 //8
#define KEY3 15 //7
#define KEY4 17 //5
#define KEYA 18 //4
#define KEYB 20 //2
#define KEYC 16 //6

//LCD
#define LCD_E 2
#define LCD_RW 3
#define LCD_RS 4

//Buzzer
//11 + 12



//===== Keypad Layout =====
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3
const char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
const byte rowPins[KEYPAD_ROWS] = {KEY1, KEY2, KEY3, KEY4};
const byte colPins[KEYPAD_COLS] = {KEYA, KEYB, KEYC};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);



//===== Main states =====
#define SND 0                  //Search and destroy
#define DOM 1                  //Domination
#define CONFIG 2               //Config mode



//===== Button objects =====
Button redButton = Button(RBUTTON, BUTTON_PULLUP_INTERNAL, true, 50);
Button greenButton = Button(GBUTTON, BUTTON_PULLUP_INTERNAL, true, 50);



//===== Timer objects =====
Timer sndPrimingTimer(0, true);
Timer sndCodeDispTimer(0, true);
Timer sndCodeResultTimer(0, true);
Timer sndCountdownTimer(0, true);



//===== LCD Object =====
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* SCK/E=*/ LCD_E, /* SID/RW=*/ LCD_RW, /* CS/RS=*/ LCD_RS, /* reset=*/ U8X8_PIN_NONE);



//===== Configurable settings =====
//These will be over-ridden with values in EEPROM on startup
//Holding both buttons on startup will revert to the default values entered below:

//Global settings
byte stateMain = 0; //0: SND, 1: DOM                (Default: 0 (SND))
byte volume = 10; //0 (off) to 10 (max)             (Default: 10)

//SND
long sndPrimingTime = 3000; //millis                (Default: 3 seconds)
byte sndCodeLength = 6; //Ranges from 3 to 8        (Default: 6 digits)
int sndCodeDisplayTime = 0; //millis (0 for auto)   (Default: 0 (auto))
long sndCountdownTime = 90000; //millis             (Default: 1m30s)
bool sndPenaltyMode = false; //true = on            (Default: Penalty off)
long sndPenaltyApplied = 10000; //millis            (Default: 10s)

//DOM
long domPrimingTime = 10000; //millis               (Default: 10 seconds)
long domCountdownTime = 120000; //millis            (Default: 2m00s)



//===== EEPROM Addresses =====
int addStateMain;
int addVolume;
int addSndPrimingTime;
int addSndCodeLength;
int addSndCodeDisplayTime;
int addSndCountdownTime;
int addSndPenaltyMode;
int addSndPenaltyApplied;
int addDomPrimingTime;
int addDomCountdownTime;



//===== Random seed generation =====
volatile uint32_t seed;  // These two variables can be reused in your program after the
volatile int8_t nrot;    // function CreateTrulyRandomSeed()executes in the setup() function.



//===== Setup =====
void setup() {
  Serial.begin(119200);

  //Generate random seed
  CreateTrulyRandomSeed();
  randomSeed(seed);


  //Set-up pins
  pinMode(KEYNO, INPUT_PULLUP); //GND->COM Pin22->NO, 1 means locked, 0 means unlocked
  pinMode(KEYGND, OUTPUT);
  pinMode(RBUTTON, INPUT_PULLUP); //GND->switch Pin23->switch, 0 means pressed
  pinMode(GBUTTON, INPUT_PULLUP); //GND->switch Pin24->switch, 0 means pressed
  pinMode(RBUTTONGND, OUTPUT);
  pinMode(GBUTTONGND, OUTPUT);

  digitalWrite(KEYGND, LOW);
  digitalWrite(RBUTTONGND, LOW);
  digitalWrite(GBUTTONGND, LOW);

  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);


  //LCD setup
  u8g2.begin();
  resetFont();


  //Keypad setup
  keypad.setDebounceTime(1);


  //EEPROM setup
  setupEEPROM();


  //Calculate automatic code display length if set to 0
  if (sndCodeDisplayTime == 0) {
    sndCodeDisplayTime = 100 + sndCodeLength * 250;
  }
}



//===== Main loop =====
void loop() {
  resetFont();
  u8g2.clearBuffer();


  //Check key state
  if (!digitalRead(KEYNO)) {
    stateMain = CONFIG;
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);
  }


  //Run code relevant to set mode
  switch (stateMain) {
    case SND:
      {
        mode_snd();
      }
      break;

    case DOM:
      {
        mode_dom();
      }
      break;

    case CONFIG:
      {
        mode_config();
      }
      break;
  }


  u8g2.sendBuffer();
}
