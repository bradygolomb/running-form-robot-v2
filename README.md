# Running Form Robot v2

A ROS 2 (C++) and Gazebo rebuild of my **Running Form Robot**: an autonomous camera rover I built in 2024 that followed a runner outdoors and reported stride rate in real time using computer-vision motion tracking.

The original hardware has since been retired. This project revisits the system in software, so the full pipeline can be developed, tested, and shown in simulation.

> **Status:** In progress. Week 1 of the build is complete.

## What works now

- **ROS 2 Jazzy workspace** on Ubuntu 24.04 (WSL2)
- **`gait_publisher` node (C++):** publishes a simulated knee angle on `/knee_angle` at 50 Hz. The angle swings between 5° and 65° at 1.5 strides per second.
- Output checked live in `rqt_plot`: the range and the stride frequency match the model.

## Roadmap

- [x] ROS 2 + Gazebo environment setup
- [x] First C++ node: simulated knee-angle publisher
- [ ] Leg model (URDF) running in Gazebo
- [ ] PID joint control with `ros2_control`
- [ ] Computer-vision pose tracking (OpenCV) to measure stride rate and joint angles from running video
- [ ] Form feedback: compare measured gait to target ranges
- [ ] Demo video and write-up on [my portfolio](https://bradygolomb.github.io/)

## Run it

Requires ROS 2 Jazzy on Ubuntu 24.04.

```bash
cd ~/ros2_ws
colcon build
source install/setup.bash
ros2 run rfr_v2 gait_publisher
```

In a second terminal, plot the output:

```bash
ros2 run rqt_plot rqt_plot /knee_angle/data
```

## Repository layout

```
src/rfr_v2/
├── src/gait_publisher.cpp   # simulated knee-angle publisher
├── CMakeLists.txt
└── package.xml
```

## Author

**Brady Golomb**, Mechanical Engineering (BSE) and Robotics (MSE), University of Pennsylvania
[Portfolio](https://bradygolomb.github.io/)