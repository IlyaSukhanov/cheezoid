from flask import Flask, request
from command import cmd_repl, create_fifo, PIPENAME
import socket
import sys

app = Flask(__name__)
app.config['DEBUG'] = True


@app.route("/")
def hello():
    hostname = socket.gethostname()
    msg = """
    <html><head><title>Cheezoid</title></head>
    <body><h3>cheezoid httpd controller</h3>

    For example:<br>
    <ul>
    <li>
    Make current location the origin<br>
    <code>
    curl -X POST --data 'set' %s:5000/cmd
    </code>
    </li>
    <li>
    Move cheezoid back to origin<br>
    <code>
    curl -X POST --data 'reset' %s:5000/cmd
    </code>
    </li>
    <li>
    Move cheezoid back to origin then draw a line<br>
    <code>
    curl -X POST --data 'reset\\n pen down\\n move (13, 97)\\n pen up\\n' %s:5000/cmd
    </code>
    </li>
    </ul>
    <br>
    Below are RESTful endpoints, POST to send, GET to receive<br>
    <ul>
    <li>/status to view cheezoid status</li>
    <li>/cmd to send command</li>
        <ul>
        <li>set: make current position origin. and orient cheezoid correctly</li>
        <li>move (relative angle in degree, relative distance in cm): orient cheezoid at angel of param 1 degrees to vertical Y
        then move distance in param 2 in cm </li>
        <li>pen [up/down]: put pen down or pen up </li>
        <li>reset: move cheezoid back to origin and oriented </li>
        </ul>
    <li>/svg to send svg</li>
    <li>/canvas to GUI</li>
    </ul>
    </body>
    </html>
"""
    return msg % (hostname, hostname, hostname)

@app.route("/cmd", methods=['POST', 'GET'])
def cmd_process():
    global fifo
    if request.method == 'GET':
        return "Please use POST\n"
    elif request.method == 'POST':
        body = request.data or request.form.keys()[0]
        cmds = cmd_repl(fifo, body)
        return "%s\n" % ("\n".join(cmds))

@app.route("/status")
def status_process():
    return "status\n"

@app.route("/svg")
def svg_process():
    return "svg\n"

@app.route("/canvas")
def canvas_process():
    return "canvas"

if __name__ == "__main__":
    global fifo
    fifo = create_fifo(PIPENAME)
    app.run(host="0.0.0.0")
    fifo.close()
