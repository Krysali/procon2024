import numpy as np
import torch
from random import randint

def encode_state(state):
        n, m = state.shape
        one = np.zeros((5, 5))
        two = np.zeros((5, 5))
        three = np.zeros((5, 5))
        four = np.zeros((5, 5))

        # daraa harii
        mp = {
            1 : one,
            2 : two,
            3 : three,
            4 : four
        }

        for r in range(n):
            for c in range(m):
                x = int(state[r][c])
                mp[x][r][c] = x

        # daraa harii bur chuhal
        res = np.stack((one, two, three, four))
        res = np.expand_dims(res, axis=0)
        res = torch.tensor(res, dtype=torch.float16)
        # N C W H --> N W H C
        # res = res.permute(0, 2, 3, 1)

        return res

A = np.arange(1, 17).reshape((4, 4))

for i in range(4):
     for j in range(4):
          A[i][j] = randint(1, 4)

X = encode_state(A)