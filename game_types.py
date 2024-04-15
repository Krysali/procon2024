class Stencil:
    def __init__(self, width, height, cells):
        self.width = width 
        self.height = height
        self.cells = cells  # List of lists representing the stencil pattern (0/1)

class Board:
    def __init__(self, width, height, pieces):
        self.width = width
        self.height = height
        self.pieces = pieces  # List of lists representing the piece values

class GameState:
    def __init__(self, board, stencils, goal, num_moves):
        self.board = board # Board object
        self.stencils = stencils  # List of Stencil objects 
        self.goal_state = goal  # Board object
        self.num_moves = num_moves