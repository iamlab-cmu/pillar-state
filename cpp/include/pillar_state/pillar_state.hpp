#ifndef PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
#define PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "pillar_state/proto/pillar_state.pb.h"

// TODO: not sure to make this namespace Pillar
namespace Pillar
{

class State
{
public:
  State() { }
  State(const std::string& yaml_path)
  {
    bool success = load_from_yaml_file(yaml_path);

    // Loading in a ctor requires us to throw because we don't want an "is valid" member field
    // This is because the ctor can't return the outcome of load_from_yaml_file
    if (!success)
    {
      throw std::runtime_error("Could not initialize from YAML file: " + yaml_path);
    }
  }

  // General version with a property
  void update_property(const std::string& property_name, const PillarMsg::Property& property)
  {
    (*state_.mutable_properties())[property_name] = property;
  }

  // Scalar version
  void update_property(const std::string& property_name, double value)
  {
    PillarMsg::Property property;
    property.add_values(value);
    update_property(property_name, property);
  }
  void update_property(const std::string& property_name, double value, double variance)
  {
    PillarMsg::Property property;
    property.add_values(value);
    property.add_variances(variance);
    update_property(property_name, property);
  }

  // Array version
  void update_property(const std::string& property_name, const std::vector<double>& values)
  {
    // create a property
    PillarMsg::Property property;
    *property.mutable_values() = {values.cbegin(), values.cend()};
    update_property(property_name, property);
  }
  void update_property(const std::string& property_name, const std::vector<double>& values, const std::vector<double>& variances)
  {
    // create a property
    PillarMsg::Property property;
    *property.mutable_values() = {values.cbegin(), values.cend()};
    *property.mutable_variances() = {variances.cbegin(), variances.cend()};
    update_property(property_name, property);
  }
  void update_property(const std::string& property_name, const std::vector<double>& values, const std::vector<std::string>& value_names)
  {
    // TODO: consider whether to throw if the value lengths and name lengths don't match
    PillarMsg::Property property;
    *property.mutable_values() = {values.cbegin(), values.cend()};
    *property.mutable_value_names() = {value_names.cbegin(), value_names.cend()};
    update_property(property_name, property);
  }
  void update_property(const std::string& property_name, const std::vector<double>& values, const std::vector<std::string>& value_names, const std::vector<double>& variances)
  {
    // TODO: consider whether to throw if the value lengths and name lengths don't match
    PillarMsg::Property property;
    *property.mutable_values() = {values.cbegin(), values.cend()};
    *property.mutable_value_names() = {value_names.cbegin(), value_names.cend()};
    *property.mutable_variances() = {variances.cbegin(), variances.cend()};
    update_property(property_name, property);
  }
  // TODO: verion with variance names. but this might be challenging to define exactly how to do that.
  // TODO: are variance names even needed? could determine from value names, perhaps.
  // TODO: e.g., var_xx, var_xy, var_yx, var_yy
  // TODO: that would assume a certain row- or column-major order
  // TODO: that also assumes a Gaussian variance

  // Clears the state
  void clear()
  {
    state_.mutable_properties()->clear();
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
      num_dimensions += it->second.values_size();
    }

    return num_dimensions;
  }
  // Wrapper for num_dimensions -- although this is DIFFERENT than the size of the map
  // So perhaps size() here could be misleading
  int size() const
  {
    return num_dimensions();
  }

  // TODO: write later
  bool save_to_yaml_file(const std::string& yaml_path, bool force_overwrite = false)
  {
    // Attempt to open file for writing

    // Emit to YAML
    return false;
  }

  // Clears the state and loads file
  bool load_from_yaml_file(const std::string& yaml_path)
  {
    clear();
    return update_from_yaml_file(yaml_path);
  }

  // Updates the state from a yaml file
  bool update_from_yaml_file(const std::string& yaml_path)
  {
    bool success = false;
    try
    {
      const YAML::Node config = YAML::LoadFile(yaml_path);
      // TODO: maybe allow specification of a different key
      const YAML::Node state_config = config["state"];

      for (const auto property : state_config)
      {
        YAML::const_iterator it = property.begin();

        const std::string property_name = it->first.as<std::string>();
        const auto property_fields = it->second;

        std::vector<double> values;
        std::vector<std::string> value_names;
        std::vector<double> variances;

        for (const auto f : property_fields)
        {
          if (f["values"])
          {
            if (f["values"].Type() == YAML::NodeType::Scalar)
            {
              values.push_back(f["values"].as<double>());
            }
            else
            {
              values = f["values"].as<std::vector<double>>();
            }
          }
          if (f["variances"])
          {
            if (f["variances"].Type() == YAML::NodeType::Scalar)
            {
              variances.push_back(f["variances"].as<double>());
            }
            else
            {
              variances = f["variances"].as<std::vector<double>>();
            }
          }
          if (f["value_names"])
          {
            if (f["value_names"].Type() == YAML::NodeType::Scalar)
            {
              value_names.push_back(f["value_names"].as<std::string>());
            }
            else
            {
              value_names = f["value_names"].as<std::vector<std::string>>();
            }
          }
        }

        update_property(property_name, values, value_names, variances);
      }

      success = true;
    }
    catch (YAML::Exception &e)
    {
      const std::string yaml_error = e.what();
      // TODO: error ostream?
      std::cout << "YAML error: " << yaml_error << std::endl;
    }
    return success;
  }

  std::ostream& print(std::ostream& os) const
  {
    // Iterate through all the property keys
    if (num_properties() > 0)
    {
      for (const auto& p : state_.properties())
      {
        os << p.first << ":";

        const auto this_prop_value_size = p.second.values_size();
        const auto this_prop_var_size = p.second.variances_size();

        // If there isn't anything meaningful to print, just print n/a and skip
        if (this_prop_value_size == 0 && this_prop_var_size == 0)
        {
          os << " n/a" << std::endl;
          continue;
        }
        else
        {
          os << std::endl;
        }

        // Print values
        if (this_prop_value_size > 0)
        {
          os << " -> val: [ ";

          const auto this_prop_value_name_size = p.second.value_names_size();
          bool named_values = (this_prop_value_size == this_prop_value_name_size) ? true : false;

          for (auto n = 0; n < this_prop_value_size; ++n)
          {
            if (named_values)
            {
              os << "\'" << p.second.value_names(n) << "\': ";
            }

            os << p.second.values(n);
            if ((n + 1) == this_prop_value_size)
            {
              os << " ]";
            }
            else
            {
              os << ", ";
            }
          }
          os << std::endl;
        }

        // Print variance
        if (this_prop_var_size > 0)
        {
          os << " -> var: [ ";
          for (auto n = 0; n < this_prop_var_size; ++n)
          {
            os << p.second.variances(n);
            if ((n + 1) == this_prop_var_size)
            {
              os << " ]";
            }
            else
            {
              os << ", ";
            }
          }
          os << std::endl;
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
