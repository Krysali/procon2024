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

def MapGenerate():
    # Get user input picture path!!!!!!
    image_path = os.path.join(os.path.dirname(__file__), "image.png")
    width = int(input("Enter the desired width (64-256): "))
    height = int(input("Enter the desired height (64-256): "))

    # Convert the image
    try:
        grayscale_array = convert_to_4_color_grayscale(image_path, width, height)
        # print(grayscale_array)


    except ValueError as e:
        print("Error:", e)


    # Flatten the array and shuffle it
    flat_array = grayscale_array.flatten()
    random.shuffle(flat_array)

    # Reshape the shuffled array to the original dimensions
    shuffled_array = flat_array.reshape(grayscale_array.shape)

    # Save the shuffled array to a text file
    with open("output_array.txt", "w") as file:
        for row in grayscale_array:
            file.write(" ".join(map(str, row)) + "\n")


    start_strings = ["".join(map(str, row)) for row in shuffled_array]
    goal_strings = ["".join(map(str, row)) for row in grayscale_array]
    return (shuffled_array, grayscale_array)

    # # width
    # print(grayscale_array.shape[1])
    # #height 
    # print(grayscale_array.shape[0])
    # #start state np array  (shuffled_array)
    # print(start_strings)
    # # goal state np array (grayscale_array)
    # print (goal_strings)
