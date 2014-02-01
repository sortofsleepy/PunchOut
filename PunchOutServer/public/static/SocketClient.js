/**
 * A basic client side websocket wrapper
 * @param _params Parameters. Whats needed :
 * 1. Port - port to run websockets on
 * 2. Address - for when you run on live site
 * 3. callbacks - callbacks for all the WebSocket functionality. These include
 *      a. onopen
 *      b. ondisconnect
 *      c. onmessage
 *      d. onerror
 * @constructor
 */
var SocketClient = function(_params){
    //set port.
    this.port = _params.port !== undefined ? _params.port : 9000;

    //set address. Note Testing address for now
    this.address = _params.address !== undefined? _params.address : "ws://192.168.1.5:8000"

    //set callbacks
    this.callbacks = _params.callbacks !== undefined ?  _params.callbacks :{}

    //set protocols
    this.protocols = _params.protocols != undefined ? _params.protocols :[]
};

SocketClient.prototype = {

    /**
     * Inits the Websocket connection.
     *
     * Sets callbacks up to be called at the appropriate event.
     * Will write to console if event isn't specified.
     */

    init:function(){
        this.socket = new WebSocket(this.address,this.protocols);
        var parent = this;
        console.log(parent);
        /**
         * Handle the open callback
         */
        this.socket.onopen = function(){
            if(parent.callbacks["onopen"] !== undefined){
                parent.callbacks["onopen"]();
            }else{
                console.log("Connection opened to socket at " + parent.address);
            }
        };

        /**
         * Handle Error callback
          * @param e the error object from the event
         */
        this.socket.onerror = function(e){
            if(parent.callbacks["onerror"] !== undefined){
                parent.callbacks["onerror"](e);
            }else{
                console.log("A Websocket error occured : " + e.data);
            }
        }


        /**
         * Handle the message event
         * @param msg the message that is received from the server
         */
        this.socket.onmessage = function(msg){

            if(parent.callbacks["onmessage"] !== undefined){
                parent.callbacks["onmessage"](msg);
            }else{
                //alert("message received");
                console.log("Message received : " + msg.data);

            }
        }

        /**
         * Handle the close event
         * @param msg
         */
        this.socket.onclose = function(msg){
            if(parent.callbacks["onclose"] !== undefined){
                parent.callbacks["onclose"](msg);
            }else{
                console.log("Closing WebSocket connection");
            }
        }
    },

    /**
     * returns the Websocket object
     * @returns {WebSocket|*|SocketClient.socket}
     */
    getSocket:function(){
        return this.socket;
    }


};//end SocketClient prototype