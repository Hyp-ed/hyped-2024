#include "controller.hpp"

#include <fstream>
#include <sstream>

namespace hyped::motors {

std::optional<std::shared_ptr<Controller>> Controller::create(
  core::ILogger &logger,
  const std::string &message_file_path,
  const std::shared_ptr<io::ICan> can,
  const std::shared_ptr<IFrequencyCalculator> frequency_calculator)
{
  std::ifstream input_stream(message_file_path);
  if (!input_stream.is_open()) {
    logger.log(core::LogLevel::kFatal, "Failed to open file %s", message_file_path.c_str());
    return std::nullopt;
  }
  rapidjson::IStreamWrapper input_stream_wrapper(input_stream);
  rapidjson::Document document;
  const rapidjson::ParseResult result = document.ParseStream(input_stream_wrapper);
  if (!result) {
    logger.log(core::LogLevel::kFatal,
               "Error parsing JSON: %s",
               rapidjson::GetParseError_En(document.GetParseError()));
    return std::nullopt;
  }
  if (!document.HasMember("config_messages")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'config_messages' in can message file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  if (!document.HasMember("messages")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'messages' in can message file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  const auto configuration_messages = document["config_messages"].GetObject();
  std::vector<io::CanFrame> controller_configuration_messages;
  for (const auto &message : configuration_messages) {
    const auto new_message = Controller::parseJsonCanFrame(logger, message.value.GetObject());
    if (!new_message) {
      logger.log(core::LogLevel::kFatal,
                 "Invalid CAN configuration frame in JSON message file at path %s",
                 message_file_path.c_str());
      return std::nullopt;
    }
    controller_configuration_messages.push_back(*new_message);
  }
  std::unordered_map<std::string, io::CanFrame> controller_messages;
  const auto messages = document["messages"].GetObject();
  if (!messages.HasMember("enter_stop_state")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'enter_stop_state' in can message file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  if (!messages.HasMember("enter_preoperational_state")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'enter_preoperational_state' in can message file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  if (!messages.HasMember("enter_operational_state")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'enter_operational_state' in can message file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  if (!messages.HasMember("set_frequency")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'set_frequency' in can message "
               "file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  if (!messages.HasMember("shutdown")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'shutdown' in can message file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  if (!messages.HasMember("switch_on")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'switch_on' in can message file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  if (!messages.HasMember("start_drive")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'start_drive' in can message file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  if (!messages.HasMember("quick_stop")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'quick_stop' in can message file at %s",
               message_file_path.c_str());
    return std::nullopt;
  }
  for (const auto &message : messages) {
    const auto new_message = Controller::parseJsonCanFrame(logger, message.value.GetObject());
    if (!new_message) {
      logger.log(core::LogLevel::kFatal,
                 "Invalid CAN frame in JSON message file at path %s",
                 message_file_path.c_str());
      return std::nullopt;
    }
    controller_messages.emplace(message.name.GetString(), *new_message);
  }
  return std::make_shared<Controller>(
    logger, controller_messages, controller_configuration_messages, can, frequency_calculator);
}

Controller::Controller(core::ILogger &logger,
                       const std::unordered_map<std::string, io::CanFrame> &messages,
                       const std::vector<io::CanFrame> &configuration_messages,
                       const std::shared_ptr<io::ICan> can,
                       const std::shared_ptr<IFrequencyCalculator> frequency_calculator)
    : logger_(logger),
      configuration_messages_(configuration_messages),
      messages_(messages),
      can_(can),
      frequency_calculator_(frequency_calculator),
      controller_temperature_(0),
      controller_current_(0),
      controller_state_(ControllerState::kPreOperationalState)
{
}

std::optional<io::CanFrame> Controller::parseJsonCanFrame(
  core::ILogger &logger, rapidjson::GenericObject<true, rapidjson::Value> message)
{
  if (!message.HasMember("id")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'id' in message in CAN message file");
    return std::nullopt;
  }
  if (!message.HasMember("command")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'command' in message in CAN message file");
    return std::nullopt;
  }
  if (!message.HasMember("index")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'index' in message in CAN message file");
    return std::nullopt;
  }
  if (!message.HasMember("subindex")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'subindex' in message in CAN message file");
    return std::nullopt;
  }
  if (!message.HasMember("data")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'data' in message in CAN message file");
    return std::nullopt;
  }
  std::stringstream can_id_hex;
  can_id_hex << std::hex << message["id"].GetString();
  if (!can_id_hex.good()) {
    logger.log(core::LogLevel::kFatal, "Invalid message ID in CAN message file");
    return std::nullopt;
  }
  if (can_id_hex.eof()) {
    logger.log(core::LogLevel::kFatal, "No message ID in CAN message file");
    return std::nullopt;
  }
  io::CanFrame new_message;
  can_id_hex >> new_message.can_id;
  new_message.can_dlc = motors::kControllerCanFrameLength;
  std::stringstream command_hex;
  command_hex << std::hex << message["command"].GetString();
  if (!command_hex.good()) {
    logger.log(core::LogLevel::kFatal, "Invalid message command in CAN message file");
    return std::nullopt;
  }
  if (command_hex.eof()) {
    logger.log(core::LogLevel::kFatal, "No message command in CAN message file");
    return std::nullopt;
  }
  std::uint16_t command;
  command_hex >> command;
  new_message.data[0] = static_cast<std::uint8_t>(command);
  // convert index to little endian for controller
  std::stringstream index_hex;
  index_hex << std::hex << message["index"].GetString();
  if (!index_hex.good()) {
    logger.log(core::LogLevel::kFatal, "Invalid message index in CAN message file");
    return std::nullopt;
  }
  if (index_hex.eof()) {
    logger.log(core::LogLevel::kFatal, "No message index in CAN message file");
    return std::nullopt;
  }
  std::uint16_t index;
  index_hex >> index;
  new_message.data[1] = index & 0xFF;
  new_message.data[2] = (index & 0xFF00) >> 8;
  // subindex doesn't need converted
  std::stringstream subindex_hex;
  subindex_hex << std::hex << message["subindex"].GetString();
  if (!subindex_hex.good()) {
    logger.log(core::LogLevel::kFatal, "Invalid message subindex in CAN message file");
    return std::nullopt;
  }
  if (subindex_hex.eof()) {
    logger.log(core::LogLevel::kFatal, "No message subindex in CAN message file");
    return std::nullopt;
  }
  std::uint16_t sub_index;
  subindex_hex >> sub_index;
  new_message.data[3] = static_cast<std::uint8_t>(sub_index);
  // convert data to little endian
  std::stringstream data_hex;
  data_hex << std::hex << message["data"].GetString();
  if (!data_hex.good()) {
    logger.log(core::LogLevel::kFatal, "Invalid message data in CAN message file");
    return std::nullopt;
  }
  if (data_hex.eof()) {
    logger.log(core::LogLevel::kFatal, "No message data in CAN message file");
    return std::nullopt;
  }
  std::uint32_t data;
  data_hex >> data;
  new_message.data[4] = data & 0xFF;
  new_message.data[5] = (data & 0xFF00) >> 8;
  new_message.data[6] = (data & 0xFF0000) >> 16;
  new_message.data[7] = (data & 0xFF000000) >> 24;
  return new_message;
}

void Controller::processErrorMessage(const std::uint16_t error_code)
{
  switch (error_code) {
    case 0xFF01:
      logger_.log(core::LogLevel::kFatal,
                  "ERROR_CURRENT_A: Current phase A hall sensor missing or damaged");
      break;
    case 0xFF02:
      logger_.log(core::LogLevel::kFatal,
                  "ERROR_CURRENT_B: Current phase A hall sensor missing or damaged");
      break;
    case 0xFF03:
      logger_.log(core::LogLevel::kFatal, "ERROR_HS_FET: High side Fet short circuit");
      break;
    case 0xFF04:
      logger_.log(core::LogLevel::kFatal, "ERROR_LS_FET: Low side Fet short circuit");
      break;
    case 0xFF05:
      logger_.log(core::LogLevel::kFatal, "ERROR_DRV_LS_L1: Low side Fet phase 1 short circuit");
      break;
    case 0xFF06:
      logger_.log(core::LogLevel::kFatal, "ERROR_DRV_LS_L2: Low side Fet phase 2 short circuit");
      break;
    case 0xFF07:
      logger_.log(core::LogLevel::kFatal, "ERROR_DRV_LS_L3: Low side Fet phase 3 short circuit");
      break;
    case 0xFF08:
      logger_.log(core::LogLevel::kFatal, "ERROR_DRV_HS_L1: High side Fet phase 1 short circuit");
      break;
    case 0xFF09:
      logger_.log(core::LogLevel::kFatal, "ERROR_DRV_HS_L2: High side Fet phase 2 short circuit");
      break;
    case 0xFF0A:
      logger_.log(core::LogLevel::kFatal, "ERROR_DRV_HS_L3: High side Fet phase 3 short circuit ");
      break;
    case 0xFF0B:
      logger_.log(core::LogLevel::kFatal,
                  "ERROR_MOTOR_FEEDBACK: Wrong feedback selected (check feedback type)");
      break;
    case 0xFF0C:
      logger_.log(core::LogLevel::kFatal,
                  "ERROR_DC_LINK_UNDERVOLTAGE: DC voltage not applied to bridge or too low");
      break;
    case 0xFF0D:
      logger_.log(core::LogLevel::kFatal, "ERROR_PULS_MODE_FINISHED: Pulse mode finished");
      break;
    case 0xFF0E:
      logger_.log(core::LogLevel::kFatal, "ERROR_APP_ERROR");
      break;
    case 0xFF0F:
      logger_.log(core::LogLevel::kFatal, "ERROR_EMERGENCY_BUTTON: Emergency button pressed");
      break;
    case 0xFF10:
      logger_.log(core::LogLevel::kFatal,
                  "ERROR_CONTROLLER_OVERTEMPERATURE: Controller overtemperature");
      break;
    case 0x3210:
      logger_.log(core::LogLevel::kFatal,
                  "ERROR_DC_LINK_OVERVOLTAGE: Power supply voltage too high");
      break;
    default:
      logger_.log(core::LogLevel::kFatal,
                  "GENERIC_ERROR: Unspecific error occurred with code %i",
                  error_code);
      break;
  }
}

ControllerStatus Controller::processWarningMessage(const std::uint8_t warning_code)
{
  // If there is no error, return nominal.
  if (warning_code == 0) { return ControllerStatus::kNominal; }
  logger_.log(core::LogLevel::kInfo, "Controller Warning found, (code: %x)", warning_code);

  // In the event some warning(s) have occured, print each and return highest priority.
  if (warning_code & 0x1) {
    logger_.log(core::LogLevel::kInfo, "Controller Warning: Controller Temperature Exceeded");
  }
  if (warning_code & 0x2) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: Motor Temperature Exceeded");
  }
  if (warning_code & 0x4) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: DC link under voltage");
  }
  if (warning_code & 0x8) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: DC link over voltage");
  }
  if (warning_code & 0x10) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: DC link over current");
  }
  if (warning_code & 0x20) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: Stall protection active");
  }
  if (warning_code & 0x40) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: Max velocity exceeded");
  }
  if (warning_code & 0x80) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: BMS Proposed Power");
  }
  if (warning_code & 0x100) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: Capacitor temperature exceeded");
  }
  if (warning_code & 0x200) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: I2T protection");
  }
  if (warning_code & 0x400) {
    logger_.log(core::LogLevel::kFatal, "Controller Warning: Field weakening active");
  }
  return ControllerStatus::kUnrecoverableWarning;
}

