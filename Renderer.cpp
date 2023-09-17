#include "Renderer.hpp"

#include <Adafruit_GFX.h>

#include <string.h>

Renderer::Renderer(Adafruit_ILI9341* _tft)
  : tft(_tft) {
}

void Renderer::reset() {
  tft->fillScreen(ILI9341_BLACK);  // boja zaslona
}

void Renderer::drawMenuItem(int x, int y, const char* menuItem, bool selected) {
  if (selected) {
    tft->drawRoundRect(x, y, 170, 40, 100, ILI9341_WHITE);
  }
  tft->setTextColor(ILI9341_WHITE);
  tft->setTextSize(2);
  tft->setCursor(x + 45, y + 12);
  tft->print(menuItem);
}

void Renderer::drawStartMenu(const char* selectedMenuItem) {
  tft->fillRect(0, 90, 320, 150, ILI9341_BLACK);
  drawMenuItem(75, 100, " START ", strcmp(selectedMenuItem, "START") == 0);
  drawMenuItem(75, 150, "MATCHES", strcmp(selectedMenuItem, "MATCHES") == 0);
}

void Renderer::drawTitleScreen(const char* selectedMenuItem) {
  reset();
  tft->setTextColor(ILI9341_ORANGE);
  tft->setTextSize(4);
  tft->setCursor(100, 15);
  tft->print("TABLE");
  tft->setCursor(50, 45);
  tft->print("BASKETBALL");

  drawStartMenu(selectedMenuItem);
}

void Renderer::drawPlayerNames(char* const player1, char* const player2) {
  tft->setTextSize(2);
  tft->setCursor(5, 120);
  tft->setTextColor(ILI9341_RED);
  tft->printf("%*s%s%*s", (12 - strlen(player1)) / 2, " ", player1, (11 - strlen(player1)) / 2, " ");
  tft->setTextColor(ILI9341_WHITE);
  tft->printf(" VS ");
  tft->setTextColor(ILI9341_BLUE);
  tft->printf("%*s%s%*s", (11 - strlen(player2)) / 2, " ", player2, (12 - strlen(player2)) / 2, " ");
}

void Renderer::drawTimer(int countdown, bool activeMatch) {
  tft->fillRect(0, 20, 320, 80, ILI9341_BLACK);
  tft->setTextColor(ILI9341_WHITE);
  tft->setTextSize(5);

  if (countdown == 0) {
    if (activeMatch) {
      tft->setCursor(15, 40);
      tft->printf("GAME OVER!");
    } else {
      tft->setCursor(120, 40);
      tft->printf("GO!");
    }
    return;
  } else if (countdown >= 10) {
    tft->setCursor(130, 40);
  } else {
    tft->setCursor(145, 40);
  }
  tft->printf("%d", countdown);
}

void Renderer::drawPoints(int playerIndex, int points) {
  tft->setTextColor(ILI9341_WHITE);
  tft->setTextSize(5);
  if (playerIndex == 1) {
    tft->fillRect(0, 155, 160, 50, ILI9341_BLACK);
    if (points >= 10) {
      tft->setCursor(40, 160);
    } else {
      tft->setCursor(60, 160);
    }
  } else if (playerIndex == 2) {
    tft->fillRect(160, 155, 160, 50, ILI9341_BLACK);
    if (points >= 10) {
      tft->setCursor(220, 160);
    } else {
      tft->setCursor(240, 160);
    }
  }
  tft->printf("%d", points);
}

void Renderer::drawMatchStart(Match* match) {
  reset();
  drawMenuItem(75, 40, " START ", true);
  drawPlayerNames(match->getPlayer1(), match->getPlayer2());
  drawPoints(1, match->getPoints1());
  drawPoints(2, match->getPoints2());
}

void Renderer::drawMatchList(char* matchHistory, int startIndex) {
  tft->fillRect(0, 45, 320, 195, ILI9341_BLACK);
  tft->setTextColor(ILI9341_WHITE);
  tft->setTextSize(2);
  tft->setCursor(0, 50);
  int i = 0;
  char* match = strtok(matchHistory, "\n");
  for (; i < startIndex && match != NULL; ++i) {
    match = strtok(NULL, "\n");
  }
  for (; i < startIndex + 11 && match != NULL; ++i) {
    tft->printf("%d %s\n", i + 1, match);
    match = strtok(NULL, "\n");
  }
}

void Renderer::drawMatchHistory(char* matchHistory, int startIndex) {
  reset();
  tft->setTextColor(ILI9341_ORANGE);
  tft->setTextSize(3);
  tft->setCursor(0, 15);
  tft->print("MATCH HISTORY:");

  drawMatchList(matchHistory, startIndex);
}
