#ifndef PILLAR_STATE_INCLUDE_PILLAR_STATE_H_
#define PILLAR_STATE_INCLUDE_PILLAR_STATE_H_

#include <iostream>

#include <Eigen/Dense>

// TODO: not sure to make this namespace Pillar
namespace Pillar
{
// namespace E = Eigen;

class State
{
public:
  State()
  {
    std::cout << "Hello there." << std::endl;
  }
  bool has_high_ground() const
  {
    return true;
  }
private:
};

} // namespace Pillar

#endif // PILLAR_STATE_INCLUDE_PILLAR_STATE_H_
