import json
from game_types import *

def parse_problem(problem_json):
    """Parses the JSON data of a problem and returns a dictionary containing the game state.

    Args:
        problem_json (str): The raw JSON string representing the problem. 

    Returns:
        dict: A dictionary containing the parsed game state information, including:
            - board: A dictionary with "width", "height", "start" (initial state), and "goal" (goal state) keys. 
            - general: A list of dictionaries, each representing a general stencil with "p" (stencil number), "width", "height", and "cells" keys. 
    """

    # Parse the JSON string into dictionary
    json_parse_data = json.loads(problem_json)

    # Parse the board data
    board_data = json_parse_data["board"]

    # Create a Board object
    board_pieces = []
    for row in board_data["start"]:
        board_row = []
        for piece in row:
            board_row.append(piece)
        board_pieces.append(board_row)
    board = Board(board_data["width"], board_data["height"], board_pieces)

    # Parse the goal data
    goal_state = []
    for row in board_data["goal"]:
        goal_row = []
        for piece in row:
            goal_row.append(piece)
        goal_state.append(goal_row)

    # Parse the stencil data
    general_stencil_data = json_parse_data["general"]
    
    # loop through the general stencil data and create a list of Stencil objects
    general_stencils = []
    for stencil in general_stencil_data["patterns"]:
        stencil_cells = []
        for row in stencil["cells"]:
            stencil_row = []
            for cell in row:
                stencil_row.append(cell)
            stencil_cells.append(stencil_row)
        general_stencils.append(Stencil(stencil["width"], stencil["height"], stencil_cells))
    return GameState(board, general_stencils, goal_state, 0)