// NMT stands for network management
core::Result Controller::processNmtMessage(const std::uint8_t nmt_code)
{
  switch (nmt_code) {
    // Operational State
    case 0x01:
      logger_.log(core::LogLevel::kDebug, "Controller enter operational state");
      controller_state_ = ControllerState::kOperationalState;
      return core::Result::kSuccess;
    // Stop State
    case 0x02:
      logger_.log(core::LogLevel::kDebug, "Controller enter stop state");
      controller_state_ = ControllerState::kStopState;
      return core::Result::kSuccess;
    // Pre-operational State
    case 0x03:
      logger_.log(core::LogLevel::kDebug, "Controller enter pre-operational state");
      controller_state_ = ControllerState::kPreOperationalState;
      return core::Result::kSuccess;
    // Reset node state
    case 0x81:
      logger_.log(core::LogLevel::kDebug, "Controller enter reset node state");
      controller_state_ = ControllerState::kResetNodeState;
      return core::Result::kSuccess;
    // Stop state
    case 0x82:
      logger_.log(core::LogLevel::kDebug, "Controller enter stop state");
      controller_state_ = ControllerState::kResetCommunicationState;
      return core::Result::kSuccess;
    default:
      logger_.log(core::LogLevel::kDebug, "Controller enter unknown state");
      controller_state_ = ControllerState::kUnknownState;
      return core::Result::kFailure;
  }
}

