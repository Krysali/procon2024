import numpy as np
import cv2
import random
import os
import pickle

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
    image_path =  os.path.join(os.path.dirname(__file__), "images/" + str(image_index) + ".jpg")
    
    height = n
    width = m

    # Convert the image
    grayscale_array = convert_to_4_color_grayscale(image_path, width, height)
    
    # Save the array in a .pkl file
    file_name = "encoded_data/" + str(n) + "x" + str(n) + "/" + str(image_index) + ".pkl"
    with open(os.path.join(os.path.dirname(__file__), file_name), 'wb') as f:
        pickle.dump(grayscale_array, f)

def gen_and_save_goal_states():
    sz = 32
    while sz <= 256:

        for image_index in range(124):

            GenerateGoalState(sz, sz, image_index)

        sz *= 2

def delete_files(directory, pattern):
  """
  Deletes files in a directory matching a specific pattern.

  Args:
      directory (str): The path to the directory containing the files.
      pattern (str): The pattern to match for file deletion.
  """

  # Get a list of files in the directory
  files = os.listdir(directory)

  # Delete files matching the pattern
  for file in files:
    if pattern in file:
      os.remove(os.path.join(directory, file))

def delete_jpg_files():
    sz = 32
    while sz <= 256:
        directory = os.path.join(os.path.dirname(__file__), "encoded_data/" + str(sz) + "x" + str(sz))  # Replace with the actual directory path
        pattern = ".jpg"  # Delete all files with the ".txt" extension

        delete_files(directory, pattern)
        sz *= 2

gen_and_save_goal_states()
