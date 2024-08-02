from PIL import Image
import numpy as np
import random
import matplotlib.pyplot as plt

# zurag 
img = Image.open('data1.png')

# zurgiin hemjeeg tohiruulah
max_size = (256, 256)
img.thumbnail(max_size)


img = img.convert('L')


img_array = np.array(img)

# zurgiin hemjee nygtarshalaas shaltgaalj (0, 1, 2, 3 toonuudaar ilerhiileh)
def d_quantization(size):
    return [0, 1, 2, 3]

# pixel bolgoh
num_pixels = img_array.size
quantization_levels = d_quantization(num_pixels)


def quantize(value):
    return min(quantization_levels, key=lambda x: abs(x - value // (256 // len(quantization_levels))))


quantized_array = np.vectorize(quantize)(img_array)

# die cut 
def generate_fixed_die(size, die_type):
    if die_type == 1:
        return np.ones((size, size), dtype=int)
    elif die_type == 2:
        return np.fromfunction(lambda i, j: (i % 2 == 0).astype(int), (size, size))
    elif die_type == 3:
        return np.fromfunction(lambda i, j: (j % 2 == 0).astype(int), (size, size))
    else:
        raise ValueError("!")

def a_die(board, die, position, direction):
    y, x = position
    die_height, die_width = die.shape
    
    # die cutlah yvtsad boardos iluu garsan esehiig shalgah 
    y_end = min(y + die_height, board.shape[0])
    x_end = min(x + die_width, board.shape[1])
    
    
    removed_pieces = board[y:y_end, x:x_end][die[:y_end-y, :x_end-x] == 1]
    
    # die cutlasan hesegee shiljuuleh
    if direction == 'left':
        board[y:y_end, :x_end] = np.roll(board[y:y_end, :x_end], -1, axis=1)
    elif direction == 'right':
        board[y:y_end, x:x_end] = np.roll(board[y:y_end, x:x_end], 1, axis=1)
    elif direction == 'up':
        board[:y_end, x:x_end] = np.roll(board[:y_end, x:x_end], -1, axis=0)
    elif direction == 'down':
        board[y:y_end, x:x_end] = np.roll(board[y:y_end, x:x_end], 1, axis=0)
    else:
        raise ValueError("!")
    
    # left right, up down oos shaltgaal cutlasan hesgee butsaaj hiih
    if direction in ['left', 'right']:
        for i, piece in enumerate(removed_pieces):
            if y < board.shape[0] and x_end - len(removed_pieces) + i < board.shape[1]:
                board[y + i // die_width, x_end - len(removed_pieces) + i] = piece
    elif direction in ['up', 'down']:
        for i, piece in enumerate(removed_pieces):
            if y_end - len(removed_pieces) + i < board.shape[0] and x < board.shape[1]:
                board[y_end - len(removed_pieces) + i, x + i % die_width] = piece

def random_die_moves(board, num_moves):
    height, width = board.shape
    die_types = [1, 2, 3]
    for _ in range(num_moves):
        die_size = random.randint(1, 3)  # die size aa 1x1, 3x3 aar tohiruulsan 
        die_type = random.choice(die_types)
        die = generate_fixed_die(die_size, die_type)
        
        y = random.randint(0, height - die_size)
        x = random.randint(0, width - die_size)
        direction = random.choice(['left', 'right', 'up', 'down'])
        
        a_die(board, die, (y, x), direction)

def upper_left(board, num_moves):
    
    die_size = random.randint(1, 3)
    die_type = random.choice([1, 2, 3])
    die = generate_fixed_die(die_size, die_type)
    direction = random.choice(['left', 'right', 'up', 'down'])
    
    a_die(board, die, (0, 0), direction)
    
    # 
    random_die_moves(board, num_moves - 1)

# final state iig gargaj avna
def t_final_state(board, num_moves):
    upper_left(board, num_moves)
    return board

# cut lah toogoo garaar oruulna
cut = int(input("cut number: "))
final_state = t_final_state(quantized_array.copy(), cut)

# final state initial state
print("Initial State:")
for row in quantized_array:
    print(','.join(map(str, row)))

print("\nFinal State:")
for row in final_state:
    print(','.join(map(str, row)))

# Show image
fig, axs = plt.subplots(1, 2, figsize=(12, 6))
axs[0].imshow(quantized_array, cmap='gray')
axs[0].set_title('Initial State')
axs[0].axis('off')

axs[1].imshow(final_state, cmap='gray')
axs[1].set_title('Final State')
axs[1].axis('off')

plt.show()
