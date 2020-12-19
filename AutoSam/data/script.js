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
        document.getElementById('STEAMTEMP').value = allData.ST;
        document.getElementById('STEAMTEMPF').value = allData.SF;
        document.getElementById('STEAMTEMPS').value = allData.SS;
        document.getElementById('PIPETEMP').value = allData.PT;
        document.getElementById('PIPETEMPF').value = allData.PF;
        document.getElementById('PIPETEMPS').value = allData.PS;
        document.getElementById('WATERTEMP').value = allData.WT;
        document.getElementById('TANKTEMP').value = allData.TT;
        document.getElementById('TANKTEMPF').value = allData.TF;
        document.getElementById('TANKTEMPS').value = allData.TS;
        document.getElementById('APRESS2').value = allData.AP;
        document.getElementById('AIRTEMP').value = allData.AT;
        document.getElementById('MEMFREE').value = allData.FM;
        document.getElementById('WARNING').value = allData.WA;
        document.getElementById('DELTAS').value = allData.DS;
        document.getElementById('DELTAP').value = allData.DP;
        document.getElementById('DELTAT').value = allData.DT;
        document.getElementById('VALVE').value = allData.VS;
        document.getElementById('VERSION').value = allData.VR;
        document.getElementById('MODEI').value = allData.MD;
        document.getElementById('SETTEMP1I').value = allData.S1;
        document.getElementById('SETTEMP2I').value = allData.S2;
        document.getElementById('AUTOSTATUS').value = allData.AS;
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
var auto = false;

function setDel1() {
    var delta1 = document.getElementById('DELTA1').value;
    var delay1 = document.getElementById('DELAY1').value;
    if (delta1 != 0 && auto == false) {
        document.getElementById('APRESS1').value = document.getElementById('APRESS2').value;
        //document.getElementById('SETTEMP1').value=(+document.getElementById('STEAMTEMP').value + +delta1).toFixed(2);
        //document.getElementById('SETTEMP1').value=(document.getElementById('SETTEMP1I').value;
        auto = true;

    } else if (delta1 != 0 && auto == true) {
        //document.getElementById('SETTEMP1').value=(+document.getElementById('STEAMTEMP').value + +delta1).toFixed(2);
        //document.getElementById('SETTEMP1').value=(document.getElementById('SETTEMP1I').value;
    } else if (delta1 == 0 && auto == true) {
        //document.getElementById('SETTEMP1').value=200;
        document.getElementById('APRESS1').value = "??";
        auto = false; // отключаем режим auto
    }
    server = "/Del1?delta1=" + delta1 + "&delay1=" + delay1;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}

function setDel2() {
    var delta2 = document.getElementById('DELTA2').value;
    var delay2 = document.getElementById('DELAY2').value;
    if (delta2 != 0 && auto == false) {
        document.getElementById('APRESS1').value = document.getElementById('APRESS2').value;
        //document.getElementById('SETTEMP2').value=(+document.getElementById('PIPETEMP').value + +delta2).toFixed(2);
        //document.getElementById('SETTEMP2').value=(document.getElementById('SETTEMP2I').value;
        auto = true;
    } else if (delta2 != 0 && auto == true) {

        //document.getElementById('SETTEMP2').value=(+document.getElementById('PIPETEMP').value + +delta2).toFixed(2);
        //document.getElementById('SETTEMP2').value=(document.getElementById('SETTEMP2I').value;
    } else if (delta2 == 0 && auto == true) {
        //document.getElementById('SETTEMP2').value=200;
        document.getElementById('APRESS1').value = "??";
        auto = false;
    }
    server = "/Del2?delta2=" + delta2 + "&delay2=" + delay2;
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