from flask import Flask

app = Flask(__name__)


@app.route("/")
def hello():
    msg = """
    <html><head><title>Cheezoid</title></head>
    <body><h3>cheezoid httpd controller</h3>
    RESTful endpoints
    <ul>
    <li>/status to view cheezoid status</li>
    <li>/cmd to send command</li>
        <ul>
        <li>mov (relative angle in degree, relative distance in cm)</li>
        <li>pen (up/down) </li>
        <li>reset</li>
        </ul>
    <li>/svg to send svg</li>
    <li>/canvas to GUI</li>
    </ul>
    </body>
    </html>
"""
    return msg

if __name__ == "__main__":
    app.run()