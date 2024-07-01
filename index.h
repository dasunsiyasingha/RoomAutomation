/*
 * This ESP8266 NodeMCU code was developed by newbiely.com
 *
 * This ESP8266 NodeMCU code is made available for public use without any restriction
 *
 * For comprehensive instructions and wiring diagrams, please visit:
 * https://newbiely.com/tutorials/esp8266/esp8266-websocket
 */

const char *HTML_CONTENT = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
    <title>Document</title>
    <meta name="viewport" content="width=device-width, initial-scale=0.7">
    <style>
    body {
        font-size: 22px;
        background-color: black;
        color: aliceblue;
        margin: auto;
        width: 80%;
        padding: 10px;
    }

    .center {
        margin: auto;
        width: 60%;
        padding: 10px;
    }
    .column {
        width: 95%;
        padding: 10px;
        height: 30px; /* Should be removed. Only for demonstration */
    }

    /* Clear floats after the columns */
    .row:after {
        content: "";
        display: table;
        clear: both;
    }
    .main-container {
        width: 400px;
        height: 450px;
        overflow-y: auto;
        padding: 5px;
        margin-bottom: 5px;
    }

    .container {
        width: 400px;
        height: 450px;
        /* overflow-y: auto; */
        padding: 5px;
        margin-bottom: 5px;
    }

    .user-input {
        display: flex;
        margin-bottom: 20px;
    }

    .websocketContainer {
        /* overflow-y: auto; */
        align-items: center;
        margin-bottom: 5px;
    }

    .websocket .label {
        /* margin-left: auto; */
    }

    button{
        padding: 4px;
        padding-left: 8px;
        padding-right: 8px;
        border-radius: 14px;
        border-color: blueviolet;
        font-weight: 40px;
        font-weight: bold;
    }

    

    .column button:hover{
        background-color: rgb(166, 165, 165);
        color: black;
    }

    .btn{
        color: blueviolet; 
        font-weight: bold; 
        font-size: large; 
        background-color: aliceblue; 
        padding: 6px 20px 6px 20px; 
        margin: 0px 0px 10px 0px; 
        border-radius: 14px;
    }

    .btn-input{
        color: black; 
        font-weight: bold; 
        font-size: large; 
        background-color: rgb(253, 253, 253); 
        padding: 5px 14px 5px 14px; 
        margin: 0px 0px 0px 0px; 
        border-radius: 14px;
        border-color: blueviolet;
    }
    .grid-container {
        display: grid;
        width: fit-content;
        margin: auto;
        align-content: center;
        grid-template-columns: auto auto;
        gap: 10px;
        padding: 10px;
    }

    .grid-container > div {
        background-color: rgba(54, 53, 53, 0.8);
        text-align: center;
        padding: 20px;
        font-size: 20px;
    }

    .grid-container1 {
        display: grid;
        margin: auto;
        width: fit-content;
        height: 400px;
        align-content: start;
        grid-template-columns: auto;
        gap: 10px;
        padding: 10px;
    }

    .grid-container1 > div {
        background-color: rgba(54, 53, 53, 0.8);
        width: fit-content;
        text-align: center;
        padding: 20px;
        font-size: 20px;
    }

    .grid-container-rgb {
        display: grid;
        width: fit-content;
        margin: 10px;
        align-content: start;
        grid-template-columns: auto auto auto;
        gap: 0px;
        padding: 2px;
    }

    .grid-container-rgb > div {
        text-align: center;
        padding: 20px;
        font-size: 18px;
    }

    .state{
        width: 20vw;
        height: 10vw;
        display: flex;
        justify-content: center;
        align-items: center;
        border: solid blueviolet;
        border-radius: 14px;
        margin-top: 10px;
        background-color:rgb(73, 73, 72);
        color: black;
        font-weight: bold;
    }

    .onstate{
        background-color: rgb(252, 248, 4);
        color: black;
    }

    .offstate{
        background-color:rgb(73, 73, 72);
        color: black;
    }

    .ledcolor{
        width: 10px;
        height: 10px;
        display: flex;
        justify-content: center;
        align-items: center;
        border: solid blueviolet;
        border-radius: 14px;
        background-color:rgb(0, 0, 0);
        color: rgb(255, 255, 255);
        font-weight: bold;
    }

.grid-container-rgb > .center> input:in-range{
    -webkit-appearance: none;
    appearance: none;
    width: 18vw;
    height: 10px;
    outline: none;
    opacity: 0.8;
    transition: opacity 0.4s;
    border-radius: 15px;
    margin: auto;
    margin-right: 20px;
}

#redrange { 
    background: linear-gradient(to right,
        rgb(0, 0, 0),  rgb(255, 0, 0));
    
} 
  
#greenrange { 
    background: linear-gradient(to right,
        rgb(0, 0, 0),  rgb(0, 255, 0));
} 
  
