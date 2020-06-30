// #include <Eigen/Dense>
#include "gtest/gtest.h"

#include "pillar_state/pillar_state.hpp"

#include "pillar_state/proto/pillar_state.pb.h"

// using namespace pillar_state;
// using namespace Eigen;

TEST(PillarState, Nominal)
{
  PillarMsg::Property obj1_property;
  obj1_property.add_value(1.111);
  obj1_property.add_value(2.222);
  obj1_property.add_value(3.333);
  PillarMsg::Property obj2_property;
  obj2_property.add_value(-8.888);
  obj2_property.add_value(9.999);
  PillarMsg::Property obj2_property_scalar_array;
  obj2_property_scalar_array.add_value(123.123);
  state.add_property("object0/scalar", 9.999);
  state.add_property("object1/array", obj1_property);
  state.add_property("object1/scalar", 4.444);
  state.add_property("object2/prop0/scalar", -22.222);
  state.add_property("object2/prop0/array", obj2_property);
  state.add_property("object2/prop1/scalar", 7.777);
  state.add_property("object2/prop2/array", obj2_property_scalar_array);

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
}

TEST(PillarState, PrintEmpty)
{
  // These should still work, although they are probably not used in practice
  Pillar::State state;

  std::cout << "What does it look like to print an empty state?" << std::endl;
  std::cout << state << std::endl;

  PillarMsg::Property prop;
  state.add_property("empty_property", prop);

  std::cout << "What does it look like to print an empty state property?" << std::endl;
  std::cout << state << std::endl;

  EXPECT_TRUE(true);
}
