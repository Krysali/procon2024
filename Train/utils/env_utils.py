import re
import math
from environments.environment_abstract import Environment


def get_environment(env_name: str) -> Environment:
    env_name = env_name.lower()
    puzzle_n_regex = re.search("puzzle(\d+)", env_name)
    env: Environment

    if env_name == 'Game':
        from environments.Game import GmState
        env = GmState()
    else:
        raise ValueError('No known environment %s' % env_name)

    return env
