#ifndef PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
#define PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "pillar_state/proto/pillar_state.pb.h"

// TODO: not sure to make this namespace Pillar
namespace Pillar
{

class State
{
public:
  State() { }

  // General version with a property
  void update_property(const std::string& property_name, const PillarMsg::Property& property)
  {
    (*state_.mutable_properties())[property_name] = property;
  }

  // Scalar version
  void update_property(const std::string& property_name, double value)
  {
    PillarMsg::Property property;
    property.add_value(value);
    update_property(property_name, property);
  }
  void update_property(const std::string& property_name, double value, double variance)
  {
    PillarMsg::Property property;
    property.add_value(value);
    property.add_variance(variance);
    update_property(property_name, property);
  }

  // Array version
  void update_property(const std::string& property_name, const std::vector<double>& values)
  {
    // create a property
    PillarMsg::Property property;
    *property.mutable_value() = {values.cbegin(), values.cend()};
    update_property(property_name, property);
  }
  void update_property(const std::string& property_name, const std::vector<double>& values, const std::vector<double>& variances)
  {
    // create a property
    PillarMsg::Property property;
    *property.mutable_value() = {values.cbegin(), values.cend()};
    *property.mutable_variance() = {variances.cbegin(), variances.cend()};
    update_property(property_name, property);
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

        // Print values
        os << " -> val: ";
        const auto this_prop_value_size = p.second.value_size();
        if (this_prop_value_size > 0)
        {
          os << "[ ";
          for (auto n = 0; n < this_prop_value_size; ++n)
          {
            os << p.second.value(n);
            if ((n + 1) == this_prop_value_size)
            {
              os << " ]";
            }
            else
            {
              os << ", ";
            }
          }
        }
        else
        {
          os << "n/a";
        }
        os << std::endl;

        // Print variance
        os << " -> var: ";
        const auto this_prop_var_size = p.second.variance_size();
        if (this_prop_var_size > 0)
        {
          os << "[ ";
          for (auto n = 0; n < this_prop_var_size; ++n)
          {
            os << p.second.variance(n);
            if ((n + 1) == this_prop_var_size)
            {
              os << " ]";
            }
            else
            {
              os << ", ";
            }
          }
        }
        else
        {
          os << "n/a";
        }
        os << std::endl;
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