core::Result Controller::processSdoMessage(const std::uint16_t index,
                                           const std::uint8_t subindex,
                                           std::uint32_t data)
{
  // Handle error messages
  if (index == Controller::kSdoErrorIndex && subindex == 0x00) {
    processErrorMessage(data);
    return core::Result::kFailure;
  }
  // Handle warning messages
  if (index == Controller::kSdoWarningIndex && subindex == 0x00) {
    const motors::ControllerStatus result = processWarningMessage(data);
    if (result == motors::ControllerStatus::kUnrecoverableWarning) {
      return core::Result::kFailure;
    }
    return core::Result::kSuccess;
  }
  // Temperature
  if (index == Controller::kSdoTemperatureIndex && subindex == 0x00) {
    controller_temperature_ = static_cast<std::uint16_t>(data & 0xFFFF);
    return core::Result::kSuccess;
  }
  // Current
  if (index == Controller::kSdoCurrentIndex && subindex == 0x00) {
    controller_current_ = reinterpret_cast<std::int32_t &>(data);
    return core::Result::kSuccess;
  }
  logger_.log(core::LogLevel::kFatal,
              "Unknown CAN SDO message received. Index: %d, Subindex: %d, Data: %d",
              index,
              subindex,
              data);
  return core::Result::kFailure;
}

