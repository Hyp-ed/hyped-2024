
#include <random>

namespace hyped::navigation {

// TODOLater: Generate something actually resembling pod data, not random numbers

class Generator {
  std::default_random_engine generator;
  std::normal_distribution<double> distribution;
  double min;
  double max;

 public:
  Generator(double min, double max) : min(min), max(max)
  {
    std::random_device rd;
    generator    = std::default_random_engine(rd());
    distribution = std::normal_distribution<double>(0, 1);
  }
};

}  // namespace hyped::navigation