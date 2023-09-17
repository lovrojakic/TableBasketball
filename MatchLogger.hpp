#ifndef TABLEBASKETBALL_MATCHLOGGER_H
#define TABLEBASKETBALL_MATCHLOGGER_H

#include "Match.hpp"

class MatchLogger {
  public:
    MatchLogger();
    bool storeMatchInfo(Match* match);
    char* readMatchHistory();
};

#endif
