#include <algorithm>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "pillar_state/pillar_state.hpp"
#include "pillar_state/proto/pillar_state.pb.h"

TEST(PillarState, Nominal)
{
  Pillar::State state;

  state.update_property("object0/scalar", 9.999);
  state.update_property("object1/array", {1.111, 2.222, 3.333});
  state.update_property("object1/scalar", 4.444);
  state.update_property("object2/prop0/scalar", -22.222);
  state.update_property("object2/prop0/array", {-8.888, 9.999});
  state.update_property("object2/prop1/scalar", 7.777);
  state.update_property("object2/prop2/array", {123.123});

  std::cout << "What does it look like to print out state? " << std::endl;
  std::cout << state << std::endl;

  std::cout << "state.num_properties(): " << state.num_properties() << std::endl;
  std::cout << "state.num_dimensions(): " << state.num_dimensions() << std::endl;
  std::cout << "state.size(): " << state.size() << std::endl;

  int num_state_properties_gt = 7;
  int num_state_dimensions_gt = 10;

  ASSERT_EQ(state.num_properties(), num_state_properties_gt);
  ASSERT_EQ(state.num_dimensions(), num_state_dimensions_gt);
  ASSERT_EQ(state.size(), num_state_dimensions_gt);

  // Now clear the state
  state.clear();

  std::cout << "What happens after running clear()?" << std::endl;
  std::cout << "state.num_properties(): " << state.num_properties() << std::endl;
  std::cout << "state.num_dimensions(): " << state.num_dimensions() << std::endl;
  std::cout << "state.size(): " << state.size() << std::endl;

  ASSERT_EQ(state.num_properties(), 0);
  ASSERT_EQ(state.num_dimensions(), 0);
  ASSERT_EQ(state.size(), 0);
}

TEST(PillarState, PrintEmpty)
{
  // These should still work, although they are probably not used in practice
  Pillar::State state;

  std::cout << "What does it look like to print an empty state?" << std::endl;
  std::cout << state << std::endl;

  PillarMsg::Property prop;
  state.update_property("empty_property", prop);

  std::cout << "What does it look like to print an empty state property?" << std::endl;
  std::cout << state << std::endl;

  EXPECT_TRUE(true);
}

TEST(PillarState, Variance)
{
  Pillar::State state;

  state.update_property("object0/scalar", 1.111, 0.555);
  state.update_property("object0/array", {1.111, 2.222}, {0.111, 0.222, 0.333, 0.444});

  std::cout << state << std::endl;
}

TEST(PillarState, StateFromYamlFile)
{
  const std::string pillar_env_yaml_path = "test/env_3room_state.yaml";
  std::cout << "Reading in: " << pillar_env_yaml_path << std::endl;
  Pillar::State state = Pillar::State::create_from_yaml_file(pillar_env_yaml_path);
  std::cout << state << std::endl;

  const std::string door_yaml_path = "test/door_state.yaml";
  std::cout << "Reading in: " << door_yaml_path << std::endl;
  state.load_from_yaml_file(door_yaml_path);
  std::cout << state << std::endl;

  EXPECT_TRUE(true);
}

TEST(PillarState, LiteralsAndNodes)
{
  const std::string door_yaml_path = "test/door_state.yaml";
  Pillar::State state = Pillar::State::create_from_yaml_file(door_yaml_path);

  const std::vector<std::string> literal_names_abc_gt = {"desk","frame","frame:door","frame:door:handle"};
  const auto literal_names = state.literals();

  // Copy and sort
  std::vector<std::string> literal_names_abc(literal_names.begin(), literal_names.end());
  std::sort(literal_names_abc.begin(), literal_names_abc.end());

  std::cout << "State literals:" << std::endl;
  for (const auto n : literal_names_abc)
  {
    std::cout << " -> " << n << std::endl;
  }
  EXPECT_EQ(literal_names_abc, literal_names_abc_gt);

  // Root nodes
  const std::vector<std::string> root_nodes_abc_gt = {"calibration_array", "constants", "desk", "frame"};
  const auto root_nodes = state.root_nodes();

  // Copy and sort
  std::vector<std::string> root_nodes_abc(root_nodes.begin(), root_nodes.end());
  std::sort(root_nodes_abc.begin(), root_nodes_abc.end());

  std::cout << "Root nodes:" << std::endl;
  for (const auto n : root_nodes_abc)
  {
    std::cout << " -> " << n << std::endl;
  }
  EXPECT_EQ(root_nodes_abc, root_nodes_abc_gt);

  // Get property names of a single literal
  // Note that this should NOT return any properties from any child literals
  const std::vector<std::string> frame_door_literal_props_abc_gt = {
    "frame:door:color",
    "frame:door:pose/position",
    "frame:door:pose/quaternion"
    };
  const auto frame_door_literal_props = state.get_literal_prop_names("frame:door");
  std::vector<std::string> frame_door_literal_props_abc(frame_door_literal_props.begin(), frame_door_literal_props.end());
  std::sort(frame_door_literal_props_abc.begin(), frame_door_literal_props_abc.end());

  std::cout << "Properties of literal frame:door:" << std::endl;
  for (const auto n : frame_door_literal_props_abc)
  {
    std::cout << " -> " << n << std::endl;
  }
  EXPECT_EQ(frame_door_literal_props_abc, frame_door_literal_props_abc_gt);
}

