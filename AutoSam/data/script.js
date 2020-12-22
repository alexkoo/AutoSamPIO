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
        document.getElementById('CURRENTTIME').value = allData.TIME;
        document.getElementById('RUNTIME').value = allData.RTIM;
        document.getElementById('VERSION').value = allData.VER;
        document.getElementById('MODEI').value = allData.MOD;
        document.getElementById('STEAMTEMP').value = allData.ST;
        document.getElementById('STEAMTEMPF').value = allData.SF;
        document.getElementById('STEAMTEMPS').value = allData.SS;
        document.getElementById('PIPETEMP').value = allData.PT;
        document.getElementById('PIPETEMPF').value = allData.PF;
        document.getElementById('PIPETEMPS').value = allData.PS;
        document.getElementById('TANKTEMP').value = allData.TT;
        document.getElementById('TANKTEMPF').value = allData.TF;
        document.getElementById('TANKTEMPS').value = allData.TS;
        document.getElementById('WATERTEMP').value = allData.WT;
        document.getElementById('DELTAS').value = allData.HS;
        document.getElementById('DELTAP').value = allData.HP;
        document.getElementById('DELTAT').value = allData.HT;
        document.getElementById('SETTEMP1I').value = allData.STS;
        document.getElementById('SETTEMP2I').value = allData.STP;
        document.getElementById('APRESS2').value = allData.AP;
        document.getElementById('AIRTEMP').value = allData.AT;
        document.getElementById('AUTOSTATUS').value = allData.AS;
        document.getElementById('WARNING').value = allData.WAR;
        document.getElementById('MEMFREE').value = allData.FM;
        document.getElementById('VALVE').value = allData.VS;
    }
}

function sendDelS() { // отправка уставки steam на сервер
    var delta_s = document.getElementById('DELTA_S').value;
    var delay_s = document.getElementById('DELAY_S').value;

    var server = "/DelS?delta_s=" + delta_s + "&delay_s=" + delay_s;
    request_new(server);

}

function sendDelP() { // отправка уставки pipe на сервер
    var delta_p = document.getElementById('DELTA_P').value;
    var delay_p = document.getElementById('DELAY_P').value;
    var server = "/DelP?delta_p=" + delta_p + "&delay_p=" + delay_p;
    request_new(server);

}