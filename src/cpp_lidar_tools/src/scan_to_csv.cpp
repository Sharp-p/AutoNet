//
// Created by pibuntu on 15/02/26.
//
#include <chrono>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

using std::placeholders::_1;
namespace fs = std::filesystem;

class LidarCsvRecorder : public rclcpp::Node
{
public:
    LidarCsvRecorder() : Node("lidar_csv_recorder")
    {
        // creating file name
        std::string p = __FILE__;
        std::string dir;
        const size_t last_slash_idx = p.rfind(fs::path::preferred_separator);
        if (std::string::npos != last_slash_idx)
        {
            dir = p.substr(0, last_slash_idx);
        }
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << "lidar_scan" << std::put_time(std::localtime(&now_time), "%Y%m%d_%H%M%S") << ".csv";
        filename_ = dir + fs::path::preferred_separator + ".."
            + fs::path::preferred_separator + "data" + fs::path::preferred_separator + ss.str();

        RCLCPP_INFO(this->get_logger(), ".csv file path: %s", filename_.c_str());

        csv_file_.open(filename_);
        if (!csv_file_.is_open())
        {
            RCLCPP_ERROR(this->get_logger(), "Not able to open file: %s", filename_.c_str());
            return;
        }

        // writing header
        csv_file_ << "Sec,NanoSec,Angle_Min,Angle_Increment,Ranges...\n";

        RCLCPP_INFO(this->get_logger(), "Recording started on: %s", filename_.c_str());

        // creating subscription to scan topic
        subscription_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", 10, std::bind(&LidarCsvRecorder::scan_callback, this, _1));
    }

    ~LidarCsvRecorder()
    {
        if (csv_file_.is_open())
        {
            csv_file_.close();
            RCLCPP_INFO(this->get_logger(), "File closed.");
        }
    }

private:
    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        // extracting timestamp
        long int sec = msg->header.stamp.sec;
        long int nanosec = msg->header.stamp.nanosec;

        // writing database
        csv_file_ << sec << "," << nanosec << ","
            << msg->angle_min << "," << msg->angle_increment;

        for (float range : msg->ranges)
        {
            csv_file_ << "," << range;
        }

        csv_file_ << "\n";

        csv_file_.flush();
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscription_;
    std::ofstream csv_file_;
    std::string filename_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LidarCsvRecorder>());
    rclcpp::shutdown();
    return 0;
}