import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

import tensorflow as tf
import numpy as np
#import matplotlib.pyplot as plt
from tensorflow.keras import datasets, layers, models
import pickle



image_pickle_in = open("/Users/yunhuima/Documents/Python/train_images.pickle", "rb")
train_images = pickle.load(image_pickle_in)
label_pickle_in = open("/Users/yunhuima/Documents/Python/train_labels.pickle", "rb")
train_labels = pickle.load(label_pickle_in)

test_image_pickle_in = open("/Users/yunhuima/Documents/Python/test_images.pickle", "rb")
test_images = pickle.load(test_image_pickle_in)
test_label_pickle_in = open("/Users/yunhuima/Documents/Python/test_labels.pickle", "rb")
test_labels = pickle.load(test_label_pickle_in)

train_images = np.array(train_images/255.0)
train_labels = np.array(train_labels)

test_images = np.array(test_images)
test_labels = np.array(test_labels)

model = models.Sequential()
model.add(layers.Conv2D(32, (3, 3), activation='relu', input_shape=(50, 50, 1), padding='same'))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(64, (3, 3), activation='relu', padding='same'))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(64, (3, 3), activation='relu', padding='same'))
model.add(layers.MaxPooling2D((2, 2)))

model.add(layers.Flatten())
model.add(layers.Dense(5, activation='softmax'))

model.summary()

#Training the model
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

model.fit(train_images, train_labels, epochs=8)

model.save('/Users/yunhuima/Documents/Python/traffic_sign.h5')

'''
converter = tf.lite.TFLiteConverter.from_saved_model("/Users/yunhuima/Documents/Python/traffic_sign.model")
#converter.optimizations = [tf.lite.Optimize.OPTIMIZE_FOR_SIZE]
tflite_model = converter.convert()
open("/Users/yunhuima/Documents/Python/traffic_sign_tfLite_l.tflite", "wb").write(tflite_model)

'''
