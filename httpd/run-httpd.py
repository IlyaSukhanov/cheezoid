"""
Copyright 2015 Ilya Sukhanov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

from flask import Flask, request
from command import cmd_repl, create_fifo, PIPENAME
import socket
import sys
from svg import from_svg_to_cmds

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
    curl -X POST --data 'set' %s/cmd
    </code>
    </li>
    <li>
    Move cheezoid back to origin<br>
    <code>
    curl -X POST --data 'reset' %s/cmd
    </code>
    </li>
    <li>
    Move cheezoid back to origin then draw a line<br>
    <code>
    curl -X POST --data 'reset\\n pen down\\n move (13, 97)\\n pen up\\n' %s/cmd
    </code>
    </li>
    </ul>
    <br>
    Below are RESTful endpoints, POST to send, GET to receive<br>
    <ul>
    <li><a href="/status">/status<a> to view cheezoid status</li>
    <li><a href="/cmd">/cmd</a> to send command</li>
        <ul>
        <li>set: make current position origin. and orient cheezoid correctly</li>
        <li>move (relative angle in degree, relative distance in cm): orient cheezoid at angel of param 1 degrees to vertical Y
        then move distance in param 2 in cm </li>
        <li>pen [up/down]: put pen down or pen up </li>
        <li>reset: move cheezoid back to origin and oriented </li>
        </ul>
    <li><a href="/svg">/svg</a> to send svg</li>
    <li><a href="/canvas">/canvas</a> to GUI</li>
    </ul>
    </body>
    </html>
"""
    return msg % (hostname, hostname, hostname)

@app.route("/cmd", methods=['POST', 'GET'])
def cmd_process():
    global fifo
    if request.method == 'GET':
        with open("static/cmd.html") as f:
            return f.read()
    elif request.method == 'POST':
        body = request.data or request.form.get("commands", request.form.keys()[0])
        cmds = cmd_repl(fifo, body)
        return "%s\n" % ("\n".join(cmds))

@app.route("/status")
def status_process():
    return "I'm alive!"

@app.route("/svg", methods=['GET', 'POST'])
def svg_process():
    hostname = socket.gethostname()
    if request.method == 'GET':
        return """
        <code>
        curl -X POST --data [svg file] -H 'Content-Type: text/xml' %s/cmd
        </code>
        """ % hostname
    elif request.method == 'POST':
        body = request.data
        cmds = from_svg_to_cmds(body)
        return "%s\n" % ("\n".join(cmds))
    else:
        return "invalid method"

@app.route("/canvas")
def canvas_process():
    with open("static/cheezoid_canvas.html") as f:
        return f.read()

if __name__ == "__main__":
    global fifo
    fifo = create_fifo(PIPENAME)
    app.run(host="0.0.0.0", port=80)
    fifo.close()
