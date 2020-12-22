var xmlHttp = createXmlHttpObject();
var allData;
var loaded = 0;

function loader() {
    process();
    load_once();

}



function createXmlHttpObject() {
    if (window.XMLHttpRequest) {
        xmlHttp = new XMLHttpRequest();
    } else {
        xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
}

function process() {
    if (xmlHttp.readyState == 0 || xmlHttp.readyState == 4) {
        xmlHttp.open('PUT', '/data.json', true);
        xmlHttp.send(null);
        xmlHttp.onreadystatechange = handleServerResponse;
    }
    setTimeout('process()', 1000);
}

function handleServerResponse() { // чтение данных с сервера
    if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
        allData = JSON.parse(xmlHttp.responseText);
        document.getElementById('CURRENTTIME').value = allData.TIME;
        document.getElementById('RUNTIME').value = allData.RTIM;
        document.getElementById('VERSION').value = allData.VER;
        document.getElementById('MODEI').value = allData.MOD;

        loaded = 1;
    }
}

function load_once() {
    if (loaded == 1) {
        document.getElementById('MODE').value = allData.MOD;
        document.getElementById('MINT').value = allData.MIT;
        document.getElementById('MAXT').value = allData.MTT;
        document.getElementById('MAXS').value = allData.MST;
        document.getElementById('HEATR').value = allData.HR;
    } else setTimeout(load_once, 1000);
}

function request_new(server) {
    var request = new XMLHttpRequest();
    request.open("GET", server, true);
    request.send();
    if (request.readyState != 4 && request.status != 200) {
        //  alert('Status= ' +request.status + ': ' + request.statusText + ', readyState= ' + request.readyState);
    }
}


function SetMODE() {
    var modesam = document.getElementById('MODE').value;
    server = "/SetMD?modesam=" + modesam;
    request_new(server);
}

function SetMINT() {
    var minboil = document.getElementById('MINT').value;
    server = "/SetMB?minboil=" + minboil;
    request_new(server);
}

function SetMTT() {
    var maxtank = document.getElementById('MAXT').value;
    server = "/SetMT?maxtank=" + maxtank;
    request_new(server);
}

function SetMST() {
    var stmax = document.getElementById('MAXS').value;
    server = "/SetSM?stmax=" + stmax;
    request_new(server);
}

function SetHR() {
    var delta0 = document.getElementById('HEATR').value;
    server = "/SetD0?delta0=" + delta0;
    request_new(server);
}

function sendbutton(button) {
    var server = "/button?state=" + button;
    request_new(server);
}



// Скрипт включения в страницу
//https://html5css.ru/howto/howto_html_include.php
//https://www.w3schools.com/howto/howto_html_include.asp
function includeHTML() {
    var z, i, elmnt, file, xhttp;
    /*loop through a collection of all HTML elements:*/
    z = document.getElementsByTagName("*");
    for (i = 0; i < z.length; i++) {
        elmnt = z[i];
        /*search for elements with a certain atrribute:*/
        file = elmnt.getAttribute("w3-include-html");
        if (file) {
            /*make an HTTP request using the attribute value as the file name:*/
            xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4) {
                    if (this.status == 200) {
                        elmnt.innerHTML = this.responseText;
                    }
                    if (this.status == 404) {
                        elmnt.innerHTML = "Page not found.";
                    }
                    /*remove the attribute, and call this function once more:*/
                    elmnt.removeAttribute("w3-include-html");
                    includeHTML();
                }
            }
            xhttp.open("GET", file, true);
            xhttp.send();
            /*exit the function:*/
            return;
        }
    }
};