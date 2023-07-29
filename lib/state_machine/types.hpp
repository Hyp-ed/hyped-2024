//#pragma once

#include "message.hpp"
#include "state.hpp"

#include <cstdint>

#include <boost/functional/hash/hash.hpp>

namespace hyped::state_machine {
struct SourceAndMessage {
  State source;
  Message message;

  bool operator==(const SourceAndMessage &key) const
  {
    return key.source == source && key.message == message;
  }
};

struct source_and_message_hash {
  std::size_t operator()(SourceAndMessage const &key) const
  {
    std::size_t seed = 0;
    boost::hash_combine(seed, key.source);
    boost::hash_combine(seed, key.message);
    return seed;
  }
};

}  // namespace hyped::state_machine