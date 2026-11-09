"""Show the leg in RViz, driven by gait_publisher.

Run with:  ros2 launch rfr_v2 display.launch.py
"""
import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg = get_package_share_directory('rfr_v2')

    with open(os.path.join(pkg, 'urdf', 'leg.urdf')) as f:
        robot_description = f.read()

    return LaunchDescription([
        # Turns /joint_states + the URDF into the leg's 3D pose
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': robot_description}],
        ),
        # Your C++ node: produces the hip and knee angles
        Node(
            package='rfr_v2',
            executable='gait_publisher',
            output='screen',
        ),
        # 3D viewer, opened with the saved side view
        Node(
            package='rviz2',
            executable='rviz2',
            arguments=['-d', os.path.join(pkg, 'rviz', 'leg.rviz')],
        ),
    ])