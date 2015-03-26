from flask import Flask
from cmd import cmd_repl, create_fifo, PIPENAME

app = Flask(__name__)
app.config['DEBUG'] = True

fifo = create_fifo(PIPENAME)

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
        <li>move (relative angle in degree, relative distance in cm)</li>
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

@app.route("/cmd")
def cmd_process():
    global fifo
    cmd_repl(fifo)
    return "here"

@app.route("/status")
def status_process():
    return "status"

@app.route("/svg")
def svg_process():
    return "svg"

@app.route("/canvas")
def canvas_process():
    return "canvas"

if __name__ == "__main__":
    app.run()