std::uint8_t Controller::getControllerTemperature() const
{
  return controller_temperature_;
}

core::Float Controller::getControllerCurrent() const
{
  return controller_current_;
}

core::Result Controller::run(FauxState state)
{
  // TODOLater this should be using state machine's states, not faux
  switch (state) {
    case FauxState::kInitial:
      return core::Result::kSuccess;
    case FauxState::kConfigure:
      return configure();
    case FauxState::kReady:
      return core::Result::kSuccess;
    case FauxState::kAccelerate:
      return accelerate();
    case FauxState::kStop:
      return stop();
    case FauxState::kReset:
      return reset();
    default:
      logger_.log(core::LogLevel::kFatal, "Invalid state %d", static_cast<int>(state));
      return core::Result::kFailure;
  }
}

core::Result Controller::configure()
{
  for (io::CanFrame message : configuration_messages_) {
    core::Result result = can_->send(message);
    if (result != core::Result::kSuccess) {
      logger_.log(core::LogLevel::kFatal, "Failed to send configuration message");
      return result;
    }
  }
  return core::Result::kSuccess;
}

core::Result Controller::accelerate()
{
  std::uint16_t new_frequency      = frequency_calculator_->calculateFrequency(velocity_);
  const auto set_frequency_message = messages_.find("set_frequency");
  if (set_frequency_message == messages_.end()) {
    logger_.log(core::LogLevel::kFatal, "Failed to find 'set_frequency' message");
    return core::Result::kFailure;
  }
  io::CanFrame message = set_frequency_message->second;
  message.data[4]      = new_frequency & 0xFF;
  message.data[5]      = (new_frequency >> 8) & 0xFF;
  message.data[6]      = (new_frequency >> 16) & 0xFF;
  message.data[7]      = (new_frequency >> 24) & 0xFF;
  core::Result result  = can_->send(message);
  if (result != core::Result::kSuccess) {
    logger_.log(core::LogLevel::kFatal, "Failed to send 'set_frequency' message");
    return result;
  }
  const auto start_drive_message = messages_.find("start_drive");
  if (start_drive_message == messages_.end()) {
    logger_.log(core::LogLevel::kFatal, "Failed to find 'start_drive' message");
    return core::Result::kFailure;
  }
  result = can_->send(start_drive_message->second);
  if (result != core::Result::kSuccess) {
    logger_.log(core::LogLevel::kFatal, "Failed to send 'start_drive' message");
    return result;
  }
  return core::Result::kSuccess;
}

