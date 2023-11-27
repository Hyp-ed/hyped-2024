#include <gtest/gtest.h>

#include <motors/pid_controller.hpp>

namespace hyped::motors {

TEST(motors, pid)
{
  PidController pidcontroller(2, 2, 2, 2, -19802, 5, -19404, 5, 1);

  std::array<core::Float, 100> measurement_list
    = {2,   4,   6,   8,   10,  12,  14,  16,  18,  20,  22,  24,  26,  28,  30,  32,  34,
       36,  38,  40,  42,  44,  46,  48,  50,  52,  54,  56,  58,  60,  62,  64,  66,  68,
       70,  72,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  96,  98,  100, 102,
       104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130, 132, 134, 136,
       138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 170,
       172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200};

  std::array<core::Float, 100> expected_output
    = {1.40000000000,      -1.64000000000,     -10.21600000000,    -21.87040000000,
       -38.07776000000,    -57.95334400000,    -82.02799360000,    -109.98320384000,
       -142.01007769600,   -177.99395338240,   -218.00362797056,   -261.99782321766,
       -310.00130606940,   -361.99921635836,   -418.00047018499,   -477.99971788901,
       -542.00016926659,   -609.99989844004,   -682.00006093597,   -757.99996343842,
       -838.00002193695,   -921.99998683783,   -1010.00000789730,  -1101.99999526162,
       -1198.00000284303,  -1297.99999829418,  -1402.00000102349,  -1509.99999938591,
       -1622.00000036846,  -1737.99999977893,  -1858.00000013264,  -1981.99999992041,
       -2110.00000004775,  -2241.99999997135,  -2378.00000001719,  -2517.99999998969,
       -2662.00000000619,  -2809.99999999629,  -2962.00000000223,  -3117.99999999866,
       -3278.00000000080,  -3441.99999999952,  -3610.00000000029,  -3781.99999999983,
       -3958.00000000010,  -4137.99999999994,  -4322.00000000004,  -4509.99999999998,
       -4702.00000000001,  -4897.99999999999,  -5098.00000000000,  -5302.00000000000,
       -5510.00000000000,  -5722.00000000000,  -5938.00000000000,  -6158.00000000000,
       -6382.00000000000,  -6610.00000000000,  -6842.00000000000,  -7078.00000000000,
       -7318.00000000000,  -7562.00000000000,  -7810.00000000000,  -8062.00000000000,
       -8318.00000000000,  -8578.00000000000,  -8842.00000000000,  -9110.00000000000,
       -9382.00000000000,  -9658.00000000000,  -9938.00000000000,  -10222.00000000000,
       -10510.00000000000, -10802.00000000000, -11098.00000000000, -11398.00000000000,
       -11702.00000000000, -12010.00000000000, -12322.00000000000, -12638.00000000000,
       -12958.00000000000, -13282.00000000000, -13610.00000000000, -13942.00000000000,
       -14278.00000000000, -14618.00000000000, -14962.00000000000, -15310.00000000000,
       -15662.00000000000, -16018.00000000000, -16378.00000000000, -16742.00000000000,
       -17110.00000000000, -17482.00000000000, -17858.00000000000, -18238.00000000000,
       -18622.00000000000, -19010.00000000000, -19402.00000000000, -19798.00000000000};
  for (std::size_t i = 0; i < 100; i++) {
    core::Float output = pidcontroller.update(3, measurement_list[i]);
    std::cout << "Output is: " << output << "   Expected output is: " << expected_output[i] << "\n";
    ASSERT_TRUE(output == expected_output[i]);
  }
}
}  // namespace hyped::motors
