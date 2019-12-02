# Program Description
The STM32 project folder is used to debug and program the STM32F4 Discovery board. 

The program uses Serial data from the python application to get information about objects and QR Codes seen on the Webcam feed. The data collection is done by using the DMA peripheral to transfer data collected on the USART2 peripheral to memory to minimize CPU usage and control the Robot Arm.

## Limitations and Possible Solutions
* The OWI-535 Robot Arm could not be controlled with continuous movement due to the QR Code capture. When rotating with continuous motion, the QR Codes could not be detected until motion stopped. As a result, the Robot Arm  is moved in short bursts to allow for QR Code detection.  


  This issue could possibly be resolved with the inclusion of Motor 2 (see the [Pin Connections](../README.md#Pin-Connections) section) to allow the Robot Head to move with less curvature.  
* Including another form of feedback for the motor position would allow for a more versatile control of the Robot Arm. With this it would be possible to to control the depth at which the object is seen, and allow us to pick up objects from any position. As can be seen in the youtube video link in the [Project Description](../README.md#Project-Description), the object position needed to be set at a specific position for the Arm to pick up.

  Some forms of feedback would be using potentiometers to detect the movement of the joints. Attaching potentiometers requires using an external gear system. To minimize variance of gear rotation and movement, the gear system needs to have as little slack as possible.

  Another form of feedback would be to install optical encoders inside the gearbox of the DC motors. With this, it would be possible to count the gear rotations inside the motor, and calculate the distance traveled based on the gear revolutions and gear ratios. Unlike potentiometers, using optical encoders would require an initial calibration phase to establish the initial position of the Robot Arm.