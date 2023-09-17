#ifndef TABLEBASKETBALL_RENDERER_H
#define TABLEBASKETBALL_RENDERER_H

#include "Match.hpp"

#include <Adafruit_ILI9341.h>

class Renderer {
  private:
    Adafruit_ILI9341* tft;
  public:
    Renderer(Adafruit_ILI9341* _tft);
    void reset();
    void drawMenuItem(int x, int y, const char* menuItem, bool selected);
    void drawStartMenu(const char* selectedMenuItem);
    void drawTitleScreen(const char* selectedMenuItem);
    void drawPlayerNames(char* const player1, char* const player2);
    void drawTimer(int countdown, bool activeMatch);
    void drawPoints(int playerIndex, int points);
    void drawMatchStart(Match* match);
    void drawMatchList(char* matchHistory, int startIndex);
    void drawMatchHistory(char* matchHistory, int startIndex);
};

#endif
