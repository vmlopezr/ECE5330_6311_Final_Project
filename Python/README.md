# Program Description
This folder contains the python code for the Camera Capture and data collection.

## Video Capture
The program uses openCV to capture live video feed from a USB Webcam, and uses openCV functions to detect contours on the frames based on HSV color ranges. With detected contours, it is then possible to add bounding rectangles to mark specific objects in the video frames.

The program also uses the pyzbar library to detect and decode QR Codes on the video frames. After detecting the frames, it is then possible to highlight the QR Codes in the video frames.

## Data Collection

The program collects data about the colored objects detected as well as the QR Codes detected. 

After detecting colored objects in a frame, the x and y positions of the right-most colored object in the frame is collected along with its colored.

With regards to QR Codes, the program collects the x and y positions of the center of the QR Code in the frame, as well as the decoded message. 

The program is constrained to detected only Red, Green, and Yellow objects.
The data collected for any of those objects is saved as a string in the following format:
1. Red Objects: "Rx1x2x3y1y2y3"
    - The 'R' character stands for the detected color Red.
    - x1, x2, and x3 are the digits for the x position.  The position is saved as an integer with leading zeroes, for example 003 or 023, to set the character size.
    - y1, y2, and y3 are the digits for the y position.  The position is saved as an integer with leading zeroes, for example 003 or 023, to set the character size.
2. Yellow Objects : "Yx1x2x3y1y2y3"  

3. Green Objects : "Gx1x2x3y1y2y3"  

4. No object detected: "N999999"
    - The data is set this way to specify that no colored objects are displayed on the frame.  
    
The data collected for any of QR Codes is saved as a string in the following format:
1. 'Red': "Rx1x2x3y1y2y3"
    - The 'R' character stands for the decoded message 'Red'.
    - x1, x2, and x3 are the digits for the x position.  The position is saved as an integer with leading zeroes, for example 003 or 023, to set the character size.
   - y1, y2, and y3 are the digits for the y position.  The position is saved as an integer with leading zeroes, for example 003 or 023, to set the character size.
2. 'Yellow' : "Yx1x2x3y1y2y3"  

3. 'Green' : "Gx1x2x3y1y2y3"  

4. 'Transport' : 'Tx1x2x3y1y2y3"
    - The QR Code with the encoded message 'Transport' is the object pick up point.
4. No QR Code detected: "N999999"
## Data transfer

After the data for the colored object and QR Code is collected, it is concatenated into a 14 byte string. For example:
  - "R320240Y130100"
    - The first 7 characters specify that the center of a Red object is located at pixel 320 in the x position and pixel 240 in the y position of the frame.
    - The next 7 characters specify that the center of a QR Code with 'Yellow' as the message is located at pixel 130 in the x position and pixel 100 in the y position of the frame.
    
This data is then sent to the specified COM port for the USB-to-TTL adapter connected the to the USART peripheral of the STM32F4 Discovery Board. In this manner, the microcontroller is always receiving data on any objects or QR codes displayed on the camera.
