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