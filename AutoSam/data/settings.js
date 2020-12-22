var xmlHttp = createXmlHttpObject();
var allData;
var loaded = 0;

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

function SendForm() {
    var modesam = document.getElementById('MODE').value;
    var minboil = document.getElementById('MINT').value;
    var maxtank = document.getElementById('MAXT').value;
    var maxst = document.getElementById('MAXS').value;
    var heat_rate = document.getElementById('HEATR').value;

    var s_modesam = "/SetForm?modesam=" + modesam;
    var s_minboil = "&delta0=" + heat_rate;
    var s_maxtank = "&stmax=" + maxst;
    var s_maxst = "&maxtank=" + maxtank;
    var s_heat_rate = "&minboil=" + minboil;
    server = s_modesam + s_minboil + s_maxtank + s_maxst + s_heat_rate
    request_new(server);
    setTimeout(load_once, 3000);
}