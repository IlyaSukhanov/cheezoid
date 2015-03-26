from flask import Flask

app = Flask(__name__)


@app.route("/")
def hello():
    msg = """
    <html><head><title>Cheezoid</title></head>
    <body><h3>cheezoid httpd controller</h3>
    <ul>
    <li>/status to view cheezoid status</li>
    <li>/cmd to send command</li>
    <li>/svg to send svg</li>
    </ul>
    </body>
    </html>
"""
    return msg

if __name__ == "__main__":
    app.run()