'''
Copyright (c) 2019 STMicroelectronics
This work is licensed under the MIT license
'''

# STM32Cube.AI on OpenMV MNIST Example

import sensor, image, time, nn_st
import json
from pyb import UART


sensor.reset()                      # Reset and initialize the sensor.
sensor.set_contrast(3)
sensor.set_brightness(0)
sensor.set_auto_gain(False)
sensor.set_auto_exposure(False)
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to Grayscale
sensor.set_framesize(sensor.QQVGA)   # Set frame size to 80x60
sensor.skip_frames(time = 2000)     # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.

uart = UART(3, 9600)

# [STM32Cube.AI] Initialize the network
net = nn_st.loadnnst('network')

traffic = ["Others","Speed_limit_30", "Stop_Sign", "Right_turn","Left_turn"]

while(True):
    clock.tick()             # Update the FPS clock.
    img = sensor.snapshot()  # Take a picture and return the image.
    data = []
    x = 0
    # [STM32Cube.AI] Run the inference

    out = net.predict(img)
    if out.index(max(out)) == 0:
        img.draw_string(0, 0, str(traffic[0]))
        x = 0
        #print(traffic[0])
    elif out.index(max(out)) == 1:
        img.draw_string(0, 0, str(traffic[1]))
        x = 1
        #print(traffic[1])
    elif out.index(max(out)) == 2:
        img.draw_string(0, 0, str(traffic[2]))
        x = 2
        #print(traffic[2])
    elif out.index(max(out)) == 3:
        img.draw_string(0, 0, str(traffic[3]))
        x = 3
        #print(traffic[3])
    elif out.index(max(out)) == 4:
        img.draw_string(0, 0, str(traffic[4]))
        x = 4
        #print(traffic[4])


    data.append(x)
    data_out = json.dumps(set(data))
    uart.write(data_out)

   # print('Network argmax output: {}'.format( out.index(max(out)) ))
    #img.draw_string(0, 0, str(out.index(max(out))))
    print('FPS {}'.format(clock.fps())) # Note: OpenMV Cam runs about half as fast when connected
