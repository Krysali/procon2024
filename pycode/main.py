from json_parsing import *
from http_request import *

def main():
    url = "localhost"
    parse_problem(get_problem(url))