#bluerange { 
    background: linear-gradient(to right,
        rgb(0, 0, 0),  blue);
} 
    </style>
    <script>
        var ws;
        var wsm_max_len = 4096; /* bigger length causes uart0 buffer overflow with low speed smart device */

        function connect_server() {
            if(ws == null) {
                ws = new WebSocket("ws://" + window.location.host + ":81");
                document.getElementById("ws_state").innerHTML = "CONNECTING";
                ws.onopen = ws_onopen;
                ws.onclose = ws_onclose;
                ws.onmessage = ws_onmessage;
            } else
                ws.close();
        }

        
        function ws_onopen() {
            document.getElementById("ws_state").innerHTML = "CONNECTED";
            document.getElementById("ws_state").style.fontWeight="bold";

            document.getElementById("bt_connect").innerHTML = "Disconnect";
            document.getElementById("bt_connect").style.backgroundColor="blueviolet";
            document.getElementById("bt_connect").style.color="white";
        }

        function ws_onclose() {
            document.getElementById("ws_state").innerHTML = "CLOSED";
            document.getElementById("ws_state").style.fontWeight="regular";

            document.getElementById("bt_connect").innerHTML = "Connect";
            document.getElementById("bt_connect").style.backgroundColor="aliceblue";
            document.getElementById("bt_connect").style.Color="blueviolet";

            ws.onopen = null;
            ws.onclose = null;
            ws.onmessage = null;
            ws = null;
        }

        // SEND DATA AS A jsonArray to server(esp8266 nodemcu board)
        function senddata(itemnum, stvalue){
            // var array = [itemnum, stvalue];
            var message = {
                itemType: "switches",
                valuesArray: [itemnum, stvalue]
            };
            var jsonMessage = JSON.stringify(message);
            ws.send(jsonMessage);
        }

        function sendRGBdata(colorArray){
            var colrMessage = {
                itemType: "rgb",
                valuesArray: colorArray
            };
            var jsoncolrMessage = JSON.stringify(colrMessage);
            ws.send(jsoncolrMessage);
        }

        function onofffunc(num){
            let st = "item"+num+"state";
            let btn = "item"+num+"btn";
            let itemnum = "item"+num;

            let state = document.getElementById(st);
            let itembtn = document.getElementById(btn);

            if (itembtn.value=="on"){
                itembtn.innerHTML="turn off";
                senddata(itemnum, itembtn.value);
                itembtn.value="off";

                // state.classList.remove("offstate");
                // state.classList.add("onstate");
                // state.innerHTML="ON";
                
                console.log(itembtn.value);

            }else{

                itembtn.innerHTML="turn on";
                senddata(itemnum, itembtn.value);
                itembtn.value="on";

                // state.classList.remove("onstate");
                // state.classList.add("offstate");
                // state.innerHTML="OFF";
                
                console.log(itembtn.value);
            }
        }

//DATA RECEIVED FROM SERVER
        function ws_onmessage(e_msg) {
            e_msg = e_msg || window.event; // MessageEvent
            console.log(e_msg.data);
            
            var msg = e_msg.data;

            var message = JSON.parse(msg);

            console.log("RECEIVED MSG IS: ")
            console.log(message);
            let itemType = message.itemType;
            var valuesArray = message.valuesArray;

            if(itemType == "switches"){
                console.log("Array length is 2");

                let upItemState = valuesArray[0]+"state";
                let upItemBtn = valuesArray[0]+"btn";
                let upState = valuesArray[1];
                updateOnOff(upItemState, upItemBtn, upState);

            }else if(itemType == "rgb"){
                console.log("Array length is 3");
                updateRGBcolor(valuesArray);
            }
            
        }

        function updateOnOff(itemSt, itemBtn ,upst){
            let state = document.getElementById(itemSt);
            let itembtn = document.getElementById(itemBtn);
            console.log("Updates:"+upst+" st: "+itemSt);
            if (upst=="on"){
                itembtn.innerHTML="turn off";
                itembtn.value="off";

                state.classList.remove("offstate");
                state.classList.add("onstate");
                state.innerHTML="ON";
                
                console.log(itembtn.value);

            }else{

                itembtn.innerHTML="turn on";
                itembtn.value="on";

                state.classList.remove("onstate");
                state.classList.add("offstate");
                state.innerHTML="OFF";
                
                console.log(itembtn.value);
            }
        }

// UPDATE RGB DATA WHEN RECEIVED SERVER DATA
        function updateRGBcolor(updteRGBarr){
            let redvalue = 0;
            let greenvalue = 0;
            let bluevalue = 0;

            redvalue = updteRGBarr[0];
            greenvalue = updteRGBarr[1];
            bluevalue = updteRGBarr[2];

            let redbox = document.getElementById("redbox");
            let greenbox = document.getElementById("greenbox");
            let bluebox = document.getElementById("bluebox");

            let redrange = document.getElementById("redrange");
            let greenrange = document.getElementById("greenrange");
            let bluerange = document.getElementById("bluerange");
            let mixrgb = document.getElementById("mixrgb");

            redrange.value = redvalue;
            greenrange.value = greenvalue;
            bluerange.value = bluevalue;

            redbox.textContent = redvalue;
            redbox.style.background = "rgb("+redvalue+", 0, 0)";

            greenbox.textContent = greenvalue;
            greenbox.style.background = "rgb(0, "+greenvalue+", 0)";

            bluebox.textContent = bluevalue;
            bluebox.style.background = "rgb(0, 0, "+bluevalue+")";

            mixrgb.style.background="rgb("+redvalue+", "+greenvalue+", "+bluevalue+")";
        }
