# **ECE 5330 Final Project: Lego Color Sorting via Camera Feedback**

Clone the repo to a local folder in your computer.
**Note:** This application uses Python 3.5 and up. Verify that the python installation location is in PATH.

# **Table of Contents**
- [Project Description](#Project%20Description)
- [Setting up the Software](#Setting%20up%20the%20project)
  - [Downloading Dependencies](##Downloading%20Dependencies)
  - [Running the Python Application](##Running%20the%20Python%20Application)
- [STM32 Microcontroller](#STM32%20Microcontroller)
  - [Electrical Components](##Electrical%20Components)
  - [Serial Communication](##Serial%20Communication)
  - [Motor Connections](##Motor%20Connections)

The project is composed of two sections:
1. The python application: Camera_Data_Detection.py.
2. The STM32F411VE microcontroller program located in the STM folder.

**Note:** The python application may be run independently without the microcontroller.

 ```diff
  -[WARNING:]
    -  The microcontroller program will only work correctly when the motor, L298N, and USB Serial connections are set correctly.
    -The python application must be running for the microcontroller to sucessfully maneuver the OWI arm.
  ```

The python application uses the opencv in conjuction with a STM32 microcontroller.
Camera data is sent to the STM32 microcontroller via Universal Asynchronous Receiver-Transmitter (UART) connection.

The microcontroller used for this project is the [STM32F411VE Discovery board](https://www.st.com/en/evaluation-tools/32f411ediscovery.html).
The firmware was written using STM's HAL library, as well as compiled and written using the [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html).


# **Setting up the Software**
**Note:** This project was written on Windows 10,  there may be issues when attempting to run the python
application on MacOS or Linux Distros.
## **Downloading Dependencies**
**Note:** The project was written with Python 3 and its dependencies installed globally.
A virtual environment may be set up locally if needed. The instructions in this file only enumerate global installation of the dependencies.
The Python application was written using Python 3.7.5+.
  1. To install Python 3 on Windows go to the [Python Downloads Page](https://www.python.org/downloads/) and download the most recent version. (Install to PATH)
  **NOTE:** Use both of the following commands on PowerShell or CMD to verify the python installation.
      - *python --version*
      - *python3 --version*

  Python may be installed as *python* or *python3* depending on whether previous versions were already installed. Use the correct python command for the rest of the commands. The instructions after this point assume Python was installed as *python3*.
**NOTE: Some of the following steps may be skipped if the packages are already installed on your system**

  2. Use the following command for opencv install. Click here for the [OpenCV-Python](https://pypi.org/project/opencv-python/) page.
      - *python3 -m pip install opencv-python --upgrade*

  3. Use the following command to install *pyzbar*. Click here for the [Pyzbar](https://pypi.org/project/pyzbar/)page.
      - *python3 -m pip install pyzbar --upgrade*

  4. Use the following command to install *pyserial*. Click here for the [Pyserial](https://pypi.org/project/pyserial/)page.
      - *python3 -m pip install pyserial --upgrade*

  5. Use the following command to install *numpy*. Click here for the [Numpy](https://pypi.org/project/numpy/)page.
      - *python3 -m pip install numpy --upgrade*

## **Running the Python Application**
  1. Download the project by either cloning to a local folder, or downloading the repository as a zip file and extracting.

  2. Navigate to the project folder on your terminal.
  3. Enter either of the following commands to run the python application. Before running, connect a USB webcam to the computer.
      - To run the application with default capture device 0 and no serial communication, run without arguments:

        *python3 Camera_Data_Detection.py*

      - To run the application without serial communication and different capture device:

        *python3 Camera_Data_Detection.py 1*

        **NOTE:** The argument 1 is used to set the openCV video capture device. On laptops, the built-in webcam may be set as capture device 0. Extra USB webcams may be set as capture device 1 or above.

      - To run the application with a specific capture device and serial port use:

        *python3 Camera_Data_Detection.py 1 COM3*

        **NOTE:** The second argument 'COM#' especifies the Windows COM Port for the microcontroller.
          - On Linux distros or Mac OS, USB devices are usually listed under */dev/* as */dev/TTYSx* or */dev/TTYUSBx/*.

        **WARNING:** The STM32F411VE microcontroller does not have a USB-Serial converter chip such as Arduinos have. A separate USB-to-Serial converter must be used with the STM32 pins.
     The **COM PORT** must be available on the computer (Device Manager on Windows) for the Python application to run successfully.
     See the [Serial Communication](##Serial%20Communication) section for instructions on setting up the converter.
# **STM32 Microcontroller**
  ## **Electrical Components**
  ## **Serial Communication**
  ## **Motor Connections**
