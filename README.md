# Ryan-The-Maze_Runner: A-maze-ing Race Robot

## Project Overview
Ryan-The-Maze_Runner is an mBot developed by Team B03-S3-T1 for the CG1111A module at NUS, aiming to successfully navigate a maze, avoid obstacles, and perform various functions such as color detection, lane correction, and more. This project showcases the integration of sensors, embedded systems, and intelligent decision-making to navigate complex environments.

## Features and Functionality
- Maze Navigation: Ryan navigates a maze using ultrasonic and IR sensors for lane correction.
- Color Identification: Detects different colors in the maze using RGB LEDs and a Light Dependent Resistor (LDR).
- Obstacle Detection and Lane Correction: Uses ultrasonic and IR sensors to keep in the center of the lane and correct its trajectory.
- Celebratory Music: Plays a tune ("我的歌声里" by Qu Wanting) upon successful completion of the maze using MeBuzzer.

## System Architecture

### Hardware Components:
- Ultrasonic Sensor: Measures distance to the left wall for lane correction.
- IR Sensor: Mounted on the right side to detect right wall proximity.
- MeLine Follower: Detects black stoplines.
- Color Sensor: Built using RGB LEDs and LDR to identify colors.
- Motors and MeBuzzer: Controls movement and plays celebratory music.
- Chassis and Skirting: Used to block ambient light interference and provide structural stability.

### Key Sensors and Subsystems:
- Lane Correction: Uses ultrasonic and IR sensors to maintain proper alignment in the maze.
- Color Identification: Utilizes LDR to measure reflected light from colored surfaces, enabling differentiation among various colors.
- Stopline Detection: Uses the MeLine Follower to detect the maze stopline for sequence initiation.

## General Algorithm
Ryan follows three main sequences to navigate the maze:

- Advance and Correct Lane Position: Uses ultrasonic and IR sensors to keep the robot centered between the walls.
- Detect Stoplines: Stops at black stoplines and enters the "Blast, Read & React Sequence."
- Color Identification: Identifies specific colors and reacts accordingly to guide navigation.

## Challenges and Solutions
- Ambient Light Issues: Ambient light caused inconsistencies in color detection. Black skirting and a chimney were added to block unwanted light.
- Jittery Movements: Ryan's movements were jittery due to overly frequent corrections. The margin of error for lane correction was adjusted, and delay timings were fine-tuned to smooth its trajectory.
- Sensor Reliability: The IR sensor was inconsistent, so it was paired with the ultrasonic sensor to provide more reliable lane correction.

## Lessons Learned
- Calibration Is Key: Frequent recalibrations were required to ensure accuracy, emphasizing the importance of optimizing sensor calibration and environmental setup.
- Team Troubleshooting: Several components failed during the project, but systematic troubleshooting helped identify and solve these issues effectively.
- Fine-Tuning Parameters: Adjusting correction thresholds, loop timings, and other parameters was crucial to achieving smooth navigation through the maze.
