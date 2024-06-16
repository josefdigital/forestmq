from flask import Flask
import requests


app = Flask(__name__)


@app.route('/')
def hello_world():  # put application's code here
    data = {
        "message": "provider test!",
    }
    r = requests.post("http://localhost:8005/provider", json=data)
    print(f"Request status: {r.status_code}")

    return {
        "data": f"request body: {r.json()}",
    }


if __name__ == '__main__':
    app.run()
