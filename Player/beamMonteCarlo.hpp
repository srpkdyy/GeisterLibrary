#ifndef BEAM_MONTE_CARLO
#define BEAM_MONTE_CARLO

#include <string>
#include "random.hpp"
#include "player.hpp"
#include "simulator.hpp"

class BeamMonteCarlo: public Player {
  cpprefjp::random_device rd;
  std::mt19937 mt;

public:
  BeamMonteCarlo(): mt(rd()) {
  }

  virtual std::string decideRed() {
    std::uniform_int_distribution<int> serector(0, pattern.size() - 1);
    return pattern[serector(mt)];
  }

  virtual std::string decideHand(std::string_view res) {
    game.setState(res);

    auto legalMoves = candidateHand();

    int nPlay = 10;
    int maxScore = -nPlay - 1;

    Hand bestHand;
    for (const auto& move : legalMoves) {
      Geister node = game;
      node.move(move);

      auto sim = Simulator(node);
      int score = sim.run(nPlay);

      if (score > maxScore) {
        maxScore = score;
        bestHand = move;
      }
    }
    return bestHand;
  }

  virtual std::vector<Hand> candidateHand() {
    return game.getLegalMove1st();
  }
};

#endif

