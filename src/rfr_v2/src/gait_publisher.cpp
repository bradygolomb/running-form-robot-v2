#include <chrono>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;

class GaitPublisher : public rclcpp::Node {
public:
  GaitPublisher() : Node("gait_publisher"), t_(0.0) {
    pub_ = create_publisher<std_msgs::msg::Float64>("knee_angle", 10);
    // Runs every 20 ms (50 times per second)
    timer_ = create_wall_timer(20ms, [this]() {
      std_msgs::msg::Float64 msg;
      // Knee swings between 5 and 65 degrees, 1.5 strides per second
      msg.data = 35.0 + 30.0 * std::sin(2.0 * M_PI * 1.5 * t_);
      pub_->publish(msg);
      t_ += 0.02;
    });
  }

private:
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  double t_;
};

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GaitPublisher>());
  rclcpp::shutdown();
  return 0;
}