#pragma once

#include <functional>
#include <optional>
#include <string>

#include <rapidjson/document.h>

#include <core/types.hpp>

namespace hyped::debug {
class ICommand {
 public:
  ICommand(std::string name, std::string description, std::function<core::Result()> handler)
      : name_(name),
        description_(description),
        handler_(handler)
  {
  }
  std::string getName() { return name_; }
  std::string getDescription() { return description_; }
  void execute() { handler_(); }

  static std::optional<std::unique_ptr<ICommand>> parse(
    rapidjson::GenericObject<true, rapidjson::Value> config);

 private:
  std::string name_;
  std::string description_;
  std::function<void()> handler_;
};
}  // namespace hyped::debug
