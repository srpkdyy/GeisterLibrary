#ifndef MY_PLAYER
#define MY_PLAYER

#include <string>
#include "random.hpp"
#include "player.hpp"


class MyPlayer: public Player{
    cpprefjp::random_device rd;
    std::mt19937 mt;
public:
    MyPlayer(): mt(rd()){
    }

    virtual std::string decideRed(){
        cpprefjp::random_device rd;
        std::mt19937 mt(rd());

        std::uniform_int_distribution<int> serector(0, pattern.size() - 1);
        return pattern[serector(mt)];
    }

    virtual std::string decideHand(std::string_view res){
        game.setState(res);

        const std::array<Unit, 16>& units = game.allUnit();
        for (const Unit& u: units) {
          if (u.color() == UnitColor::Blue) {
            if (u.x() == 0 && u.y() == 0) return Hand{u, Direction::West};
            if (u.x() == 5 && u.y() == 0) return Hand{u, Direction::East};
          }

        }


        auto legalMoves = candidateHand();
        std::uniform_int_distribution<int> serector1(0, legalMoves.size() - 1);
        auto action = legalMoves[serector1(mt) % legalMoves.size()];
        return action;
    }

    virtual std::vector<Hand> candidateHand(){
        return game.getLegalMove1st();
    }
};

#endif

