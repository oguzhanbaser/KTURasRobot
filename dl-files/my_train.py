
import keras
from keras.layers.merge import Concatenate
from keras.models import Sequential, model_from_json, Model
from keras.layers import Activation, Dense, Conv2D, Flatten, Dropout, concatenate, MaxPool2D, Merge, Input
# from sklearn.model_selection import train_test_split

import numpy as np
from numpy import inf
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
        scan_i.append(np.concatenate((np_scan[i][0:15], np_scan[i][-16:-1]), axis=0))
        cmd.append(np.append(np_cmd_x[i], np_cmd_y[i]))
    except cv2.error as e:
        pass
    # cv2.imshow('img', img)
    # cv2.waitKey(100)


np_scan_i = np.array(scan_i)
np_imgs = np.array(imgs)
np_cmd = np.array(cmd)

np_scan_i[np_scan_i == inf] = 3.5

np_imgs = np_imgs.astype('float32')
np_imgs /= 255.0

print np_imgs.shape
print np_scan_i.shape
print np_cmd.shape

print np_scan_i

# exit(0)

# model1 = Sequential()
# model2 = Sequential()
#
# model1.add(Conv2D(16, (3, 3), input_shape=(240, 320, 3), activation='elu', strides=(2, 2)))
# model2.add(Dense(24, input_shape=(30, ), activation='elu'))
#
# model1.add(Conv2D(24, (3, 3), activation='elu', strides=(2, 2)))
# model1.add(MaxPool2D(pool_size=(2, 2)))
# model2.add(Dense(32, activation='elu'))
# model2.add(Dropout(0.25))
#
# model1.add(Conv2D(32, (3, 3), activation='elu', strides=(2, 2)))
# model1.add(Dropout(0.25))
# model2.add(Dense(48, activation='relu'))
# model2.add(Dropout(0.25))
#
# model1.add(Flatten())
#
# model1.add(Dense(32, activation='relu'))
# model1.add(Dropout(0.25))
# model2.add(Dense(64, activation='relu'))
# model2.add(Dropout(0.25))
#
# merged = Merge([model1, model2], mode='concat')
#
# final_model = Sequential()
# final_model.add(merged)
# final_model.add(Dense(2, activation='sigmoid'))



img_inputs = Input(shape=(240, 320, 3))
scan_inputs = Input(shape=(30, ))

conv_1 = Conv2D(32, (4, 4), strides=(2, 2))(img_inputs)
act_1 = Activation('elu')(conv_1)

conv_2 = Conv2D(32, (4, 4), strides=(2, 2))(act_1)
act_2 = Activation('elu')(conv_2)

conv_3 = Conv2D(32, (2, 2), strides=(1, 1))(act_2)
act_3 = Activation('elu')(conv_3)

pool_1 = MaxPool2D(pool_size=(2,2), strides=(2,2))(act_3)

flat_1 = Flatten()(pool_1)

image_fc = Dense(32)(flat_1)
# act_4 = Activation('elu')(image_fc)

scan_fc = Dense(32)(scan_inputs)
# act_5 = Activation('relu')(scan_fc)

concatenate_layer = concatenate([image_fc, scan_fc])

all_fc = Dense(10)(concatenate_layer)
act_6 = Activation('relu')(all_fc)

outputs = Dense(2)(act_6)

final_model = Model(inputs=[img_inputs, scan_inputs], outputs=outputs)

final_model.compile(loss='mean_squared_error',
                    optimizer=keras.optimizers.Adam(lr=0.0001), metrics=["accuracy"])

final_model.summary()

final_model.fit([np_imgs, np_scan_i], np_cmd, shuffle=True, validation_data=([np_imgs, np_scan_i], np_cmd),
                epochs=10, batch_size=1)

final_model.save_weights('my_files/my_model.h5')
with open('my_files/my_model.json', 'w') as json_file:
    json_file.write(final_model.to_json())
