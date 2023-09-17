#ifndef TABLEBASKETBALL_MATCH_H
#define TABLEBASKETBALL_MATCH_H

class Match {
  private:
    char* const player1;
    char* const player2;
    int points1;
    int points2;
    bool active;
  public:
    Match(int nameLength);
    ~Match();
    char* const getPlayer1();
    char* const getPlayer2();
    int getPoints1();
    void player1Scored();
    int getPoints2();
    void player2Scored();
    bool isActive();
    void start();
    void finish();
};

#endif
