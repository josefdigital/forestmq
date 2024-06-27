from flask import Flask
import requests


app = Flask(__name__)


@app.route('/provider/<count>')
def provider(count: int):  # put application's code here
    data = {
        "message": f"test message {count} ",
        # "destroy": True,
    }
    r = requests.post("http://0.0.0.0:8005/provider", json=data)
    print(f"Request status: {r.status_code}")

    return {
        "data": f"request body: {r.json()}",
    }


if __name__ == '__main__':
    app.run(port=5001)
