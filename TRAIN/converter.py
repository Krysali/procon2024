import numpy as np
import cv2
import random
import os

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
    if not 64 <= width <= 256 or not 64 <= height <= 256:
        raise ValueError("Width and height must be between 64 and 256.")

    # Load the image
    img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

    # Resize the image
    img = cv2.resize(img, (width, height))

    # Quantize the grayscale values to 4 levels
    bins = np.array([0, 64, 128, 192, 256])
    quantized_img = np.digitize(img, bins) - 1

    return quantized_img

def MapGenerate(n, m, rand):
    # Get user input picture path
    image_path = os.path.join(os.path.dirname(__file__), "image.png")
    
    height = n
    width = m

    # Convert the image
    grayscale_array = convert_to_4_color_grayscale(image_path, width, height)

    # Flatten the array and shuffle it
    flat_array = grayscale_array.flatten()
    random.shuffle(flat_array)

    # Reshape the shuffled array to the original dimensions
    shuffled_array = flat_array.reshape(grayscale_array.shape)

    # if random
    random.shuffle(flat_array)
    shuffled_array_prime = flat_array.reshape(grayscale_array.shape)
    
    if rand:
        return(shuffled_array, shuffled_array_prime)
    else:
        return (shuffled_array, grayscale_array)
