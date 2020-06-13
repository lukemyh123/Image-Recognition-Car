import sensor, image, time
import json
from pyb import UART
import random

green_threshold   = ( 60,  100,  -71,   -12,   23,   127)


sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.
clock = time.clock() # Tracks FPS.

uart = UART(3, 9600)

def find_max_blob(blobs):
    #find the max blob
    max_area = 0
    for blob in blobs:
        if blob.w() * blob.h() > max_area:
            max_blob = blob
            max_area = blob.w() * blob.h()
    return max_blob


while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.

    blobs = img.find_blobs([green_threshold])

    if blobs:
        data = []
        max_blob = find_max_blob(blobs)

        img.draw_rectangle(max_blob.x(), max_blob.y(), max_blob.w(), max_blob.h()) # rect
        img.draw_cross(max_blob.cx(), max_blob.cy())

        #data that we need to send;
        cx = max_blob.cx()
        #cy = max_blob.cy()
        blob_size = int(max_blob.w() * max_blob.h())
        data.append((cx, blob_size))

        data_out = json.dumps(set(data))
        uart.write(data_out)
        print(data)
    else:
        data = []
        cx = 0
        blob_size = 0;
        data.append((cx, blob_size))

        data_out = json.dumps(set(data))
        uart.write(data_out)
        print(data)

