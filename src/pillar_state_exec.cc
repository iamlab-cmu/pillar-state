#include <iostream>

#include "pillar_state/pillar_state.h"

int main(int argc, char** argv)
{
  Pillar::State state;
  std::cout << state.has_high_ground() << std::endl;

  return 0;
}
