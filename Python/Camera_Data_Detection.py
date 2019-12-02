import cv2
import numpy
import sys
import time
import pyzbar.pyzbar as pyzbar
from pyzbar.pyzbar import ZBarSymbol
import serial
import signal


def displayQRCodes(frame, Objects):
    """
     Display barcode and QR code location

     Parameters:
     -----------
     frame : openCV Frame
     Objects : QRCodes Detected

     Return:
     -----------
     center: tuple, (int, int)

     center is a tuple containing the center coordinates of QR Code in frame
    """

    center = (999, 999)
    # Loop over all decoded objects
    for decodedObject in Objects:

        decodedMessage = decodedObject.data.decode()

        points = decodedObject.polygon

        # If the points do not form a quad, find convex hull
        if len(points) > 4:
            hull = cv2.convexHull(
                numpy.array([point for point in points], dtype=numpy.float32))
            hull = list(map(tuple, numpy.squeeze(hull)))
        else:
            hull = points

        # Number of points in the convex hull
        n = len(hull)
        sumx = 0
        sumy = 0
        # Draw the convext hull
        for j in range(0, n):
            cv2.line(frame, hull[j], hull[(j+1) % n], (255, 0, 0), 3)
            sumx = sumx + hull[j].x
            sumy = sumy + hull[j].y

        QR_Center = (int(sumx/n), int(sumy/n))
        center = QR_Center
        center_text = "(" + str(QR_Center[0]) + ", " + str(QR_Center[1]) + ")"
        cv2.circle(frame, QR_Center, 5, (255, 0, 0), -1)
        cv2.putText(frame, center_text, (QR_Center[0]+10, QR_Center[1]-10),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 2)
    return center


def rightMostObject(R_Cor, Y_Cor, G_Cor):
    """
     Select Object Data to send to the Microcontroller.
     When there are multiple objects on video frame. Select the right-most
     object on the frame.

     Parameters
     ----------
     R_Cor: tuple
     Y_Cor: tuple
     G_Cor: tuple

     The tuples are formatted in the form (char, int, int), where:
         - The character denotes the object color
         - The first integer denotes the x position of the object in the frame
         - The second integer denotes the y position of the object in the frame

     Returns
     ----------
     """
    inumpyut_data = [R_Cor, Y_Cor, G_Cor]
    rightMost_xvalue = 0

    # coordinate of value 999 means there is no color currently detected
    if(R_Cor[1] == 999 and Y_Cor[1] == 999 and G_Cor[1] == 999):
        output_data = ('N', 999, 999)
    else:
        for i, coordinate in enumerate(inumpyut_data):
            if(not coordinate[1] == 999):
                if(coordinate[1] >= rightMost_xvalue):
                    output_data = coordinate
                    rightMost_xvalue = coordinate[1]
    return output_data


