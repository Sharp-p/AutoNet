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
        fs::path home_path = std::getenv("HOME");
        fs::path default_path = home_path / "lidar_data";

        this->declare_parameter("output_directory", default_path.string());
        // creating file name
        std::string save_dir_str = this->get_parameter("output_directory").as_string();
        fs::path base_path(save_dir_str);

        // trying to create save directory (if it does not exists)
        try
        {
            if (!fs::exists(base_path))
            {
                fs::create_directories(base_path);
                RCLCPP_INFO(this->get_logger(), "Created directory: %s", base_path.c_str());
            }
        } catch (const fs::filesystem_error& e)
        {
            RCLCPP_ERROR(this->get_logger(),
                "Critical error during the creation of directory: %s", e.what());
            return;
        }

        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << "lidar_scan" << std::put_time(std::localtime(&now_time), "%Y%m%d_%H%M%S") << ".csv";
        fs::path full_path = base_path / ss.str();

        filename_ = full_path.string();

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