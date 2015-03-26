from flask import Flask, request
from cmd import cmd_repl, create_fifo, PIPENAME

app = Flask(__name__)
app.config['DEBUG'] = True

fifo = create_fifo(PIPENAME)

@app.route("/")
def hello():
    msg = """
    <html><head><title>Cheezoid</title></head>
    <body><h3>cheezoid httpd controller</h3>
    RESTful endpoints, POST to send, GET to receive<br>
    For example:<br>
    <code>
    curl -X POST --data 'reset' -H 'Content-type: text/plain' localhost:5000/cmd
    </code>
    or <br>
    <code>
    curl -X POST --data 'reset\n pen down\n move (13, 97)\n pen up' -H 'Content-type: text/plain' localhost:5000/cmd
    </code>
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

@app.route("/cmd", methods=['POST', 'GET'])
def cmd_process():
    global fifo
    if request.method == 'GET':
        return "cmd\n"
    elif request.method == 'POST':
        cmd_repl(fifo, request.data)
        return "%s\n" % (request.data)

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
    app.run(host="0.0.0.0")