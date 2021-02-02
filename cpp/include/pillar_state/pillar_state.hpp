#ifndef PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
#define PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_

#include <exception>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "pillar_state/proto/pillar_state.pb.h"

namespace py = pybind11;

// TODO: not sure to make this namespace Pillar
namespace Pillar
{

class StateTreeNode
{
public:
  StateTreeNode(const std::string& name,
                const std::string& fully_qualified_parent_namespace,
                const std::unordered_set<std::string>& child_namespaces,
                bool is_literal) :
                name_(name),
                fq_parent_namespace_(fully_qualified_parent_namespace),
                child_namespaces_(child_namespaces),
                is_literal_(is_literal) { }

  std::string fully_qualified_name() const
  {
    std::string fq_name = "";
    return fq_name;
  }

  std::string name() const
  {
    return name_;
  }

  std::string fq_parent_name() const
  {
    return fq_parent_namespace_;
  }

  bool is_literal() const
  {
    return is_literal_;
  }

  bool is_property() const
  {
    return child_namespaces_.empty();
  }

  bool is_root() const
  {
    return fq_parent_namespace_.empty();
  }

  void register_child_namespace(const std::string& child_namespace)
  {
    // unordered_set will ensure that no duplicates occur
    child_namespaces_.insert(child_namespace);
  }

  std::unordered_set<std::string> child_namespaces() const
  {
    return child_namespaces_;
  }

private:
  std::string name_;
  std::string fq_parent_namespace_;
  std::unordered_set<std::string> child_namespaces_;
  bool is_literal_;
};


class State
{
public:
  State() { }

  static State create_from_yaml_file(const std::string& yaml_path)
  {
    State s;
    if (!s.load_from_yaml_file(yaml_path))
    {
      throw std::runtime_error("Could not initialize from YAML file: " + yaml_path);
    }
    return s;
  }

  static State create_from_serialized_string(const std::string& ser)
  {
    State s;
    if (!s.load_from_serialized_string(ser))
    {
      throw std::runtime_error("Could not initialize from serialized string!");
    }
    return s;
  }

