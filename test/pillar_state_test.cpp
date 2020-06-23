// #include <Eigen/Dense>
#include "gtest/gtest.h"

#include "pillar_state/pillar_state.hpp"

#include "pillar_state/proto/pillar_state.pb.h"

// using namespace pillar_state;
// using namespace Eigen;

TEST(GTest, Nominal)
{
  EXPECT_TRUE(true);
}

TEST(PillarState, Nominal)
{
  Pillar::State state;
  EXPECT_TRUE(state.has_high_ground());

  state.add_property_value_test("general", 66.);
  state.add_property_value_test("kenobi", 66.);

  PillarMsg::Property property;
  property.add_value(1.111);
  property.add_value(2.222);
  property.add_value(3.333);
  state.add_property("array", property);
  state.add_property("scalar", 4.444);
}
