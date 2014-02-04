var opponent = {};

var client = new SocketClient({
    port:8000,
    callbacks:{
        onmessage:function(msg){
            msg = JSON.parse(msg.data);

            if(msg.hasOwnProperty("userid")){
                localStorage.setItem("userid",msg.userid);
            }

            if(msg.hasOwnProperty("event")){
                if(msg.event === "opponentupdate"){
                    console.log(msg);
                }
            }
        }
    }
});
client.init();



var x = window.innerWidth / 2;
var y = window.innerHeight / 2;

float r,g,b;
void setup(){
    size(window.innerWidth,window.innerHeight);
    background(200);

    r = random(0,255);
    g = random(0,255);
    b = random(0,255);
}


void draw(){
    background(200);


    float dx = mouseX - x;
    float dy = mouseY - y;


    x += dx * 0.05;
    y += dy * 0.05;


    fill(100);
    text("X:"+Math.floor(x)+"\nY:"+Math.floor(y),x + 30,y - 30);

    fill(r,g,b);
    ellipse(x,y,20,20);
}


window.addEventListener("keydown",function(e){
    if(e.keyCode === 77){
        client.socket.send(JSON.stringify({

        }))
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
