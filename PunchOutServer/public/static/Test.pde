var x = window.innerWidth / 2;
var y = window.innerHeight / 2;

float r,g,b,dx,dy;
var opponent = {};

var debug = false;

var client = new SocketClient({
    port:8000,
    callbacks:{
        onmessage:function(msg){
            msg = JSON.parse(msg.data);

            if(msg.hasOwnProperty("userid")){
                localStorage.setItem("userid",msg.userid);
            }


            if(msg.event === "opponentupdate"){
                var clientid = parseInt(localStorage.getItem("userid"));
                console.log("Client is :" + clientid + " Msg is :" + msg.id);
               if(msg.id === clientid){
                   console.log(msg);
                   /*dx = mouseX - msg.data.currentHeadPosition.x;
                    dy = mouseY - msg.data.currentHeadPosition.y;

                    x += dx * 0.05;
                    y += dy * 0.05;*/

                  if(!debug){
                      x = msg.data.currentHeadPosition.x;
                      y = msg.data.currentHeadPosition.y;

                  }

               }else{
                   console.log("nomatch");
               }
            }

        }
    }
});
client.init();



void setup(){
    size(window.innerWidth,window.innerHeight);
    background(200);

    r = random(0,255);
    g = random(0,255);
    b = random(0,255);
}


void draw(){
    background(200);

    if(debug){
        dx = mouseX - x;
        dy = mouseY - y;


        x += dx * 0.05;
        y += dy * 0.05;
    }
/*
 dx = mouseX - x;
 dy = mouseY - y;


 x += dx * 0.05;
 y += dy * 0.05;
*/

 fill(100);
    if(debug){
        text("Debug mode is on",20,20);
    }else{
        text("Debug Mode is off",20,20);
    }
    text("X:"+Math.floor(x)+"\nY:"+Math.floor(y),x + 30,y - 30);

    fill(r,g,b);
    ellipse(x,y,20,20);
}


window.addEventListener("keydown",function(e){
    if(e.keyCode === 77){
       if(debug){
           debug = false;
       }else if(!debug){
           debug = true;
       }
    }
})



window.addEventListener("mousemove",function(e){

    var string = {
        id:localStorage.getItem("userid"),
        event:{
            type:"headupdate",
            value:{
                x:Math.floor(x),
                y:Math.floor(y)
            }
        }
    }

    client.socket.send(JSON.stringify(string));
})
