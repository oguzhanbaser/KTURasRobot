
import keras
from keras.layers.merge import Concatenate
from keras.models import Sequential, model_from_json, Model
from keras.layers import Activation, Dense, Conv2D, Flatten, Dropout, concatenate, MaxPool2D, Merge, Input
# from sklearn.model_selection import train_test_split

import numpy as np
import cv2

np_scan = np.load('my_files/np_scan.npy')
np_frame_name =  np.load('my_files/np_frame_name.npy')
np_cmd_x = np.load('my_files/np_cmd_x.npy')
np_cmd_y = np.load('my_files/np_cmd_y.npy')

imgs = []
scan_i = []
cmd = []

for i in range(len(np_frame_name)):
    # print np_frame_name[i]
    try:
        img = cv2.imread('my_files/' + np_frame_name[i])
        img = cv2.resize(img, (320, 240))
        imgs.append(img)
        scan_i.append(np.concatenate((np_scan[i][0:30], np_scan[i][-31:-1]), axis=0))
        cmd.append(np.append(np_cmd_x[i], np_cmd_y[i]))
    except cv2.error as e:
        pass
    # cv2.imshow('img', img)
    # cv2.waitKey(100)


np_scan_i = np.array(scan_i)
np_imgs = np.array(imgs)
np_cmd = np.array(cmd)

np_imgs = np_imgs.astype('float32')
np_imgs /= 255.0

print np_imgs.shape
print np_scan_i.shape
print np_cmd.shape

# print np_scan_i[0][0]

# exit(0)

img_inputs = Input(shape=(240, 320, 3))
scan_inputs = Input(shape=(60, ))

conv_1 = Conv2D(24, (5, 5), strides=(2, 2), activation='elu')(img_inputs)

conv_2 = Conv2D(36, (5, 5), strides=(2, 2), activation='elu')(conv_1)

conv_3 = Conv2D(48, (5, 5), strides=(2, 2), activation='elu')(conv_2)

conv_4 = Conv2D(64, (3, 3), activation='elu')(conv_3)

conv_5 = Conv2D(64, (3, 3), activation='elu')(conv_4)

drop1 = Dropout(0.2)(conv_5)

pool_1 = MaxPool2D(pool_size=(2,2), strides=(2,2))(drop1)

flat_1 = Flatten()(pool_1)

image_fc = Dense(100, activation='elu')(flat_1)

image_fc_2 = Dense(50, activation='elu')(image_fc)

image_fc_3 = Dense(10, activation='elu')

scan_fc = Dense(32, activation='relu')(scan_inputs)

concatenate_layer = concatenate([image_fc, scan_fc])

all_fc = Dense(10, activation='tanh')(concatenate_layer)

outputs = Dense(2, activation='tanh')(all_fc)

final_model = Model(inputs=[img_inputs, scan_inputs], outputs=outputs)

final_model.compile(loss='mean_squared_error',
                    optimizer=keras.optimizers.Adam(lr=0.0001), metrics=["accuracy"])

final_model.summary()

final_model.fit([np_imgs, np_scan_i], np_cmd, shuffle=True,
                epochs=10, batch_size=1)

final_model.save_weights('my_files/my_model.h5')
with open('my_files/my_model.json', 'w') as json_file:
    json_file.write(final_model.to_json())
