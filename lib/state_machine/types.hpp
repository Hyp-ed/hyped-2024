#pragma once

#include "state.hpp"

#include <boost/functional/hash/hash.hpp>

namespace hyped::state_machine {
struct SourceAndTarget {
  State source;
  State target;

  bool operator==(const SourceAndTarget &key) const
  {
    return key.source == source && key.target == target;
  }
};

struct source_and_target_hash {
  std::size_t operator()(SourceAndTarget const &key) const
  {
    std::size_t seed = 0;
    boost::hash_combine(seed, key.source);
    boost::hash_combine(seed, key.target);
    return seed;
  }
};

}  // namespace hyped::state_machine
