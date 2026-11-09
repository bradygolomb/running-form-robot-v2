// gait_publisher: drives a simulated 2-joint leg (hip + knee) through a running stride.
//
// Publishes:
//   /joint_states  (sensor_msgs/JointState, radians)  -> moves the leg model in RViz
//   /knee_angle    (std_msgs/Float64, degrees)        -> same signal as Week 1, for rqt_plot
//
// Gait model (simple approximation, 1.5 strides per second):
//   hip  = 10 + 20 * sin(phase)   ->  -10 deg (extension) to 30 deg (flexion)
//   knee = 35 + 30 * cos(phase)   ->    5 deg to 65 deg
// The knee reaches peak bend a quarter-stride before the hip swings fully forward,
// which is roughly what happens during the swing phase of running.

#include <chrono>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;

namespace {
constexpr double kPi = 3.14159265358979323846;
constexpr double kStrideHz = 1.5;

constexpr double kHipMidDeg = 10.0;
constexpr double kHipAmpDeg = 20.0;
constexpr double kKneeMidDeg = 35.0;
constexpr double kKneeAmpDeg = 30.0;

double deg2rad(double deg) { return deg * kPi / 180.0; }
}  // namespace

class GaitPublisher : public rclcpp::Node
{
public:
  GaitPublisher()
  : Node("gait_publisher")
  {
    joint_pub_ = create_publisher<sensor_msgs::msg::JointState>("joint_states", 10);
    knee_pub_ = create_publisher<std_msgs::msg::Float64>("knee_angle", 10);
    start_time_ = now();
    timer_ = create_wall_timer(20ms, std::bind(&GaitPublisher::tick, this));  // 50 Hz
    RCLCPP_INFO(get_logger(), "Gait publisher started: %.1f strides/s", kStrideHz);
  }

private:
  void tick()
  {
    const double t = (now() - start_time_).seconds();
    const double phase = 2.0 * kPi * kStrideHz * t;

    const double hip_deg = kHipMidDeg + kHipAmpDeg * std::sin(phase);
    const double knee_deg = kKneeMidDeg + kKneeAmpDeg * std::cos(phase);

    sensor_msgs::msg::JointState js;
    js.header.stamp = now();
    js.name = {"hip_joint", "knee_joint"};
    js.position = {deg2rad(hip_deg), deg2rad(knee_deg)};
    joint_pub_->publish(js);

    std_msgs::msg::Float64 knee_msg;
    knee_msg.data = knee_deg;
    knee_pub_->publish(knee_msg);
  }

  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr knee_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Time start_time_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GaitPublisher>());
  rclcpp::shutdown();
  return 0;
}