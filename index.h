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


        // function item1func(){
        //     let state = document.getElementById("item1state");
        //     let item1btn = document.getElementById("item1btn");

        //     if (item1btn.value=="on"){
        //         state.classList.remove("offstate");
        //         state.classList.add("onstate");
        //         state.innerHTML="ON";
        //         item1btn.innerHTML="turn off";
        //         item1btn.value="off";
        //         st=1;
        //         console.log(item1btn.value);
        //     }else{
        //         state.classList.remove("onstate");
        //         state.classList.add("offstate");
        //         state.innerHTML="OFF";
        //         item1btn.innerHTML="turn on";
        //         item1btn.value="on";
        //         st=0;
        //         console.log(item1btn.value);
        //     }
        // }

        function onofffunc(st, btn, itemnum){
            let state = document.getElementById(st);
            let itembtn = document.getElementById(btn);

            if (itembtn.value=="on"){
                itembtn.innerHTML="turn off";
                ws.send(itemnum+itembtn.value);
                itembtn.value="off";

                // state.classList.remove("offstate");
                // state.classList.add("onstate");
                // state.innerHTML="ON";
                
                console.log(itembtn.value);

            }else{

                itembtn.innerHTML="turn on";
                ws.send(itemnum+itembtn.value);
                itembtn.value="on";

                // state.classList.remove("onstate");
                // state.classList.add("offstate");
                // state.innerHTML="OFF";
                
                console.log(itembtn.value);
            }
        }

        function onoffcommon(item){
            switch(item){
                case 1:
                    onofffunc("item1state", "item1btn", "item1");
                    break;
                case 2:
                    onofffunc("item2state", "item2btn", "item2");
                    break;
                case 3:
                    onofffunc("item3state", "item3btn", "item3");
                    break;
                case 4:
                    onofffunc("item4state", "item4btn", "item4");
                    break;
                case 5:
                    onofffunc("item5state", "item5btn", "item5");
                    break;
                case 6:
                    onofffunc("item6state", "item6btn", "item6");
                    break;
            }
        }




        function ws_onmessage(e_msg) {
            e_msg = e_msg || window.event; // MessageEvent
            console.log(e_msg.data);
            if(e_msg.data == "item1off"){
                updateOnOff("item1state", "item1btn", "off");

            }else if(e_msg.data == "item1on"){
                updateOnOff("item1state", "item1btn", "on");

            }else if(e_msg.data == "item2off"){
                updateOnOff("item2state", "item2btn", "off");

            }else if(e_msg.data == "item2on"){
                updateOnOff("item2state", "item2btn", "on");

            }else if(e_msg.data == "item3off"){
                updateOnOff("item3state", "item3btn", "off");

            }else if(e_msg.data == "item3on"){
                updateOnOff("item3state", "item3btn", "on");

            }else if(e_msg.data == "item4off"){
                updateOnOff("item4state", "item4btn", "off");

            }else if(e_msg.data == "item4on"){
                updateOnOff("item4state", "item4btn", "on");

            }else if(e_msg.data == "item5off"){
                updateOnOff("item5state", "item5btn", "off");
                
            }else if(e_msg.data == "item5on"){
                updateOnOff("item5state", "item5btn", "on");

            }else if(e_msg.data == "item6off"){
                updateOnOff("item6state", "item6btn", "off");

            }else if(e_msg.data == "item6on"){
                updateOnOff("item6state", "item6btn", "on");
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
                <button class="btn-input" value="on" id="item1btn" onclick="onoffcommon(1)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Night Light</span>
                <div class="state center" id="item2state">OFF</div>
                <button class="btn-input" value="on" id="item2btn" onclick="onoffcommon(2)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Fan</span>
                <div class="state center" id="item3state">OFF</div>
                <button class="btn-input" value="on" id="item3btn" onclick="onoffcommon(3)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Plug 1</span>
                <div class="state center" id="item4state">OFF</div>
                <button class="btn-input" value="on" id="item4btn" onclick="onoffcommon(4)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Plug 2</span>
                <div class="state center" id="item5state">OFF</div>
                <button class="btn-input" value="on" id="item5btn" onclick="onoffcommon(5)" style="margin-top:20px;">turn on</button>
            </div>

            <div>
                <span class="center" style="font-weight: bold;">Plug 3</span>
                <div class="state center" id="item6state">OFF</div>
                <button class="btn-input" value="on" id="item6btn" onclick="onoffcommon(6)" style="margin-top:20px;">turn on</button>
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
