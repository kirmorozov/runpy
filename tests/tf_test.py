import tensorflow as tf
import os
from tensorflow import keras
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
import numpy as np

# tf.logging.set_verbosity(tf.logging.ERROR)
# mymodel = Sequential()
# based on example https://gist.github.com/Orbifold/3f51f3c21d348873f4025813bfcf5614
def trainAndSave():
    x_input = np.array([[1,2,3,4,5],[1,2,3,4,6]])
    y_input = np.array([[10],[10]])


    model = Sequential()
    model.add(Dense(units=32, activation="tanh", input_dim=x_input.shape[1], kernel_initializer='random_normal'))
    model.add(Dense(units=1, kernel_initializer='random_normal'))


    model.compile(loss='mse', optimizer='sgd', metrics=['accuracy'])

    model.summary()

    pathToSave = os.path.dirname(__file__) + "/keras_model"
    history = model.fit(x_input, y_input, epochs=20, batch_size=32)
    model.predict(x_input, batch_size=128)
    print("Saved to " + pathToSave)
    model.save(pathToSave)

def loadModel():
    pathToLoad = os.path.dirname(__file__) + "/keras_model"
    model = keras.models.load_model(pathToLoad)
    # model.summary()
    mymodel = model
    return model


class Model(object):
    _instance = None
    model = None
    def __new__(cls):
        if cls._instance is None:
            # print('Creating the object')
            cls._instance = super(Model, cls).__new__(cls)
            # Put any initialization here.
            # tf.logging.set_verbosity(tf.logging.ERROR)
            tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)

            cls._instance.model = loadModel()
        return cls._instance


def predictOne(singleInput):
    my = Model()
    res = my.model.predict(np.array([singleInput]))
    return res[0].tolist()

def predictMany(singleInput):
    my = Model()
    res = my.model.predict(np.array(singleInput))
    return res.tolist()

if __name__=="__main__":
    trainAndSave()
    Model()
    print(predictOne([1,2,5,4,5]))
    print(predictOne([1,2,4,5,5]))


