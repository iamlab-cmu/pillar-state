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
  // TODO: don't hardcode path of this file
  const std::string pillar_env_yaml_path = "/home/telee/ws/pillar-state/test/env_3room_state.yaml";
  std::cout << "Reading in: " << pillar_env_yaml_path << std::endl;
  Pillar::State state(pillar_env_yaml_path);
  std::cout << state << std::endl;

  const std::string door_yaml_path = "/home/telee/ws/pillar-state/test/door_state.yaml";
  std::cout << "Reading in: " << door_yaml_path << std::endl;
  state.load_from_yaml_file(door_yaml_path);
  std::cout << state << std::endl;

  EXPECT_TRUE(true);
}
