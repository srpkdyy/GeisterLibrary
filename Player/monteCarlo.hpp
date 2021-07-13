#ifndef MONTE_CARLO
#define MONTE_CARLO

#include <string>
#include "random.hpp"
#include "player.hpp"
#include "simulator.hpp"


class MonteCarlo: public Player{
    cpprefjp::random_device rd;
    std::mt19937 mt;
public:
    MonteCarlo(): mt(rd()){
    }

    virtual std::string decideRed(){
        cpprefjp::random_device rd;
        std::mt19937 mt(rd());

        std::uniform_int_distribution<int> serector(0, pattern.size() - 1);
        return pattern[serector(mt)];
    }

    virtual std::string decideHand(std::string_view res){
        game.setState(res);
        Hand bestHand;
        auto legalMoves = candidateHand();

        for (const auto& move : legalMoves) {
          Unit u = move.unit;
          if (u.isBlue()) {
            if (u.x() == 0 && u.y() == 0) return Hand{u, 'W'};
            if (u.x() == 5 && u.y() == 5) return Hand{u, 'E'};
          }
        }

        int nPlay = 100;
        int maxScore = -nPlay - 1;
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

        /*
        std::uniform_int_distribution<int> selector(0, legalMoves.size() - 1);
        return legalMoves[selector(mt)];
        */
        return bestHand;
    }

    virtual std::vector<Hand> candidateHand(){
        return game.getLegalMove1st();
    }
};

#endif

