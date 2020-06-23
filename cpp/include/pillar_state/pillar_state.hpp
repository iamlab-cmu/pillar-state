#ifndef PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
#define PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_

#include <iostream>

#include "pillar_state/proto/pillar_state.pb.h"

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
  bool has_high_ground() const
  {
    std::cout << "It's over, Anakin." << std::endl;
    return true;
  }
  void add_property_value_test(const std::string& property_name, double property_value)
  {
    std::cout << "add_property_test\n" << property_name << std::endl << property_value << std::endl;
    PillarMsg::Property property_test;
    std::cout << "property_test.value_size(): " << property_test.value_size() << std::endl;
    std::cout << "property_test.variance_size(): " << property_test.variance_size() << std::endl;
    property_test.add_value(property_value);
    std::cout << "property_test.value_size(): " << property_test.value_size() << std::endl;
    std::cout << "property_test.variance_size(): " << property_test.variance_size() << std::endl;
  }
  void add_property(const std::string& property_name, const PillarMsg::Property& property)
  {
    //(*state_)[property_name] = property;
    //state_.properties[property_name] = property;
    (*state_.mutable_properties())[property_name] = property;
  }
  void add_property(const std::string& property_name, double value)
  {
    // scalar version
    PillarMsg::Property property;
    property.add_value(value);
    add_property(property_name, property);
  }
  int num_properties() const
  {
    return state_.properties().size();
  }
private:
  PillarMsg::Property property_;
  PillarMsg::State state_;
};

} // Pillar

#endif // PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
