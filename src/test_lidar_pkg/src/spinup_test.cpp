//
// Created by sharpeii on 13/03/26.
//
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_srvs/srv/empty.hpp"

class LidarSpinupTest : public rclcpp::Node {

public:
    LidarSpinupTest() : Node("lidar_spinup_test"), state_(0), first_scan_received_(false)
    {

    }

private:
    int state_;
    bool first_scan_received_;
    rclcpp::Time start_time_;
    rclcpp::Time wait_time_;

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;

}