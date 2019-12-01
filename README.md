# **ECE 5330 Final Project: Lego Color Sorting via Camera Feedback**

Clone the repo to a local folder in your computer.
**Note:** This application uses Python 3.5 and up. Verify that the python installation location is in PATH.

# **Table of Contents**

- [Project Description](#Project-Description)
- [Setting up the Software](#Setting-up-the-project)
  - [Downloading Dependencies](#Downloading-Dependencies)
  - [Running the Python Application](#Running-the-Python-Application)
- [STM32 Microcontroller](#STM32-Microcontroller)
  - [Electrical Components](#Electrical-Components)
  - [Serial Communication](#Serial-Communication)
  - [Motor Connections](#Motor-Connections)

The project is composed of two sections:

1. The python application: Camera_Data_Detection.py.
2. The STM32F411VE microcontroller program located in the STM folder.

**Note:** The python application may be run independently without the microcontroller.

**WARNING:** The microcontroller program will only work correctly when the motor, L298N, and USB Serial connections are set correctly.
The python application must be running for the microcontroller to sucessfully maneuver the OWI arm.

The python application uses the opencv in conjuction with a STM32 microcontroller.
Camera data is sent to the STM32 microcontroller via Universal Asynchronous Receiver-Transmitter (UART) connection.

The microcontroller used for this project is the [STM32F411VE Discovery board](https://www.st.com/en/evaluation-tools/32f411ediscovery.html).
The firmware was written using STM's HAL library, as well as compiled and written using the [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html).

# **Setting up the Software**

**Note:** This project was written on Windows 10, there may be issues when attempting to run the python
application on MacOS or Linux Distros.

## **Downloading Dependencies**

**Note:** The project was written with Python 3 and its dependencies installed globally.
A virtual environment may be set up locally if needed. The instructions in this file only enumerate global installation of the dependencies.
The Python application was written using Python 3.7.5+.

1. To install Python 3 on Windows go to the [Python Downloads Page](https://www.python.org/downloads/) and download the most recent version. (Install to PATH)
   **NOTE:** Use both of the following commands on PowerShell or CMD to verify the python installation. - _python --version_ - _python3 --version_

Python may be installed as _python_ or _python3_ depending on whether previous versions were already installed. Use the correct python command for the rest of the commands. The instructions after this point assume Python was installed as _python3_.
**NOTE: Some of the following steps may be skipped if the packages are already installed on your system**

2. Use the following command for opencv install. Click here for the [OpenCV-Python](https://pypi.org/project/opencv-python/) page.

   - _python3 -m pip install opencv-python --upgrade_

3. Use the following command to install _pyzbar_. Click here for the [Pyzbar](https://pypi.org/project/pyzbar/)page.

   - _python3 -m pip install pyzbar --upgrade_

4. Use the following command to install _pyserial_. Click here for the [Pyserial](https://pypi.org/project/pyserial/)page.

   - _python3 -m pip install pyserial --upgrade_

5. Use the following command to install _numpy_. Click here for the [Numpy](https://pypi.org/project/numpy/)page.
   - _python3 -m pip install numpy --upgrade_

## **Running the Python Application**

1. Download the project by either cloning to a local folder, or downloading the repository as a zip file and extracting.

2. Navigate to the project folder on your terminal.
3. Enter either of the following commands to run the python application. Before running, connect a USB webcam to the computer.

   - To run the application with default capture device 0 and no serial communication, run without arguments:

     _python3 Camera_Data_Detection.py_

   - To run the application without serial communication and different capture device:

     _python3 Camera_Data_Detection.py 1_

     **NOTE:** The argument 1 is used to set the openCV video capture device. On laptops, the built-in webcam may be set as capture device 0. Extra USB webcams may be set as capture device 1 or above.

   - To run the application with a specific capture device and serial port use:

     _python3 Camera_Data_Detection.py 1 COM3_

     **NOTE:** The second argument 'COM#' especifies the Windows COM Port for the microcontroller. - On Linux distros or Mac OS, USB devices are usually listed under _/dev/_ as _/dev/TTYSx_ or _/dev/TTYUSBx/_.

     **WARNING:** The STM32F411VE microcontroller does not have a USB-Serial converter chip such as Arduinos have. A separate USB-to-Serial converter must be used with the STM32 pins.
     The **COM PORT** must be available on the computer (Device Manager on Windows) for the Python application to run successfully.
     See the [Serial Communication](##Serial%20Communication) section for instructions on setting up the converter.

# **STM32 Microcontroller**

## **Electrical Components**

## **Serial Communication**

## **Motor Connections**
