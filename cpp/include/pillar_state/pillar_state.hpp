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
  // Returns the *number of state properties*
  int num_properties() const
  {
    return state_.properties().size();
  }
  // Returns the *number of state elements*
  int num_dimensions() const
  {
    int num_dimensions = 0;

    for (auto it = state_.properties().cbegin(); it != state_.properties().cend(); ++it)
    {
      num_dimensions += it->second.value_size();
    }

    return num_dimensions;
  }
  // Wrapper for num_dimensions -- although this is DIFFERENT than the size of the map
  // So perhaps size() here could be misleading
  int size() const
  {
    return num_dimensions();
  }
  std::ostream& print(std::ostream& os) const
  {
    // Iterate through all the property keys
    if (num_properties() > 0)
    {
      for (const auto& p : state_.properties())
      {
        os << p.first << ":" << std::endl;

        if (p.second.value_size() > 0)
        {
          for (const auto &v : p.second.value())
          {
            os << " -> " << v << std::endl;
          }
        }
        else
        {
          os << " -> n/a" << std::endl;
        }
      }
    }
    else
    {
      os << "n/a" << std::endl;
    }

    // return os << "hay\n";
    return os;
  }
private:
  PillarMsg::Property property_;
  PillarMsg::State state_;
};

std::ostream& operator<<(std::ostream& os, const State& state)
{
  return state.print(os);
}

} // Pillar

#endif // PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
