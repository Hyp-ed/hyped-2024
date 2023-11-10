#pragma once

#include <functional>
#include <optional>
#include <string>

#include <rapidjson/document.h>

#include <core/types.hpp>

namespace hyped::debug {
class Command {
 public:
  Command(const std::string &name,
          const std::string &description,
          const std::function<void()> handler)
      : name_(name),
        description_(description),
        handler_(handler)
  {
  }
  std::string getName() { return name_; }
  std::string getDescription() { return description_; }
  void execute() { handler_(); }

 private:
  const std::string name_;
  const std::string description_;
  const std::function<void()> handler_;
};
}  // namespace hyped::debug
