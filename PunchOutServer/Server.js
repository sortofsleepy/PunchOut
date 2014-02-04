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
   // if(connections.length < 2){


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


    //}


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

        var data = JSON.parse(msg);
        var index = 0;

        /**
         * Match id w/ the list of registered connections
         */
        var user_exists = false;
        var numconnections = connections.length;
        for(var i = 0;i<numconnections;++i){
            if(connections[i].id === parseInt(data.id)){
                user_exists = true;
                index = i;
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
                    connections.splice(index,1);
                    notifyUpdate();
                    break;
               /**
                * When we get new information about the hands.
                */
                case Events.NEW_HAND_POS:
                    connections[index].currentHandPosition = data.event.value;
                    notifyUpdate();
                    break;
               /**
                * When we get new information about the head
                */
                case Events.NEW_HEAD_POS:

                    connections[index].currentHeadPosition = data.event.value;
                    notifyUpdate();
                    break;
            }

        }else{
            console.log("user does not exist")
        }


        /**
         * This will broadcast to all players that a update has taken place.
         * The id of the opposite player will be sent so that the current player
         * can update their display.
         */
        function notifyUpdate(){
            var update = {}
            //get the opponent's id
            for(var i = 0;i<connections.length;++i){
                /**
                 *  Look for the player opposite that of the one
                 *  sending the ID.
                 */

                if(i !== index){
                    update["id"] = i;
                    update["event"] = "opponentupdate";
                    update["data"] = connections[index];
                }
            }
            console.log("Notifying opponent of " + JSON.stringify(update));
            ws.send(JSON.stringify(update));
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