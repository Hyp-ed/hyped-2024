#include "hardware_spi.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>

namespace hyped::io {

std::optional<std::shared_ptr<HardwareSpi>> HardwareSpi::create(core::ILogger &logger,
                                                                const SpiBus bus,
                                                                const SpiMode mode,
                                                                const SpiWordSize word_size,
                                                                const SpiBitOrder bit_order,
                                                                const SpiClock clock)
{
  // SPI bus only works in kernel mode on Linux, so we need to call the provided driver
  const char *spi_bus_address = getSpiBusAddress(bus);
  const int file_descriptor   = open(spi_bus_address, O_RDWR, 0);
  if (file_descriptor < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to open SPI device");
    return std::nullopt;
  }
  // Set clock frequency
  const std::uint32_t clock_value = getClockValue(clock);
  const int clock_write_result    = ioctl(file_descriptor, SPI_IOC_WR_MAX_SPEED_HZ, &clock_value);
  if (clock_write_result < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to set clock frequency to %d", clock_value);
    return std::nullopt;
  }
  // Set word size
  const std::uint8_t bits_per_word = static_cast<std::uint8_t>(word_size);
  const int word_size_write_result
    = ioctl(file_descriptor, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
  if (word_size_write_result < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to set bits per word");
    return std::nullopt;
  }
  // Set SPI mode
  const std::uint8_t selected_mode = static_cast<std::uint8_t>(mode);
  const int mode_write_result      = ioctl(file_descriptor, SPI_IOC_WR_MODE, &selected_mode);
  if (mode_write_result < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to set SPI mode");
    return std::nullopt;
  }
  // Set bit order
  const std::uint8_t order     = static_cast<std::uint8_t>(bit_order);
  const int order_write_result = ioctl(file_descriptor, SPI_IOC_WR_LSB_FIRST, &order);
  if (order_write_result < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to set bit order");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully initialised SPI");
  return std::make_shared<HardwareSpi>(logger, file_descriptor);
}

HardwareSpi::HardwareSpi(core::ILogger &logger, const int file_descriptor)
    : logger_(logger),
      file_descriptor_(file_descriptor)
{
}

HardwareSpi::~HardwareSpi()
{
  close(file_descriptor_);
}

core::Result HardwareSpi::read(const std::uint8_t register_address,
                               const std::uint8_t *rx,
                               const std::uint16_t len)
{
  spi_ioc_transfer message[2] = {};
  // send address
  message[0].tx_buf = reinterpret_cast<std::uint64_t>(&register_address);
  message[0].rx_buf = 0;
  message[0].len    = 1;
  // receive data
  message[1].tx_buf     = 0;
  message[1].rx_buf     = reinterpret_cast<std::uint64_t>(rx);
  message[1].len        = len;
  const int read_result = ioctl(file_descriptor_, SPI_IOC_MESSAGE(2), message);
  if (read_result < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to read from SPI device");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully read from SPI device");
  return core::Result::kSuccess;
}

core::Result HardwareSpi::write(const std::uint8_t register_address,
                                const std::uint8_t *tx,
                                const std::uint16_t len)
{
  spi_ioc_transfer message[2] = {};
  // send address
  message[0].tx_buf = reinterpret_cast<std::uint64_t>(&register_address);
  message[0].rx_buf = 0;
  message[0].len    = 1;
  // write data
  message[1].tx_buf      = reinterpret_cast<std::uint64_t>(tx);
  message[1].rx_buf      = 0;
  message[1].len         = len;
  const int write_result = ioctl(file_descriptor_, SPI_IOC_MESSAGE(2), message);
  if (write_result < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to write to SPI device");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully wrote to SPI device");
  return core::Result::kSuccess;
}

const char *HardwareSpi::getSpiBusAddress(const SpiBus bus)
{
  if (bus == SpiBus::kSpi0) {
    return "/dev/spidev0.0";
  } else {
    return "/dev/spidev1.0";
  }
}

std::uint32_t HardwareSpi::getClockValue(SpiClock clock)
{
  switch (clock) {
    case SpiClock::k500KHz:
      return 500'000;
    case SpiClock::k1MHz:
      return 1'000'000;
    case SpiClock::k4MHz:
      return 4'000'000;
    case SpiClock::k16MHz:
      return 16'000'000;
    case SpiClock::k20MHz:
      return 20'000'000;
    default:  // for compiler
      return 0;
  }
}
}  // namespace hyped::io