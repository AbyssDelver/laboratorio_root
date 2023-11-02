#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "particletype.hpp"
#include "resonancetype.hpp"
#include "particle.hpp"
#include <cstring>

TEST_CASE("testing getter funcitons") {
  // testing print polymorphism
  SUBCASE("TypeParticle getters") {
    ParticleType particle{"name", 1., 1};
    CHECK(strcmp(particle.getName(), "name") == 0);
    CHECK(particle.getMass() == 1.);
    CHECK(particle.getCharge() == 1);
  }

    SUBCASE("ResonanceType getters") {
    ResonanceType particle{"name", 1., 1, 1.};
    CHECK(strcmp(particle.getName(), "name") == 0);
    CHECK(particle.getMass() == 1.);
    CHECK(particle.getCharge() == 1);
    CHECK(particle.getWidth() == 1.);
  }

}

// TEST_CASE("getter for particle"){
  
//     SUBCASE("ResonanceType getters") {
//     ResonanceType particle{"name", 1., 1, 1.};
//     CHECK(strcmp(particle.getName(), "name") == 0);
//     CHECK(particle.getMass() == 1.);
//     CHECK(particle.getCharge() == 1);
//     CHECK(particle.getWidth() == 1.);
//   }
// }