core::Result Controller::stop()
{
  const auto shutdown_message = messages_.find("shutdown");
  if (shutdown_message == messages_.end()) {
    logger_.log(core::LogLevel::kFatal, "Failed to find 'shutdown' message");
    return core::Result::kFailure;
  }
  core::Result result = can_->send(shutdown_message->second);
  if (result != core::Result::kSuccess) {
    logger_.log(core::LogLevel::kFatal, "Failed to send 'shutdown' message");
    return result;
  }
  return core::Result::kSuccess;
}

core::Result Controller::reset()
{
  velocity_ = 0;
  {
    const auto enter_stop_state_message = messages_.find("enter_stop_state");
    if (enter_stop_state_message == messages_.end()) {
      logger_.log(core::LogLevel::kFatal, "Failed to find 'enter_stop_state' message");
      return core::Result::kFailure;
    }
    core::Result result = can_->send(enter_stop_state_message->second);
    if (result != core::Result::kSuccess) {
      logger_.log(core::LogLevel::kFatal, "Failed to send 'enter_stop_state' message");
      return result;
    }
  }
  {
    const auto enter_preoperational_state_message = messages_.find("enter_preoperational_state");
    if (enter_preoperational_state_message == messages_.end()) {
      logger_.log(core::LogLevel::kFatal, "Failed to find 'enter_preoperational_state' message");
      return core::Result::kFailure;
    }
    core::Result result = can_->send(enter_preoperational_state_message->second);
    if (result != core::Result::kSuccess) {
      logger_.log(core::LogLevel::kFatal, "Failed to send 'enter_preoperational_state' message");
      return result;
    }
  }
  {
    const auto enter_operational_state_message = messages_.find("enter_operational_state");
    if (enter_operational_state_message == messages_.end()) {
      logger_.log(core::LogLevel::kFatal, "Failed to find 'enter_operational_state' message");
      return core::Result::kFailure;
    }
    core::Result result = can_->send(enter_operational_state_message->second);
    if (result != core::Result::kSuccess) {
      logger_.log(core::LogLevel::kFatal, "Failed to send 'enter_operational_state' message");
      return result;
    }
  }
  {
    const auto shutdown_message = messages_.find("shutdown");
    if (shutdown_message == messages_.end()) {
      logger_.log(core::LogLevel::kFatal, "Failed to find 'shutdown' message");
      return core::Result::kFailure;
    }
    core::Result result = can_->send(shutdown_message->second);
    if (result != core::Result::kSuccess) {
      logger_.log(core::LogLevel::kFatal, "Failed to send 'shutdown' message");
      return result;
    }
  }
  {
    const auto switch_on_message = messages_.find("switch_on");
    if (switch_on_message == messages_.end()) {
      logger_.log(core::LogLevel::kFatal, "Failed to find 'switch_on' message");
      return core::Result::kFailure;
    }
    core::Result result = can_->send(switch_on_message->second);
    if (result != core::Result::kSuccess) {
      logger_.log(core::LogLevel::kFatal, "Failed to send 'switch_on' message");
      return result;
    }
  }
  return core::Result::kSuccess;
}

}  // namespace hyped::motors