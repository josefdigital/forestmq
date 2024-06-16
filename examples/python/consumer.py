from flask import Flask
import requests


app = Flask(__name__)


@app.route('/consumer')
def consumer():  # put application's code here
    data = {
        "message": "provider test!",
    }
    r = requests.post("http://localhost:8005/consumer", json=data)
    print(f"Request status: {r.status_code}")
    try:
        return {
            "message": f"{r.json()}",
        }
    except:
        return {
            "queue_status": "empty",
        }


if __name__ == '__main__':
    app.run()
