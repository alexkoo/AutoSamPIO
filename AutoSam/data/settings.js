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
    var autosam_mode = document.getElementById('MODE').value;
    var min_hot_temp = document.getElementById('MINT').value;
    var max_tank_temp = document.getElementById('MAXT').value;
    var max_steam_temp = document.getElementById('MAXS').value;
    var heating_rate = document.getElementById('HEATR').value;

    var s_autosam_mode = "/SetForm?autosam_mode_h=" + autosam_mode;
    var s_min_hot_temp = "&min_hot_temp_h=" + min_hot_temp;
    var s_max_tank_temp = "&max_tank_temp_h=" + max_tank_temp;
    var s_max_steam_temp = "&max_steam_temp_h=" + max_steam_temp;
    var s_heating_rate = "&heating_rate_h=" + heating_rate;
    server = s_autosam_mode + s_min_hot_temp + s_max_tank_temp + s_max_steam_temp + s_heating_rate
    request_new(server);
    setTimeout(load_once, 5000);
}