//<!--





var xmlHttp = createXmlHttpObject();

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
        var allData = JSON.parse(xmlHttp.responseText);
        document.getElementById('CURRENTTIME').value = allData.CT;
        document.getElementById('RUNTIME').value = allData.RT;
        document.getElementById('VERSION').value = allData.VR;
        document.getElementById('MODEI').value = allData.MD;
        // /head
        document.getElementById('BOILI').value = allData.MB;
        document.getElementById('MTANKI').value = allData.MT;
        document.getElementById('DELT0I').value = allData.D0;
        document.getElementById('STMAXI').value = allData.SM;
    }
}


function SetMD() {
    var modesam = document.getElementById('MODE').value;
    server = "/SetMD?modesam=" + modesam;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}

function SetMB() {

    var minboil = document.getElementById('BOIL').value;

    server = "/SetMB?minboil=" + minboil;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}

function SetMT() {
    var maxtank = document.getElementById('MTANK').value;
    server = "/SetMT?maxtank=" + maxtank;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}

function SetD0() {
    var delta0 = document.getElementById('DELT0').value;
    server = "/SetD0?delta0=" + delta0;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}

function SetSM() {
    var stmax = document.getElementById('STMAX').value;
    server = "/SetSM?stmax=" + stmax;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}

function sendbutton(button) {
    var server = "/button?state=" + button;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}

function reset() {
    server = "/reset";
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
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