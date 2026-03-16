//
// Created by sharpeii on 13/03/26.
//
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_srvs/srv/empty.hpp"
#include <chrono>

class LidarSpinupTest : public rclcpp::Node {

public:
    LidarSpinupTest() : Node("lidar_spinup_test"), state_(0), first_scan_received_(false)
    {
        scan_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan",
        10,
        std::bind(&LidarSpinupTest::scan_callback, this, std::placeholders::_1));
        // TODO 2: Crea i due client per i servizi "/start_motor" e "/stop_motor"
        start_client_ = this->create_client<std_srvs::srv::Empty>("start_motor");
        stop_client_ = this->create_client<std_srvs::srv::Empty>("stop_motor");
        // TODO 3: Crea un wall_timer che chiami la funzione execute_test() ogni 100ms
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100),
            std::bind(&LidarSpinupTest::execute_test, this));

        RCLCPP_INFO(this->get_logger(), "Waiting for the service of the LiDAR motor (start LiDAR node");
        while (!start_client_->wait_for_service(std::chrono::seconds(1)))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_ERROR(this->get_logger(), "Stopped by terminal.");
                return;
            }
        }
        RCLCPP_INFO(this->get_logger(), "Service found and available.");
    }

private:
    int state_;
    bool first_scan_received_;
    rclcpp::Time start_time_;
    rclcpp::Time wait_time_;

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;

    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr start_client_;
    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr stop_client_;

    rclcpp::TimerBase::SharedPtr timer_;

    void execute_test()
    {
        switch (state_)
        {
            case 0:
                {
                    auto request = std::make_shared<std_srvs::srv::Empty::Request>();
                    stop_client_->async_send_request(request);
                    wait_time_ = this->now();
                    first_scan_received_ = false;
                    state_ = 1;
                    break;
                }
            case 1:
                {
                    if ((this->now() - wait_time_).seconds() > 6)
                    {
                        auto request = std::make_shared<std_srvs::srv::Empty::Request>();
                        start_client_->async_send_request(request);
                        start_time_ = this->now();
                        state_ = 2;
                    }
                    break;
                }
                // do nothing, state will be updated by the subscription callback
        }
    }

    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr scan_msg)
    {
        // to remove "unused parameter" warning
        (void)scan_msg;

        if (state_ == 2 && !first_scan_received_)
        {
            rclcpp::Duration duration = this->now() - start_time_;
            RCLCPP_INFO(this->get_logger(), "Spin-Up time: %f", duration.seconds());
            first_scan_received_ = true;
            state_ = 0;
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LidarSpinupTest>());
    rclcpp::shutdown();
    return 0;
}