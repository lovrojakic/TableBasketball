#include "Renderer.hpp"
#include "MatchLogger.hpp"
#include "Match.hpp"

#include <Keyboard.hpp>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#include "string.h"

#define NAME_LENGTH 9
#define TFT_CS 5
#define TFT_DC 21
#define TS_CS 4
#define BTN_A_PIN 32
#define BTN_UP_DOWN_PIN 35
#define BTN_L_R_PIN 34
#define PLAYER1_IR_PIN 27
#define PLAYER2_IR_PIN 39

Adafruit_ILI9341* tft = new Adafruit_ILI9341(TFT_CS, TFT_DC);
XPT2046_Touchscreen* ts = new XPT2046_Touchscreen(TS_CS);

Renderer* renderer = new Renderer(tft);
Keyboard* keyboard = new Keyboard(tft, ts);
MatchLogger* matchLogger = new MatchLogger();

hw_timer_t* My_timer = timerBegin(2, 80, true);
volatile int countdown;

Match* currentMatch;

void IRAM_ATTR onTimer() {
  renderer->drawTimer(--countdown, currentMatch->isActive());
}

const char* menuScenario() {
  const char* selectedMenuItem = "START";
  renderer->drawTitleScreen(selectedMenuItem);

  while (digitalRead(BTN_A_PIN) == HIGH) {
    if (analogRead(BTN_UP_DOWN_PIN) == 4095) {  // up
      selectedMenuItem = "START";
      renderer->drawStartMenu(selectedMenuItem);
      delay(200);
    } else if (analogRead(BTN_UP_DOWN_PIN) >= 1800 && analogRead(BTN_UP_DOWN_PIN) <= 1830) {  // down
      selectedMenuItem = "MATCHES";
      renderer->drawStartMenu(selectedMenuItem);
      delay(200);
    }
  }
  delay(200);

  return selectedMenuItem;
}

void matchScenario() {
  currentMatch = new Match(NAME_LENGTH);

  keyboard->drawPlayerInput(1);
  while (digitalRead(BTN_A_PIN) == HIGH) {
    keyboard->getKeyPress(currentMatch->getPlayer1(), NAME_LENGTH);
  }
  delay(200);

  keyboard->drawPlayerInput(2);
  while (digitalRead(BTN_A_PIN) == HIGH) {
    keyboard->getKeyPress(currentMatch->getPlayer2(), NAME_LENGTH);
  }
  delay(200);

  renderer->drawMatchStart(currentMatch);
  while (digitalRead(BTN_A_PIN) == HIGH) {
  }
  delay(200);

  countdown = 4;
  timerAlarmWrite(My_timer, 1000000, true);
  timerAlarmEnable(My_timer);
  while (countdown > 0) continue;

  currentMatch->start();
  countdown = 60;
  int lastPlayer1PointTime = 0;
  int lastPlayer2PointTime = 0;

  while (countdown > 0) {
    if (digitalRead(PLAYER1_IR_PIN) == LOW && millis() - lastPlayer1PointTime > 200) {
      currentMatch->player1Scored();
      lastPlayer1PointTime = millis();
      noInterrupts();
      renderer->drawPoints(1, currentMatch->getPoints1());
      interrupts();
    }
    if (digitalRead(PLAYER2_IR_PIN) == LOW && millis() - lastPlayer2PointTime > 200) {
      currentMatch->player2Scored();
      lastPlayer2PointTime = millis();
      noInterrupts();
      renderer->drawPoints(2, currentMatch->getPoints2());
      interrupts();
    }
  };

  timerAlarmDisable(My_timer);
  currentMatch->finish();
  matchLogger->storeMatchInfo(currentMatch);
  currentMatch->~Match();
  while (digitalRead(BTN_A_PIN) == HIGH) {
  }
  delay(200);
}

void historyScenario() {
  char* matchHistory = matchLogger->readMatchHistory();
  char* matchHistoryCopy;
  int startIndex = 0;

  matchHistoryCopy = (char*)calloc(strlen(matchHistory) + 1, sizeof(char));
  strcpy(matchHistoryCopy, matchHistory);
  renderer->drawMatchHistory(matchHistoryCopy, startIndex);
  free(matchHistoryCopy);

  while (digitalRead(BTN_A_PIN) == HIGH) {
    if (analogRead(BTN_UP_DOWN_PIN) == 4095 && startIndex > 0) {  // up
      matchHistoryCopy = (char*)calloc(strlen(matchHistory) + 1, sizeof(char));
      strcpy(matchHistoryCopy, matchHistory);
      renderer->drawMatchList(matchHistoryCopy, --startIndex);
      free(matchHistoryCopy);
      delay(200);
    } else if (analogRead(BTN_UP_DOWN_PIN) >= 1800 && analogRead(BTN_UP_DOWN_PIN) <= 1830) {  // down
      matchHistoryCopy = (char*)calloc(strlen(matchHistory) + 1, sizeof(char));
      strcpy(matchHistoryCopy, matchHistory);
      renderer->drawMatchList(matchHistoryCopy, ++startIndex);
      free(matchHistoryCopy);
      delay(200);
    }
  }
  delay(200);

  free(matchHistory);
}

void setup() {
  Serial.begin(9600);

  tft->begin();
  if (!ts->begin()) {
    Serial.println("Unable to start touchscreen.");
  }

  tft->setRotation(3);
  ts->setRotation(3);

  pinMode(BTN_A_PIN, INPUT_PULLUP);
  pinMode(BTN_UP_DOWN_PIN, INPUT_PULLUP);
  pinMode(BTN_L_R_PIN, INPUT_PULLUP);
  pinMode(PLAYER1_IR_PIN, INPUT);
  pinMode(PLAYER2_IR_PIN, INPUT);

  timerAttachInterrupt(My_timer, &onTimer, true);
}

void loop() {
  const char* selectedMenuItem = menuScenario();
  if (strcmp(selectedMenuItem, "START") == 0) {
    matchScenario();
  } else if (strcmp(selectedMenuItem, "MATCHES") == 0) {
    historyScenario();
  }
}
