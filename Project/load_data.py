import numpy as np
import matplotlib.pyplot as plt
import os
import cv2

DATA_DIR = "/Users/yunhuima/Documents/Python/Traffic"

CATEGORIES = ["Others","Speed_limit_30", "Stop_Sign", "Right_turn", "Left_turn"]
IMG_SIZE = 50

training_data = []

def create_training_data():
    for category in CATEGORIES:
        path = os.path.join(DATA_DIR, category)
        class_num = CATEGORIES.index(category)
        print(class_num)
        for img in os.listdir(path):
            try:
                img_array = cv2.imread(os.path.join(path, img), cv2.IMREAD_GRAYSCALE)
                new_array = cv2.resize(img_array, (IMG_SIZE, IMG_SIZE))
                training_data.append([new_array, class_num])
            except Exception as e:
                pass

create_training_data()

import random
random.shuffle(training_data)

train_images = []
train_labels = []

for image, label in training_data:
    train_images.append(image)
    train_labels.append(label)

train_images = np.array(train_images).reshape(-1, IMG_SIZE, IMG_SIZE, 1)

print(len(train_images))

import pickle

pickle_out = open("/Users/yunhuima/Documents/Python/train_images.pickle", "wb")
pickle.dump(train_images, pickle_out)
pickle_out.close()

pickle_out = open("/Users/yunhuima/Documents/Python/train_labels.pickle", "wb")
pickle.dump(train_labels, pickle_out)
pickle_out.close()