def main():
    # Check arguments.
    if(len(sys.argv) == 3):
        PORT = sys.argv[2]
        try:
            serial_object = serial.Serial()
            serial_object.baudrate = 115200
            serial_object.port = PORT
            serial_object.open()
        except serial.SerialException as e:
            print('{} could not be accessed'.format(PORT))
            print('Exit on Error: {}'.format(e))
            sys.exit(0)
        SERIAL = True
        CAPTURE_DEVICE = sys.argv[1]
    elif (len(sys.argv) == 2):
        SERIAL = False
        CAPTURE_DEVICE = sys.argv[1]
    else:
        SERIAL = False
        CAPTURE_DEVICE = 1

    # Default Camera Resolution: 640 x 480
    camera = cv2.VideoCapture(int(CAPTURE_DEVICE))

    if not camera.isOpened():
        print('Capture device is unavailable. Try another index.')
        sys.exit(0)

    # Create VideoWriter object.The output is stored in 'avi' file.
    record_Obj = cv2.VideoWriter('Output.avi', cv2.VideoWriter_fourcc(
        'M', 'J', 'P', 'G'), 20, (640, 480))
    if not record_Obj.isOpened():
        print('Could not open the video writing object.')
        print('Application will not record the camera to file.')
        RECORDING = False
    else:
        print('Recording video.')
        RECORDING = True

    # Define the signal call back for exiting this application.
    # Use 'Ctrl + C" to exit.
    def signal_handler(sig, frame):
        print('Pressed Ctrl+C')
        camera.release()
        record_Obj.release()
        cv2.destroyAllWindows()
        sys.exit(0)

    # Set up signal handler
    signal.signal(signal.SIGINT, signal_handler)
    print('To exit the application press \"Ctrl + C\".')

    # variables used to send QR Code data and position
    qrCode = ''
    QR_Center = (0, 0)

    # Initialize the variable to send colored object data to microcontroller
    Object_Data = ('N', 999, 999)

    # HSV Ranges for color detection
    minRed = numpy.array([0, 200, 0])
    maxRed = numpy.array([8, 255, 255])
    minyellow = numpy.array([5, 124, 123])
    maxyellow = numpy.array([30, 255, 255])
    minGreen = numpy.array([34, 50, 50])
    maxGreen = numpy.array([80, 220, 200])
    kernelOpen = numpy.ones((5, 5))
    kernelClose = numpy.ones((20, 20))

    # Capture frames from camera and process
    while True:
        didRead, Frame = camera.read()
        if(not didRead):
            print("Error: Could not read frame.")
            sys.exit(0)

        # Detect QR Codes on the live Frame
        decodedObjects = pyzbar.decode(Frame, symbols=[ZBarSymbol.QRCODE])

        # Verify a QR Code is detected, grab only the first one
        if len(decodedObjects):
            QRData = decodedObjects[0].data
            decodedData = QRData.decode('utf8')

            # Set the decoded QR Code as single Character for microcontroller
            if(decodedData == 'Green'):
                qrCode = 'G'
            elif (decodedData == 'Red'):
                qrCode = 'R'
            elif (decodedData == 'Yellow'):
                qrCode = 'Y'
            elif (decodedData == 'Sorter'):
                qrCode = 'S'
            else:
                qrCode = 'T'
        else:
            # No QR Codes detected
            QRData = ''
            qrCode = 'N'

        # Display the decoded value on frame when QR Code detected
        if QRData:
            Text = "ZBAR : {}".format(QRData.decode('utf8'))
            cv2.putText(Frame, Text, (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 1,
                        (0, 255, 0), 2, cv2.LINE_AA)

        # Highlight the QR Code on the Live Feed
        QR_Center = displayQRCodes(Frame, decodedObjects)

        # convert BGR to HSV
        HSV_Frame = cv2.cvtColor(Frame, cv2.COLOR_BGR2HSV)

        # create the Mask for color detection
        Redmask = cv2.inRange(HSV_Frame, minRed, maxRed)
        Yellowmask = cv2.inRange(HSV_Frame, minyellow, maxyellow)
        Greenmask = cv2.inRange(HSV_Frame, minGreen, maxGreen)

        # Remove noise from original frame (erosion followed by dilation)
        RedmaskOpen = cv2.morphologyEx(Redmask, cv2.MORPH_OPEN, kernelOpen)
        # Close holes in object (dilation followed by erosion)
        RedmaskClose = cv2.morphologyEx(
            RedmaskOpen, cv2.MORPH_CLOSE, kernelClose)
        RedmaskFinal = RedmaskClose

        # Remove noise from original frame (erosion followed by dilation)
        YellowmaskOpen = cv2.morphologyEx(
            Yellowmask, cv2.MORPH_OPEN, kernelOpen)
        # Close holes in object (dilation followed by erosion)
        YellowmaskClose = cv2.morphologyEx(
            YellowmaskOpen, cv2.MORPH_CLOSE, kernelClose)
        YellowmaskFinal = YellowmaskClose

        # Remove noise from original frame (erosion followed by dilation)
        GreenmaskOpen = cv2.morphologyEx(Greenmask, cv2.MORPH_OPEN, kernelOpen)
        # Close holes in object (dilation followed by erosion)
        GreenmaskClose = cv2.morphologyEx(
            GreenmaskOpen, cv2.MORPH_CLOSE, kernelClose)
        GreenmaskFinal = GreenmaskClose

        # Detect Red Countours
        Red_contours, h = cv2.findContours(
            RedmaskFinal.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        if(len(Red_contours) == 0):
            R_cX = 999
            R_cY = 999
        else:
            # Verify that the contour area is greater than 1000
            # otherwise set as if no contours seen
            if(cv2.contourArea(Red_contours[0]) < 1000):
                R_cX = 999
                R_cY = 999
            else:
                # Calculate the moments of the red object
                Red_Moments = cv2.moments(Red_contours[0])
                if Red_Moments["m00"] != 0:
                    R_cX = int(Red_Moments["m10"] / Red_Moments["m00"])
                else:
                    R_cX = 0
                # Draw rectangle around object as well as a point for center.
                R_x, R_cY, w, h = cv2.boundingRect(Red_contours[0])
                cv2.rectangle(Frame, (R_x, R_cY),
                              (R_x+w, R_cY+h), (0, 0, 255), 2)
                cv2.circle(Frame, (R_cX, R_cY), 5, (255, 0, 0), -1)
                Text = "(" + str(R_cX) + ", " + str(R_cY) + ")"
                cv2.putText(Frame, Text, (R_cX, R_cY+int(h/2)),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 2)

        # Detect Yellow Countours
        Yellow_contours, h = cv2.findContours(
            YellowmaskFinal.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        if(len(Yellow_contours) == 0):
            Y_cX = 999
            Y_cY = 999
        else:
            # Verify that the contour area is greater than 1000
            # otherwise set as if no contours seen
            if(cv2.contourArea(Yellow_contours[0]) < 1000):
                Y_cX = 999
                Y_cY = 999
            else:
                # Calculate the moments of the yellow object
                Yellow_Moments = cv2.moments(Yellow_contours[0])
                if Yellow_Moments["m00"] != 0:
                    Y_cX = int(Yellow_Moments["m10"] / Yellow_Moments["m00"])
                else:
                    Y_cX = 0
                # Draw rectangle around object as well as a point for center.
                Y_x, Y_cY, w, h = cv2.boundingRect(Yellow_contours[0])
                cv2.rectangle(Frame, (Y_x, Y_cY),
                              (Y_x+w, Y_cY+h), (0, 200, 200), 2)
                cv2.circle(Frame, (Y_cX, Y_cY), 5, (255, 0, 0), -1)
                Text = "(" + str(Y_cX) + ", " + str(Y_cY) + ")"
                cv2.putText(Frame, Text, (Y_cX, Y_cY+int(h/2)),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 2)

        # Detect Green Countours
        Green_contours, h = cv2.findContours(
            GreenmaskFinal.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        if(len(Green_contours) == 0):
            G_cX = 999
            G_cY = 999
        else:
            # Verify that the contour area is greater than 1000
            # otherwise set as if no contours seen
            if(cv2.contourArea(Green_contours[0]) < 200):
                G_cX = 999
                G_cY = 999
            else:
                # Calculate the moments of the green object
                Green_Moments = cv2.moments(Green_contours[0])
                if Green_Moments["m00"] != 0:
                    G_cX = int(Green_Moments["m10"] / Green_Moments["m00"])
                else:
                    G_cX = 0

                # Draw rectangle around object as well as a point for center.
                G_x, G_cY, G_w, G_h = cv2.boundingRect(Green_contours[0])
                cv2.rectangle(Frame, (G_x, G_cY),
                              (G_x+G_w, G_cY+G_h), (0, 255, 0), 2)
                cv2.circle(Frame, (G_cX, G_cY), 5, (255, 0, 0), -1)
                Text = "(" + str(G_cX) + ", " + str(G_cY) + ")"
                cv2.putText(Frame, Text, (G_cX, G_cY+int(G_h/2)),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 2)

        # Detect the right-most object on screen.
        # Note: If no object is on screen the coordinates are 999
        Object_Data = rightMostObject(
            ('R', R_cX, R_cY), ('Y', Y_cX, Y_cY), ('G', G_cX, G_cY))

        # Format the data to be sent to the microcontroller
        # Integers are written as 3 digits, i.e. 1 is 001, 20 is 020, etc.
        # In total 14 bytes are sent to the microcontroller at a time.
        data = Object_Data[0] + format(Object_Data[1], '03d') + format(
            Object_Data[2], '03d') + qrCode + format(QR_Center[0], '03d') \
            + format(QR_Center[1], '03d')

        if SERIAL:
            serial_object.write(data.encode())
        if RECORDING:
            record_Obj.write(Frame)
        # cv2.imshow("cameraR", Redmask)
        # cv2.imshow("cameraRO", RedmaskOpen)
        # cv2.imshow("cameraRC", RedmaskClose)
        cv2.imshow("camera", Frame)
        cv2.waitKey(10)


if __name__ == '__main__':
    main()
