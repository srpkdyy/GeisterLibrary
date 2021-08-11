#ifndef BEAM_SEARCH
#define BEAM_SEARCH

#include <string>
#include <bits/stdc++.h>
#include "random.hpp"
#include "player.hpp"
#include "simulator.hpp"


struct GameState {
  State(Geister s) : gs{s} {
    std::uniform_int_distribution<int> test(0, 100);
     score = test(mt);
  }
  int score;
  Hand hand;
  Geister gs;
  friend bool operator (const State& a, const State& b) {
    return a.score < b.score;
  }
}


class BeamSearch: public Player {
  cpprefjp::random_device rd;
  std::mt19937 mt;

public:
  BeamSearch(): mt(rd()) {
  }

  virtual std::string decideRed() {
    std::uniform_int_distribution<int> serector(0, pattern.size() - 1);
    return pattern[serector(mt)];
  }

  virtual std::string decideHand(std::string_view res) {
    game.setState(res);

    Hand hand = BeamSearch();
  }

  virtual std::vector<Hand> candidateHand() {
    return game.getLegalMove1st();
  }

  Hand BeamSearch() {
    int beamWidth = 5;
    std::priority_queue<Geister> que;
    que.push(game);

    while (not que.empty()) {
      priority_queue<Geister> nextQue;
      int queSize = len(que);

      for (int i = 0; i < min(beamWidth, queSize)) {
        Geister s = que.top(); que.pop();
        for (const auto& next : legalMoves) {
          Geister n = s;
          n.move(next);
          nextQue.push(n);
        }
      }
      que = nextQue;
    }
    return nullptr;
  }
};

#endif

