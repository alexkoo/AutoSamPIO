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
        document.getElementById('DELTAS').value = allData.HS;
        document.getElementById('DELTAP').value = allData.HP;
        document.getElementById('DELTAT').value = allData.HT;
        document.getElementById('VALVE').value = allData.VS;
        document.getElementById('VERSION').value = allData.VR;
        document.getElementById('MODEI').value = allData.MD;
        document.getElementById('SETTEMP1I').value = allData.S1;
        document.getElementById('SETTEMP2I').value = allData.S2;
        document.getElementById('AUTOSTATUS').value = allData.AS;
    }
}

function sendbutton(button) { //отправка значений кнопок
    var server = "/button?state=" + button;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}

function sendDelS() { // отправка уставки pipe на сервер
    var delta_s = document.getElementById('DELTA_S').value;
    var delay_s = document.getElementById('DELAY_S').value;
    if (delta_s != 0 && auto == false) {
        document.getElementById('APRESS1').value = document.getElementById('APRESS2').value;
    } else if (delta_s == 0 && auto == true) {
        document.getElementById('APRESS1').value = "??";
    }
    server = "/DelS?delta_s=" + delta_s + "&delay_s=" + delay_s;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();
    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}

function sendDelP() { // отправка уставки pipe на сервер
    var delta_p = document.getElementById('DELTA_P').value;
    var delay2 = document.getElementById('DELAY_P').value;
    if (delta_p != 0 && auto == false) {
        document.getElementById('APRESS1').value = document.getElementById('APRESS2').value;
    } else if (delta_p == 0 && auto == true) {
        document.getElementById('APRESS1').value = "??";
    }
    server = "/Del2?delta_p=" + delta_p + "&delay_p=" + delay2;
    request = new XMLHttpRequest();
    request.open("GET", server, false);
    request.send();

    if (request.status != 200) {
        alert(request.status + ': ' + request.statusText);
    }
}


// Скрипт включения в страницу
//https://www.w3schools.com/howto/howto_html_include.asp
//https://html5css.ru/howto/howto_html_include.php

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