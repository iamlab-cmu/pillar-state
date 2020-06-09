#include <Eigen/Dense>
#include "gtest/gtest.h"

#include "pillar_state/pillar_state.h"

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
}
