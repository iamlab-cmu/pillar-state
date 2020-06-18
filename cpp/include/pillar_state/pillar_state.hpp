#ifndef PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
#define PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_

#include <iostream>

// #include "pillar_state/pillar_state.pb.h"
#include "pillar_state.pb.h" // TODO: scope this include pillar_state/

// TODO: not sure to make this namespace Pillar
namespace Pillar
{

class State
{
public:
  State()
  {
    std::cout << "Hello there." << std::endl;
  }
  bool has_high_ground()
  {
    std::cout << "It's over, Anakin." << std::endl;
    return true;
  }
private:
  PillarMsg::Property property_;
  PillarMsg::State state_;
};

} // Pillar

#endif // PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
