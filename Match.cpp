#include "Match.hpp"

#include <cstdlib>

Match::Match(int nameLength)
  : player1((char*)calloc(nameLength + 1, sizeof(char))), player2((char*)calloc(nameLength + 1, sizeof(char))), points1(0), points2(0), active(false) {
}

Match::~Match() {
  free(player1);
  free(player2);
}

char* const Match::getPlayer1() {
  return player1;
}

char* const Match::getPlayer2() {
  return player2;
}

int Match::getPoints1() {
  return points1;
}

void Match::player1Scored() {
  points1 += 2;
}

int Match::getPoints2() {
  return points2;
}

void Match::player2Scored() {
  points2 += 2;
}

bool Match::isActive() {
  return active;
}

void Match::start() {
  active = true;
}

void Match::finish() {
  active = false;
}
