/**
 * Corrections and ammendments are welcome.
 * @type {exports}
 */
var flatiron = require("flatiron");
var fs = require("fs");
var connect = require("connect").static;
var WebSocketServer = require("ws").Server;

var app = flatiron.app;

var PORT = 5000 || proces.env.PORT;

//use functions
app.use(flatiron.plugins.http,{});

//setup static file directories.
app.http.before.push(connect(__dirname + "/public/static"));
app.http.before.push(connect(__dirname + "/public/components"));
app.http.before.push(connect(__dirname + "/public/templates"));

app.http.before.push(function(req,res){
    var found = app.router.dispatch(req,res);
    if(!found){
        res.emit("next");
    }
});

var wss = new WebSocketServer({
    port:8000
})

//number of connections
var connections = [];

//possible message types
var Events = {
    DELETE_MSG:"delete",
    NEW_HAND_POS:"handupdate",
    NEW_HEAD_POS:"headupdate"
}

wss.on("connection",function(ws){

    /**
     * Just two player for now.
     */
    if(connections.length < 2){


       //make a user object
       var user = {
           id:Math.floor(Math.random() * 9999),
           currentHeadPosition:{},
           currentHandPosition:{
               left:{},
               right:{}
           }
       }

       //add to list of users
        connections.push(user);



        ws.send(JSON.stringify({"userid":user.id}));


    }


    /**
     * 1/30/2014
     * Initial Sever handling plan.
     *
     * When the server receives a message, it should be
     * JSON formatted and contain:
     * 1. the id of the sender, which should have been stored on the client during initialization.
     * 2. the name of the event this message is regarding under a key called "event"
     *      - if the event has no value ( like the delete user event ), then the value should just be false
     *      - otherwise the value should contain a value for that event. For positional data, that should be JSON
     *      formatted as well.
     *      Examples :
     *      {
     *          left:{
     *              x:03957039457034,
     *              y:-49358043985-,
     *          },
     *          right:{
     *              x:094358703495703947850,
     *              y:-49386-34986034670349
     *          }
     *
     *      }
     *
     *      // example of full string to send from client(minus json stringifying)
     *      var string = {
                id:id,
                event:{
                    type:"delete",
                    value:false
                }
            }
     *
     */
    ws.on("message",function(msg){
        console.log("connection");
        var data = JSON.parse(msg);
        console.log(msg);
        /**
         * Match id w/ the list of registered connections
         */
        var user_exists = false;
        var numconnections = connections.length;
        for(var i = 0;i<numconnections;++i){
            if(connections[i].id === data.id){
                user_exists = true;
            }
        }

        if(user_exists){
            /**
             * Figure out what kind of data we're getting
             */
            switch(data.event.type){

               /**
                *  When we want to tell the server to delete
                *  a user.
                *
                *  TODO figure out if theres a more kosher way of
                *  deleting a user.
                */
                case Events.DELETE_MSG:
                    //de-register user from list of current connections
                    for(var i = 0;i<numconnections;++i){
                        if(connections[i].id === data.event.value){
                            connections.splice(i,1);
                        }
                    }
                    break;
               /**
                * When we get new information about the hands.
                */
                case Events.NEW_HAND_POS:
                    for(var i = 0;i<numconnections;++i){
                        if(connections[i].id === data.event.value){
                            connections[i].currentHandPosition = JSON.parse(data.handpos);
                        }
                    }
                    break;
               /**
                * When we get new information about the head
                */
                case Events.NEW_HEAD_POS:
                    for(var i = 0;i<numconnections;++i){
                        if(connections[i].id === data.event.value){
                            connections[i].currentHeadPosition = JSON.parse(data.headpos);
                        }
                    }
                    break;
            }

        }
    });






});


//start app
app.start(PORT);


app.router.get("/",function(){
    var res = this.res;
    var req = this.req;
    res.writeHead

    fs.readFile("index.html",function(err,data){
       res.end(data);
    });
})