var canvas, context;
var prevX = 0,
	prevY = 0,
	prevRot = 90,
	currentX = 0,
	currentY = 0,
	currentRot = 0; //relative to y-axis

function init() {
    canvas = document.getElementById('image');
    context = canvas.getContext('2d');
    canvas.addEventListener('mousedown', mouseclick, false);
	$('#movebutton').attr("checked", true);
}
function mouseclick(e){
	//set new previous mouse click
	prevX = currentX
	prevY = currentY
	//set new current mouse click
	currentX = e.clientX - canvas.offsetLeft;
    currentY = e.clientY - canvas.offsetTop;
    drawAndSendCmd(currentX,currentY);
}

function drawAndSendCmd(x, y){
	//places the point where the mouse was clicked
	context.fillRect(x,y,5,5);
	if (prevX == 0 && prevY == 0) {
		return
	}
	else {
		context.beginPath();
     	context.moveTo(prevX, prevY);
     	context.lineTo(currentX, currentY);
     	if($('#drawbutton').is(":checked")){
     		context.strokeStyle = '#458B00';
     	}
     	else{
     		context.strokeStyle = '#FFA500';
     	}

     	context.stroke();
     	//Set current rotational angle the bot is facing
     	var theta = Math.atan2(currentY - prevY, currentX - prevX);
		theta *= 180/Math.PI;
		prevRot = currentRot;
		currentRot = theta;
		var angleDiff = currentRot - prevRot;
		//Contraint the rotation in between -180 to 180
		if(-180 > angleDiff){
			angleDiff = 360 + angleDiff;
		}
		else if (angleDiff > 180) {
			angleDiff = angleDiff - 360;
		}
		angleDiff = Math.round(angleDiff);
		var msg = "Mouse Position: " + currentX + ", " + currentY + " Angle Facing : " + currentRot + " Diff: " + angleDiff;
		//applying the scale to the line distance
		var scale = $('#scale').val();
		var cmDist = getLineLen(prevX,prevY,currentX,currentY) / scale;
		cmDist = Math.round(cmDist*100)/100;
		updateStatusBar(createCmdString(angleDiff,cmDist));
		//postCmd(angleDiff,cmDist)

	}
}

function updateStatusBar(message) {
        var statusbar = canvas.getContext('2d');
        statusbar.clearRect(0, 0, canvas.width, 30);
        statusbar.font = '14pt Calibri';
        statusbar.fillStyle = 'black';
        statusbar.fillText(message, 10, 25);
}

function createCmdString(degree, distance){
	var cmdString = "move (" + degree + ", " + distance + ")\n";
	if($('#drawbutton').is(":checked")){
		cmdString = "pen down\n " + cmdString + " pen up\n";
	}
	return cmdString;
}

function postCmd(post_data){
	jQuery.ajax( {
	    url: '/cmd',
	    type: 'POST',
	    data: post_data,
	    contentType: 'text/plain'
	} );
}

function getLineLen(x1,y1,x2,y2){
	return Math.sqrt((Math.pow(x1-x2,2)) + (Math.pow(y1-y2,2)));
}

$('#drawbutton').click(function(){
    var movebutton = $('#movebutton');
    if($(this).is(":checked")){
        movebutton.attr("checked", false);
    }
});

$('#movebutton').click(function(){
    var drawbutton = $('#drawAndSendCmdbutton');
    if($(this).is(":checked")){
        drawbutton.attr("checked", false);
    }
});

window.onload = init();