  // General version with a property
  void update_property(const std::string& property_name, const PillarMsg::Property& property)
  {
    // if (property_name)
    const bool is_prop_name_new = state_.properties().count(property_name) == 0;
    (*state_.mutable_properties())[property_name] = property;

    // TODO: ideally, we would not rebuild the entire state tree as we are doing here
    if (is_prop_name_new)
    {
      build_state_tree();
    }
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
    // Clear the state object
    state_.mutable_properties()->clear();

    // Clear the state tree
    clear_state_tree();
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

  // Return the names of the literals
  std::unordered_set<std::string> literals() const
  {
    return literal_fqnames_;
  }

  std::unordered_set<std::string> get_literal_prop_names(const std::string& literal) const
  {
    const auto it = literal_fqnames_.find(literal);
    std::unordered_set<std::string> literal_prop_names;
    if (it != literal_fqnames_.cend())
    {
      literal_prop_names = get_prop_names_from_node_name(literal);
    }

    return literal_prop_names;
  }

  // Return the names of the top-level nodes
  std::unordered_set<std::string> root_nodes() const
  {
    return root_node_names_;
  }

  std::unordered_set<std::string> get_prop_names() const
  {
    std::unordered_set<std::string> prop_names;
    for (auto& key : state_.properties())
    {
      prop_names.insert(key.first);
    }
    return prop_names;
  }

  int get_prop_size(const std::string prop_name) const
  {
    auto props = state_.properties();
    return props[prop_name].values_size();
  }

  std::unordered_map<std::string, int> get_prop_sizes(const std::vector<std::string> prop_names) const
  {
    std::unordered_map<std::string, int> prop_sizes;
    for (auto& prop_name : prop_names)
    {
      prop_sizes[prop_name] = get_prop_size(prop_name);
    }
    return prop_sizes;
  }

  std::unordered_map<std::string, std::pair<int, int>> get_vec_idxs(const std::vector<std::string> prop_names) const
  {
    auto prop_sizes = get_prop_sizes(prop_names);

    std::unordered_map<std::string, std::pair<int, int>> idxs;
    int start_idx = 0;
    int end_idx = 0;
    for (auto& prop_name : prop_names)
    {
      end_idx = start_idx + prop_sizes[prop_name];
      idxs[prop_name] = std::make_pair(start_idx, end_idx);
      start_idx = end_idx;
    }

    return idxs;
  }

  size_t get_total_prop_sizes(const std::vector<std::string> prop_names) const
  {
    size_t total_size = 0;
    for (const auto& prop_name : prop_names)
    {
      total_size += get_prop_size(prop_name);
    }
    return total_size;
  }

  std::vector<double> get_values_as_vec(const std::vector<std::string> prop_names) const
  {
    std::vector<double> flattened_values;
    flattened_values.resize(get_total_prop_sizes(prop_names), 0.);

    auto props = state_.properties();
    int offset = 0;
    for (const auto& prop_name : prop_names)
    {
      auto values = props[prop_name].values();
      std::copy(values.begin(), values.end(), flattened_values.begin() + offset);
      offset += values.size();
    }

    return flattened_values;
  }

  std::vector<std::string> get_value_names_as_vec(const std::vector<std::string> prop_names) const
  {
    std::vector<std::string> flattened_value_names;
    flattened_value_names.resize(get_total_prop_sizes(prop_names), "");

    auto props = state_.properties();
    int offset = 0;
    for (const auto& prop_name : prop_names)
    {
      auto value_names = props[prop_name].value_names();
      auto values_size = props[prop_name].values().size();
      if (value_names.size() == values_size)
      {
        std::copy(value_names.begin(), value_names.end(), flattened_value_names.begin() + offset);
      }
      offset += values_size;
    } 

    return flattened_value_names;
  }

  bool set_values_from_vec(const std::vector<std::string> prop_names, const std::vector<double> values)
  {
    if (get_total_prop_sizes(prop_names) != values.size())
    {
      return false;
    }

    auto prop_idxs = get_vec_idxs(prop_names);
    for (const auto& prop_name : prop_names)
    {
      auto idxs = prop_idxs[prop_name];
      update_property(prop_name, {values.begin() + idxs.first, values.begin() + idxs.second});
    }

    return true;
  }

  /* TODO: write later
  bool save_to_yaml_file(const std::string& yaml_path, bool force_overwrite = false)
  {
    // Attempt to open file for writing

    // Emit to YAML
    return false;
  }
  */

  py::bytes get_serialized_string() const
  {
    std::string ser;
    if (!state_.SerializeToString(&ser))
    {
      throw std::runtime_error("Could not serialize to string!");
    }
    return py::bytes(ser);
  }

  // Updates the state from a yaml file
  bool update_from_yaml_file(const std::string& yaml_path)
  {
    bool success = false;

    // TODO: allow for different yaml structures. this format
    // assumes a dict of fully qualified property names and properties
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

  std::ostream& print(std::ostream& os, int format=1) const
  {
    if (format == 0)
    {
      return print_as_property_list(os);
    }
    else if (format == 1)
    {
      return print_as_state_tree(os);
    }
    else
    {
      // TODO: warning? error?
    }
    return os;
  }

  std::string get_print_str(int format=1) const
  {
     std::ostringstream stream;
     print(stream, format);
     return stream.str();
  }

  // Low-level printer for values of a property. Does not include new lines.
  std::ostream& print_property_values(std::ostream& os, const PillarMsg::Property& property) const
  {
    const auto prop_value_size = property.values_size();
    const auto prop_value_name_size = property.value_names_size();
    const bool named_values = (prop_value_size == prop_value_name_size) ? true : false;

    os << " -> val: [ ";

    for (auto n = 0; n < prop_value_size; ++n)
    {
      if (named_values)
      {
        os << "\'" << property.value_names(n) << "\': ";
      }

      os << property.values(n);
      if ((n + 1) == prop_value_size)
      {
        os << " ]";
      }
      else
      {
        os << ", ";
      }
    }

    return os;
  }

  // Low-level printer for variance of a property. Does not include new lines.
  std::ostream& print_property_variances(std::ostream& os, const PillarMsg::Property& property) const
  {
    const auto prop_var_size = property.variances_size();

    os << " -> var: [ ";

    for (auto n = 0; n < prop_var_size; ++n)
    {
      os << property.variances(n);
      if ((n + 1) == prop_var_size)
      {
        os << " ]";
      }
      else
      {
        os << ", ";
      }
    }

    return os;
  }

  std::ostream& print_as_property_list(std::ostream& os) const
  {
    // This format will iterate through all the properties
    if (num_properties() > 0)
    {
      for (const auto& p : state_.properties())
      {
        os << p.first << ":";

        const auto this_prop = p.second;
        const auto this_prop_value_size = this_prop.values_size();
        const auto this_prop_var_size = this_prop.variances_size();

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
          print_property_values(os, this_prop);
          os << std::endl;
        }

        // Print variance
        if (this_prop_var_size > 0)
        {
          print_property_variances(os, this_prop);
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

  // This format will print things as a tree
  std::ostream& print_as_state_tree(std::ostream& os) const
  {
    if (num_properties() > 0)
    {
      for (const auto& r : root_node_names_)
      {
        print_state_tree_from_node(os, r, 0);
      }
    }
    else
    {
      os << "n/a" << std::endl;
    }

    return os;
  }

  std::ostream& print_state_tree_from_node(std::ostream& os, const std::string& fqnode, int depth) const
  {
    if (namespace_map_from_fqname_.count(fqnode) == 0)
    {
      return os;
    }

    const auto state_node = namespace_map_from_fqname_.at(fqnode);
    const std::string spaces = std::string(3*depth, ' ');

    const auto delim = (state_node.is_literal()) ? ":" : "/";
    const auto literal_mark = (state_node.is_literal()) ? "*" : "";

    os << spaces << state_node.name() << literal_mark << std::endl;

    if (state_node.is_property())
    {
      const auto this_prop = state_.properties().at(fqnode);
      const auto this_prop_value_size = this_prop.values_size();
      const auto this_prop_var_size = this_prop.variances_size();

      if (this_prop_value_size || this_prop_var_size)
      {
        // Print values
        if (this_prop_value_size > 0)
        {
          os << spaces;
          print_property_values(os, this_prop);
          os << std::endl;
        }

        // Print variance
        if (this_prop_var_size > 0)
        {
          os << spaces;
          print_property_variances(os, this_prop);
          os << std::endl;
        }
      }
      else
      {
        // If there isn't anything meaningful to print, just print n/a and skip
        os << spaces << " n/a" << std::endl;
      }
    }

    for (const auto& child_node : state_node.child_namespaces())
    {
      // form fully qualified child
      const auto fqchild = fqnode + delim + child_node;
      print_state_tree_from_node(os, fqchild, depth + 1);
    }

    return os;
  }

private:

  // Clears the state and loads file
  bool load_from_yaml_file(const std::string& yaml_path)
  {
    clear();
    if (!update_from_yaml_file(yaml_path))
    {
      // Clear so we don't end up in a weird intermediate state
      clear();
      return false;
    }

    // Rebuild search tree
    if (!build_state_tree())
    {
      // Clear so we don't end up in a weird intermediate state
      clear();
      return false;
    }

    return true;
  }

  bool load_from_serialized_string(const std::string& ser)
  {
    clear();
    if (!state_.ParseFromString(ser))
    {
      clear();
      return false;
    }

    if (!build_state_tree())
    {
      // Clear so we don't end up in a weird intermediate state
      clear();
      return false;
    }

    return true;
  }

  // TODO: change name. low-level function. HAS NO ERROR CHECKING.
  std::unordered_set<std::string> get_prop_names_from_node_name(const std::string& node, bool follow_literals = false) const
  {
    std::unordered_set<std::string> node_properties_found;
    const auto ref = namespace_map_from_fqname_.at(node);
    if (ref.is_property())
    {
      node_properties_found.insert(node);
    }
    else
    {
      const auto delim = (ref.is_literal()) ? ":" : "/";
      for (const auto& child_node : ref.child_namespaces())
      {
        const auto fqchild = node + delim + child_node;
        // const auto is_child_literal = namespace_map_from_fqname_.at(fqchild).is_literal();
        const bool follow_node = (follow_literals) ? true : !namespace_map_from_fqname_.at(fqchild).is_literal();
        // if (follow_literals || !is_child_literal)
        if (follow_node)
        {
          const auto out = get_prop_names_from_node_name(fqchild);
          for (const auto& m : out)
          {
            node_properties_found.insert(m);
          }
        }
      }
    }

    return node_properties_found;
  }

  // Clears the state tree
  void clear_state_tree()
  {
    // Clear the namespace tree
    namespace_map_from_fqname_.clear();

    // Clear the literals
    literal_fqnames_.clear();

    // Clear the root node names
    root_node_names_.clear();
  }

  // Traverses the state dictionary to build a tree to do useful things. Clears tree first.
  bool build_state_tree()
  {
    // Clears the state tree first
    clear_state_tree();

    for (const auto& p : state_.properties())
    {
      const auto fq_prop_name = p.first;

      // Split property name
      const std::string delimiters {":/"};

      size_t begin = 0;
      size_t current_pos = 0;
      size_t last_pos = std::string::npos;

      std::vector<std::string> namespaces;
      std::vector<std::string> fully_qualified_namespaces;
      std::vector<std::string> fully_qualified_parent_namespaces;
      std::vector<bool> is_namespace_literal_vec;
      while ((begin = fq_prop_name.find_first_not_of(delimiters, current_pos)) != std::string::npos)
      {
        current_pos = fq_prop_name.find_first_of(delimiters, begin + 1);
        auto this_namespace_name = fq_prop_name.substr(begin, current_pos - begin);
        namespaces.push_back(this_namespace_name);

        auto this_fq_namespace_name = fq_prop_name.substr(0, current_pos);
        fully_qualified_namespaces.push_back(this_fq_namespace_name);

        auto this_fq_parent_name = (last_pos != std::string::npos) ? fq_prop_name.substr(0, begin - 1) : "";
        fully_qualified_parent_namespaces.push_back(this_fq_parent_name);
        last_pos = begin;

        // Check delimiter to see whether this namespace represents a literal namespace
        // Assume the last namespace is never a literal
        bool is_namespace_literal = false;
        if (current_pos != std::string::npos)
        {
          auto delimiter = fq_prop_name.at(current_pos);

          if (delimiter == ':')
          {
            is_namespace_literal = true;
          }
        }

        is_namespace_literal_vec.push_back(is_namespace_literal);
      }

      // Create tree
      for (size_t n = 0; n < namespaces.size(); ++n)
      {
        auto this_fq_namespace = fully_qualified_namespaces[n];
        auto this_local_name = namespaces[n];

        bool is_literal = is_namespace_literal_vec[n];
        auto this_fq_parent = fully_qualified_parent_namespaces[n];

        if (namespace_map_from_fqname_.count(this_fq_namespace))
        {
          // entry exists already, so just update children if we aren't at the end
          if ((n + 1) < namespaces.size())
          {
            auto child_namespace = namespaces[n+1];
            namespace_map_from_fqname_.at(this_fq_namespace).register_child_namespace(child_namespace);
          }

          // assert that the fully qualified parent
          if (namespace_map_from_fqname_.at(this_fq_namespace).fq_parent_name() != this_fq_parent)
          {
            return false;
          }

          // assert that the literal is the same
          if (namespace_map_from_fqname_.at(this_fq_namespace).is_literal() != is_literal)
          {
            return false;
          }
        }
        else
        {
          // create new entry
          std::unordered_set<std::string> child_namespaces;
          if ((n + 1) < namespaces.size())
          {
            auto child_namespace = namespaces[n+1];
            child_namespaces.insert(child_namespace);
          }
          auto temp = StateTreeNode(this_local_name, this_fq_parent, child_namespaces, is_literal);
          namespace_map_from_fqname_.insert({this_fq_namespace, temp});

          // Also add here since we're already processing stuff
          if (is_literal)
          {
            literal_fqnames_.insert(this_fq_namespace);
          }

          if (this_fq_parent.empty())
          {
            root_node_names_.insert(this_fq_namespace);
          }
        }
      }
    }

    return true;
  }

  // This is the underlying object that contains the data
  PillarMsg::State state_;

  // This is a tree to parse the namespaces of the state
  std::unordered_map<std::string, StateTreeNode> namespace_map_from_fqname_;

  // This is a handy set of all literals
  std::unordered_set<std::string> literal_fqnames_;

  // This is a handy set of all root nodes
  std::unordered_set<std::string> root_node_names_;
};


std::ostream& operator<<(std::ostream& os, const State& state)
{
  return state.print(os);
}

} // Pillar

#endif // PILLAR_STATE_INCLUDE_PILLAR_STATE_HPP_
