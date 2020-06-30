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
  State() { }
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

    return os;
  }
private:
  PillarMsg::State state_;
};

std::ostream& operator<<(std::ostream& os, const State& state)
{
  return state.print(os);
}

} // Pillar

#endif // PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
