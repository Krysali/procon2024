import requests

def get_problem(url):
    """Sends a GET request to the server to retrieve the problem data for the given problem ID.

    Returns:
        str: The raw JSON string representing the problem data.
    """
    # Send a GET request to the server to retrieve the problem data
    response = requests.get(url)
    return response.json
