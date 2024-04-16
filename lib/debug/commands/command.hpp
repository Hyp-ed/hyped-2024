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
          const std::string &usage,
          const std::function<void(std::vector<std::string>)> handler)
      : name_(name),
        description_(description),
        usage_(usage),
        handler_(handler)
  {
  }
  std::string getName() { return name_; }
  std::string getDescription() { return description_; }
  std::string getUsage() { return usage_; }
  void execute(std::vector<std::string> &args) { handler_(args); }

 private:
  const std::string name_;
  const std::string description_;
  const std::string usage_;
  const std::function<void(std::vector<std::string>)> handler_;
};

}  // namespace hyped::debug
