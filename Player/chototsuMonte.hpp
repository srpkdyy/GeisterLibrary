#ifndef CHOTOTSU_MONTE
#define CHOTOTSU_MONTE


#include <string>
#include "player.hpp"
#include "random.hpp"
#include "simulator.hpp"  


class ChototsuMonte: public Player {
  cpprefjp::random_device rnd;
  std::mt19937 mt;
  std::uniform_int_distribution<int> choiceRandom;
public:
  ChototsuMonte():
    mt(rnd()),
    choiceRandom(0, 1){}

  virtual std::string decideRed() {
    return std::string("BCFG");
  }

  virtual std::string decideHand(std::string_view res) {
    game.setState(res);

    int nBlueTaken = game.takenCount(UnitColor::Blue);
    Hand nextHand;

    if (choiceRandom(mt) < 1) {
      nextHand = chototsu();
    }
    else {
      nextHand = monteCarlo();
    }

    return nextHand;
  }

  Hand chototsu() {
    const std::array<Unit, 16>& units = game.allUnit();
    for (const Unit& u: units) {
      if (u.color() == UnitColor::Blue) {
        if (u.x() == 0 && u.y() == 0) {
          return Hand{u, Direction::West};
        }
        if(u.x() == 5 && u.y() == 0) {
          return Hand{u, Direction::East};
        }
      }
    }
    for (const Unit& u : units) {
      if (u.color() == UnitColor::Blue && u.y() == 0) {
        if (u.x() < 3 && u.x() > 0) {
          return Hand{u, Direction::West};
        }
        else if (u.x() < 5) {
          return Hand{u, Direction::East};
        }
      }
    }
    int mostFrontPos = units[0].y();
    int mostFrontIndex = 0;
    for (int u = 1; u < 8; ++u) {
      const Unit& unit = units[u];
      if (unit.color() == UnitColor::Blue && unit.y() <= mostFrontPos && unit.y() > 0) {
        if (unit.y() < mostFrontPos || choiceRandom(mt)) {
          mostFrontIndex = u;
          mostFrontPos = unit.y();
        }
      }
    }
    return Hand{units[mostFrontIndex], Direction::North};
  }

  Hand monteCarlo() {
    auto legalMoves = game.getLegalMove1st();

    for (const auto& move : legalMoves) {
      Unit u = move.unit;
      if (u.isBlue()) {
        if (u.x() == 0 && u.y() == 0) return Hand{u, 'W'};
        if (u.x() == 5 && u.y() == 0) return Hand{u, 'E'};
      }
    }

    std::vector<Simulator> simulators;
    for (auto m : legalMoves) {
      Simulator s{game};
      s.root.move(m);
      simulators.push_back(s);
    }

    std::vector<double> rewards(legalMoves.size(), 0.0);
    for (int i = 0; i < legalMoves.size(); i++) {
      rewards[i] += simulators[i].run(100);
    }

    int indexMax = 0;
    for (int i = 1; i < legalMoves.size(); i++) {
      if (rewards[indexMax] < rewards[i]) indexMax = i;
    }
    return legalMoves[indexMax];
  }
};

#endif