//END


        // RGB LIGHT CONTROLL

        function changeRed(){
            let redbox = document.getElementById("redbox");
            let redrange = document.getElementById("redrange");
            redbox.textContent=redrange.value;
            redbox.style.background="rgb("+redrange.value+", 0, 0)";
            let colorvalue = redrange.value;
            mixRGB(colorvalue);
        }

        function changeGreen(){
            let greenbox = document.getElementById("greenbox");
            let greenrange = document.getElementById("greenrange");
            greenbox.textContent=greenrange.value;
            greenbox.style.background="rgb(0, "+greenrange.value+", 0)";
            let colorvalue = greenrange.value;
            mixRGB(colorvalue);
        }

        function changeBlue(){
            let bluebox = document.getElementById("bluebox");
            let bluerange = document.getElementById("bluerange");
            bluebox.textContent=bluerange.value;
            bluebox.style.background="rgb(0, 0, "+bluerange.value+")";
            let colorvalue = bluerange.value;
            mixRGB(colorvalue);
        }

        function mixRGB(colorvalue){
            let mixrgb = document.getElementById("mixrgb");
            mixrgb.style.background="rgb("+redrange.value+", "+greenrange.value+", "+bluerange.value+")";
            var RGBarray = [redrange.value, greenrange.value, bluerange.value];
            sendRGBdata(RGBarray);
        }

    </script>
</head>
<body>
    <div id="main-container center">
        <h2 style="text-align: center; color: blueviolet; border: solid;">Dasun's Room Automation</h2>
        <div class="websocketContainer"> 
            <div class="center" style="width: fit-content;">
                <button class="btn" id="bt_connect" onclick="connect_server()">Connect</button>
            </div>
            <div class="center" style="width: fit-content;">
                <span style=" width: 360px; text-align: center;">WebSocket Status: </span><span style="color: blueviolet; text-align: center;" id="ws_state">Closed</span>
            </div>
        </div>

        <div class="grid-container" id="inputContainer">
            <div>
                <span class="center" style="font-weight: bold;">Room Light</span>
                <div class="state center" id="item1state">OFF</div>
                <button class="btn-input" value="on" id="item1btn" onclick="onofffunc(1)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Night Light</span>
                <div class="state center" id="item2state">OFF</div>
                <button class="btn-input" value="on" id="item2btn" onclick="onofffunc(2)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Fan</span>
                <div class="state center" id="item3state">OFF</div>
                <button class="btn-input" value="on" id="item3btn" onclick="onofffunc(3)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Plug 1</span>
                <div class="state center" id="item4state">OFF</div>
                <button class="btn-input" value="on" id="item4btn" onclick="onofffunc(4)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Plug 2</span>
                <div class="state center" id="item5state">OFF</div>
                <button class="btn-input" value="on" id="item5btn" onclick="onofffunc(5)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Plug 3</span>
                <div class="state center" id="item6state">OFF</div>
                <button class="btn-input" value="on" id="item6btn" onclick="onofffunc(6)" style="margin-top:20px;">turn on</button>
            </div>

        </div>

        <div class="grid-container1" style="margin-top: 0px;">
            <div>
                <span class="" style="font-weight: bold; width: max-content;"><span style="color: red;">R</span><span style="color: green;">G</span><span style="color: blue;">B</span> LED Strip</span>

                <div class="grid-container-rgb center">
                    <div class="center" style="width: 20px;"><span class="center" >R</span></div>
                    <div class="ledcolor center" id="redbox">0</div>
                    <div class="center"><input type="range" id="redrange" value="0" min="0" max="255" oninput="changeRed()"></div>
                    
                
                    <div class="center" style="width: 20px;" ><span class="center" >G</span></div>
                    <div class="ledcolor center" id="greenbox">0</div>
                    <div class="center"><input type="range" id="greenrange" value="0" min="0" max="255" oninput="changeGreen()"></div>
                    
                
                    <div class="center" style="width: 20px;" ><span class="center" >B</span></div>
                    <div class="ledcolor center" id="bluebox">0</div>
                    <div class="center"><input type="range" id="bluerange" value="0" min="0" max="255" oninput="changeBlue()"> </div>
                    
                </div>

                <div class="center" id="mixrgb" style="margin-top: 20px; width:50vw; height: 1vw; background-color: black;"></div>

            </div>
        </div>
    </div>
    
</body>
</html>
)=====";
