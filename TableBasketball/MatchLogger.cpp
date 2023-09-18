#include "MatchLogger.hpp"

#include "SD.h"
#include "FS.h"

MatchLogger::MatchLogger() {
  SD.begin(22);
}

bool MatchLogger::storeMatchInfo(Match* match) {
  const char* path = "/MatchHistory.txt";

  if (SD.cardType() == CARD_NONE) {
    Serial.println("No SD card attached");
    return false;
  }

  File file = SD.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return false;
  }
  if (file.printf("%s %d-%d %s\n", match->getPlayer1(), match->getPoints1(), match->getPoints2(), match->getPlayer2())) {
    Serial.println("Match appended");
    file.close();
    return true;
  } else {
    Serial.println("Append failed");
    file.close();
    return false;
  }
}

char* MatchLogger::readMatchHistory() {
  const char* path = "/MatchHistory.txt";
  char* matchHistory;

  if (SD.cardType() == CARD_NONE) {
    matchHistory = (char*)calloc(strlen("No SD card attached\n") + 1, sizeof(char));
    strcpy(matchHistory, "No SD card attached\n");
    Serial.println("No SD card attached");
    return matchHistory;
  }

  File file = SD.open(path);
  if (!file) {
    matchHistory = (char*)calloc(strlen("Failed to open file\n") + 1, sizeof(char));
    strcpy(matchHistory, "Failed to open file\n");
    Serial.println("Failed to open file");
  } else if (file.available() == 0) {
    matchHistory = (char*)calloc(strlen("Empty match history\n") + 1, sizeof(char));
    strcpy(matchHistory, "Empty match history\n");
    Serial.println("Empty match history");
  } else {
    matchHistory = (char*)calloc(file.available() + 1, sizeof(char));
    file.readBytes(matchHistory, file.available());
    Serial.println("Reading successful");
  }
  file.close();
  return matchHistory;
}
