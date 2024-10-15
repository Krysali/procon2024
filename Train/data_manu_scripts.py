import numpy as np
import cv2
import random
import os
import pickle

import time
from typing import List
from environments.Game import GmState
from environments.environment_abstract import Environment, State

def convert_to_4_color_grayscale(image_path, width, height):
    """
    Converts an image to a 4-color grayscale array.

    Args:
        image_path: Path to the input image.
        width: Desired width of the output image.
        height: Desired height of the output image.

    Returns:
        A NumPy array representing the 4-color grayscale image.
    """

  # Check if size is within the valid range
    if not 6 <= width <= 256 or not 6 <= height <= 256:
        raise ValueError("Width and height must be between 64 and 256.")

    # Load the image
    img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

    # Resize the image
    img = cv2.resize(img, (width, height))

    # Quantize the grayscale values to 4 levels
    bins = np.array([0, 64, 128, 192, 256])
    quantized_img = np.digitize(img, bins)

    return quantized_img

def GenerateGoalState(n, m, image_index):
    # Get user input picture path
    image_path =  os.path.join(os.path.dirname(__file__), "validation_images/" + str(image_index) + ".jpg")
    
    height = n
    width = m

    # Convert the image
    grayscale_array = convert_to_4_color_grayscale(image_path, width, height)
    
    GmState()

    # Save the array in a .pkl file
    file_name = "encoded_validation_data/" + str(n) + "x" + str(n) + "/" + str(image_index) + ".pkl"
    with open(os.path.join(os.path.dirname(__file__), file_name), 'wb') as f:
        pickle.dump(grayscale_array, f)

