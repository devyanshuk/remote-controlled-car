var button_being_pressed = false;

document.addEventListener("keydown", (e) => {
    var success = true;

    if(button_being_pressed) {
        return;
    }

    if (e.which == 37) {
        onLeftMouseDown();
    } 
    else if (e.which == 38) {
        onUpMouseDown();
    } 
    else if (e.which == 39) {
        onRightMouseDown();
    } 
    else if (e.which == 40) {
        onDownMouseDown();
    }
    else {
        success = false;
    }
    if (success) {
        button_being_pressed = true;
    }
});

document.addEventListener("keyup", (e) => {
    var success = true;
    if (e.which == 37) {
        onLeftMouseUp();
    }
    else if (e.which==38) {
        onUpMouseUp();
    }
    else if (e.which == 39) {
        onRightMouseUp();
    }
    else if (e.which == 40) {
        onDownMouseUp();
    }
    else{
        success = false;
    }
    if (success) {
        button_being_pressed = false;
    }
});


 function onLeftMouseDown() {
    console.log("left button pressed");
    document.querySelector(".names .lefttext").innerHTML = "Counterclockwise";
    document.querySelector(".keys .left").classList.add("left_pressed");
     fetch("Counterclockwise",
                {
                    method: "POST"
                });
};

 function onLeftMouseUp() {
    console.log("left button released");
    document.querySelector(".names .lefttext").innerHTML = "";
    document.querySelector(".keys .left").classList.remove("left_pressed");
     fetch("Stop",
                {
                    method: "POST"
                });
};

 function onRightMouseDown() {
    console.log("right button pressed");
    document.querySelector(".names .righttext").innerHTML = "Clockwise";
    document.querySelector(".keys .right").classList.add("right_pressed");
     fetch("Clockwise",
                {
                    method: "POST"
                });
};

 function onRightMouseUp() {
    console.log("right button released");
    document.querySelector(".names .righttext").innerHTML = "";
    document.querySelector(".keys .right").classList.remove("right_pressed");
     fetch("Stop",
                {
                    method: "POST"
                });
};

 function onUpMouseDown() {
    console.log("up button pressed");
    document.querySelector(".names .uptext").innerHTML = "Forward";
    document.querySelector(".keys .up").classList.add("up_pressed");
     fetch("Forward",
                {
                    method: "POST"
                });
};

 function onUpMouseUp() {
    console.log("up button released");
    document.querySelector(".names .uptext").innerHTML = "";
    document.querySelector(".keys .up").classList.remove("up_pressed");
     fetch("Stop",
                {
                    method: "POST"
                });
};

function onDownMouseDown() {
    console.log("down button pressed");
    document.querySelector(".names .downtext").innerHTML = "Backward";
    document.querySelector(".keys .down").classList.add("down_pressed");
    fetch("Backward",
                {
                    method: "POST"
                });
};

 function onDownMouseUp() {
    console.log("down button released");
    document.querySelector(".names .downtext").innerHTML = "";
    document.querySelector(".keys .down").classList.remove("down_pressed");
     fetch("Stop",
                {
                    method: "POST"
                });
};


function send_pwm() {
    var pwm_value = document.getElementById("pwm").value;
    console.log("sending pwm value:" + pwm_value);
    fetch("PWM/" + pwm_value,
            {
                method: "POST"
            });
}

function send_servo_val() {
    var servo_value = document.getElementById("servo").value;
    document.querySelector(".names .SERVO").innerHTML = "Servo (" + servo_value + ")&#176;";
    console.log("sending servo value:" + servo_value);
    fetch("Servo/" + servo_value,
            {
                method: "POST"
            });
}

function auto_drive() {
    fetch("Autodrive/" + (document.getElementById("togBtn").checked).toString(),
            {
                method: "POST"
            });
}

function wall_follow() {
    fetch("WallFollow/" + (document.getElementById("togBtn3").checked).toString(),
            {
                method: "POST"
            });
}


//'{"servo":"servo_val", "pwm":"pwm_val", "auto_drive":"bool", "wall_follow":"bool", "distance":"dist_val"}'
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
        console.log(this.responseText);
      var obj = JSON.parse(this.responseText);
      document.getElementById("togBtn").checked = JSON.parse(obj.auto_drive);
      document.getElementById("togBtn3").checked = JSON.parse(obj.wall_follow);
      document.querySelector(".names .distance").innerHTML = "Obstacle distance: " + obj.distance + " cm";
    }
  };
  xhttp.open("GET", "/Updates", true);
  xhttp.send();
}, 1000 ) ;