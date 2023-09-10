#include <string>
#include <unordered_map>

namespace hyped::core {

enum class MqttTopic { kTest };

static std::unordered_map<MqttTopic, std::string> mqtt_topic_to_string
  = {{MqttTopic::kTest, "test"}};
static std::unordered_map<std::string, MqttTopic> mqtt_string_to_topic
  = {{"test", MqttTopic::kTest}};

}  // namespace hyped::core