# AutoNet - Drone Swarm Planning from Topology  (WIP)

![Status](https://img.shields.io/badge/Status-Work_in_Progress-yellow)
![ROS2](https://img.shields.io/badge/ROS_2-Kilted%2FJazzy-brightgreen)
![C++](https://img.shields.io/badge/Language-C%2B%2B17-blue)

**AutoNet** is an advanced research project focused on **Swarm Robotics**, distributed sensing, and cooperative navigation.

The goal of the project is to develop a fleet of autonomous agents capable of:
*   **Mutual Localization & Communication:** Utilizing **Ultra-Wideband (UWB)** technology for precise relative positioning and inter-robot data exchange.
*   **Topology-Aware Planning:** Navigating complex environments based on a predefined dynamic topology.
*   **Environmental Sensing:** Mapping and analyzing the surroundings using LIDAR technology.

## 🚀 Current Features
*   **RPLIDAR A1 Integration:** Full support for Slamtec RPLIDAR A1M8 via ROS 2.
*   **Data Recording:** Custom C++ node (`scan_to_csv`) that captures `LaserScan` messages and saves them as CSV files in real-time.
*   **Automated Launch:** Single launch file to start the driver and the recorder simultaneously with correct parameters.

## 🤖 The Agent (Hardware)
Each unit in the swarm is built (for now) upon the **Elegoo Conqueror Robot Tank** chassis (or a compatible mobile base), upgraded to handle onboard computation and high-fidelity sensing:
*   **Master Node:** Raspberry Pi 4B (Running ROS 2).
*   **Low-Level Controller:** Arduino Uno (Motor & Actuator control).
*   **Perception:** Slamtec RPLIDAR A1M8 (360° Laser Scan).
*   **Communication:** UWB Modules for mesh networking and ranging.

## 📋 Prerequisites
Before running this project, ensure you have:
1.  **ROS 2 Installed** (Humble or Jazzy recommended). [Installation Guide](https://docs.ros.org/en/humble/Installation.html)
2.  **Build Tools:** `colcon` and `cmake`.

## 📦 Installation & Setup

### 1. Create a Workspace
If you don't have a ROS 2 workspace yet, create one:
```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
```

### 2. Clone Repositories
You need to clone this repository and the official RPLIDAR driver (specifically the `ros2` branch).

```bash
# Clone this project
git clone https://github.com/YOUR_USERNAME/cpp_lidar_tools.git

# Clone RPLIDAR ROS 2 Driver
git clone -b ros2 https://github.com/Slamtec/rplidar_ros.git
```

### 3. Install Dependencies
Run `rosdep` to install any missing system dependencies:
```bash
cd ~/ros2_ws
rosdep update
rosdep install --from-paths src --ignore-src -r -y
```

### 4. Build the Workspace
Compile the packages using `colcon`.
```bash
cd ~/ros2_ws
colcon build --symlink-install
```

### 5. USB Permissions
To access the LIDAR without using `sudo` every time, add your user to the `dialout` group and **reboot**:
```bash
sudo usermod -a -G dialout $USER
sudo reboot
```

## ▶️ Usage

### Option A: Using the Launch File (Recommended)
This command starts both the LIDAR driver and the data recorder. It automatically configures the serial port `/dev/ttyUSB0` and baudrate `115200`.

```bash
source ~/ros2_ws/install/setup.bash
ros2 launch cpp_lidar_tools record_scan.launch.py
```
**Output:** CSV files will be automatically generated in `~/lidar_data/`.

### Option B: Running Nodes Separately
If you want to debug specific components, you can run the nodes individually in separate terminals.

**Terminal 1 (Lidar Driver):**
```bash
source ~/ros2_ws/install/setup.bash
ros2 run rplidar_ros rplidar_node --ros-args -p serial_port:=/dev/ttyUSB0 -p serial_baudrate:=115200 -p frame_id:=laser
```

**Terminal 2 (CSV Recorder):**
```bash
source ~/ros2_ws/install/setup.bash
ros2 run cpp_lidar_tools scan_to_csv --ros-args -p output_directory:="/home/$USER/lidar_data"
```

## 📂 Data Output Format
The generated CSV files follow this structure:
```csv
Sec,NanoSec,Angle_Min,Angle_Increment,Ranges...
16789012,500000,0.0,0.017,0.5_0.52_inf_0.6...
```
*   **Sec/NanoSec:** Timestamp of the scan.
*   **Angle_Min:** Start angle of the scan (radians).
*   **Angle_Increment:** Step between each measurement point.
*   **Ranges:** Array of distance measurements (meters). `inf` indicates no return (out of range).

## 🔜 Roadmap
*   [x] Lidar Integration & Data Recording.
*   [ ] Serial Communication Node (RPi <-> Arduino).
*   [ ] Motor Control Implementation.
*   [ ] Navigation Stack configuration.

## 📄 License
[](LICENSE)