TEST(PillarState, SerializeAndDeserialize)
{
  const std::string pillar_env_yaml_path = "test/env_3room_state.yaml";
  Pillar::State state = Pillar::State::create_from_yaml_file(pillar_env_yaml_path);

  std::string ser = state.get_serialized_string();
  Pillar::State new_state = Pillar::State::create_from_seralized_string(ser);

  EXPECT_EQ(state.num_properties(), new_state.num_properties());
  EXPECT_EQ(state.num_dimensions(), new_state.num_dimensions());
}


TEST(PillarState, ReadFlattenedValues)
{
  const std::string pillar_env_yaml_path = "test/env_3room_state.yaml";
  Pillar::State state = Pillar::State::create_from_yaml_file(pillar_env_yaml_path);

  std::vector<std::string> prop_names = {
    "wall1:length", "wall1:pose2d", "landmark2:pose2d"
  };

  auto prop_sizes = state.get_prop_sizes(prop_names);
  EXPECT_EQ(prop_sizes["wall1:length"], 1);
  EXPECT_EQ(prop_sizes["wall1:pose2d"], 2);
  EXPECT_EQ(prop_sizes["landmark2:pose2d"], 2);

  auto flattened_idxs = state.get_flattened_idxs(prop_names);
  EXPECT_EQ(flattened_idxs["wall1:length"].first, 0);
  EXPECT_EQ(flattened_idxs["wall1:length"].second, 1);
  EXPECT_EQ(flattened_idxs["wall1:pose2d"].first, 1);
  EXPECT_EQ(flattened_idxs["wall1:pose2d"].second, 3);
  EXPECT_EQ(flattened_idxs["landmark2:pose2d"].first, 3);
  EXPECT_EQ(flattened_idxs["landmark2:pose2d"].second, 5);

  auto flattened_values = state.get_flattened_values(prop_names);
  std::vector<double> gt_values = {80, 40, 0, 23, 2};
  for (size_t i = 0; i < flattened_values.size(); ++i) 
  {
    EXPECT_EQ(flattened_values[i], gt_values[i]);
  }

  auto flattened_value_names = state.get_flattened_value_names(prop_names);
  std::vector<std::string> gt_value_names = {"", "x", "y", "x", "y"};
  for (size_t i = 0; i < flattened_value_names.size(); ++i) 
  {
    EXPECT_EQ(flattened_values[i], gt_values[i]);
  }

}

TEST(PillarState, WriteFlattenedValues)
{
  const std::string pillar_env_yaml_path = "test/env_3room_state.yaml";
  Pillar::State state = Pillar::State::create_from_yaml_file(pillar_env_yaml_path);

  std::vector<std::string> prop_names = {
    "wall1:length", "wall1:pose2d", "landmark2:pose2d"
  };

  auto flattened_values = state.get_flattened_values(prop_names);
  for (size_t i = 0; i < flattened_values.size(); i++)
  {
    flattened_values[i] *= 2;
  }

  state.set_flattened_values(prop_names, flattened_values);

  auto new_flattened_values = state.get_flattened_values(prop_names);
  for (size_t i = 0; i < flattened_values.size(); ++i) 
  {
    EXPECT_EQ(new_flattened_values[i], flattened_values[i]);
  }
}