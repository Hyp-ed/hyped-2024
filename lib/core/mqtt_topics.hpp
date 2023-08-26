#include <string>
#include <unordered_map>

namespace hyped::core {

enum class MqttTopic { kTest };

static std::unordered_map<MqttTopic, std::string> mqtt_topic_strings = {{MqttTopic::kTest, "test"}};

}  // namespace hyped::core