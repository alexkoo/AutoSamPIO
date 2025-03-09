//<!--
var xmlHttp = createXmlHttpObject(); //запрос
var allData;
var st_mode = 0;
var loaded = 0;
var indexLoad = 1; // индикатор что загружен индекс
//var valveStatus;
var autoStatus;

function process() {
  //цикл выполнения
  if (xmlHttp.readyState == 0 || xmlHttp.readyState == 4) {
    xmlHttp.open("PUT", "/data.json", true);
    xmlHttp.send(null);
    xmlHttp.onreadystatechange = handleServerResponse;

    if (indexLoad == 1) {
      //valve_status();
      invis();
      auto_status();
      mode_inf();
    }

  }
  setTimeout("process()", 1000);
}

function createXmlHttpObject() {
  //создание запроса
  if (window.XMLHttpRequest) {
    xmlHttp = new XMLHttpRequest();
  } else {
    xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
  }
  return xmlHttp;
}

function request_new(server) {
  //Запрос к серверу
  var request = new XMLHttpRequest();
  request.open("GET", server, true);
  request.send();
  // request.onreadystatechange = function() {              //запрашивает request.readyState, че с ним потом делать хз
  // if (request.readyState == 4 && request.status == 200) {
  //  alert('Status= ' + request.status + ': ' + request.statusText + ', readyState= ' + request.readyState);
  // }
  //}
}

function handleServerResponse() {
  // парсинг json
  if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
    allData = JSON.parse(xmlHttp.responseText);
    //head
    document.getElementById("CURRENTTIME").value = allData.TIME;
    document.getElementById("RUNTIME").value = allData.RTIM;
    document.getElementById("VERSION").value = allData.VER;
    st_mode = Number(allData.MOD);
    //dist
    if (indexLoad == 1) {
      document.getElementById("STEAMTEMP").value = allData.ST;
      document.getElementById("STEAMTEMPF").value = allData.SF;
      document.getElementById("STEAMTEMPS").value = allData.SS;

      document.getElementById("PIPETEMP").value = allData.PT;
      document.getElementById("PIPETEMPF").value = allData.PF;
      document.getElementById("PIPETEMPS").value = allData.PS;

      document.getElementById("TANKTEMP").value = allData.TT;
      document.getElementById("TANKTEMPF").value = allData.TF;
      document.getElementById("TANKTEMPS").value = allData.TS;

      document.getElementById("WATERTEMP").value = allData.WT;
      document.getElementById("SETSTEAMS").value = allData.SSS;

      document.getElementById("DELTAS").value = allData.HS;
      document.getElementById("DELTAP").value = allData.HP;
      document.getElementById("DELTAT").value = allData.HT;
      document.getElementById("SETTEMPS").value = allData.STS;
      document.getElementById("SETTEMPP").value = allData.STP;

      document.getElementById("APRESS2").value = allData.AP;
      document.getElementById("AIRTEMP").value = allData.AT;
    //document.getElementById("MEMFREE").value = allData.FM;
      autoStatus = allData.AS;
  

    }
    // settings
    loaded = 1;
  }
}


function mode_inf() {
  st_mode = String(autoStatus)[5];
  st_mode_arr = ["ERR", "Рект", "Дист", "Погода"]
  st_mode_val = st_mode_arr[st_mode];
  document.getElementById("MODEI").value = st_mode_val;
}


function auto_status() {

  st_proc = String(autoStatus)[2];
  st_auto = String(autoStatus)[3];
  st_valve = String(autoStatus)[4];

  st_proc_arr = [" ", "Бак нагревается  ", "Бак нагрет ", "Нагрев узла ", "Узел нагревается ", "Узел нагрет ", "Отбор хвостов ", "конец отбора ",];
  st_auto_arr = ["MAN ", "Auto  Pipe ", "Auto Steam "];
  st_valve_arr = ["Closed ", "Open "];

  st_proc_val = st_proc_arr[st_proc];
  st_auto_val = st_auto_arr[st_auto];
  st_valve_val = st_valve_arr[st_valve];

  status_str = st_proc_val + st_valve_val + st_auto_val;

  document.getElementById("AUTOSTATUS").value = status_str;

}
/*
function valve_status() {
  if (valveStatus == 1) {
    document.getElementById("VALVE").value = "Открыт";
  } else {
    document.getElementById("VALVE").value = "Закрыт";
  }
}

*/
function load_once() {
  if (loaded == 1) {
    document.getElementById("MINT").value = allData.MIT;
    document.getElementById("MAXT").value = allData.MTT;
    document.getElementById("MAXS").value = allData.MST;
    document.getElementById("HEATR").value = allData.HR;
    document.getElementById("ADDR").value = allData.ADDR;
  } else setTimeout(load_once, 1000);
  
}


function send_form_mode() {
  //отправка значения статуса
  var autosam_mode = document.form_mode.MODE.value;
  server = "/SetFormMode?autosam_mode_h=" + autosam_mode;
  request_new(server);
}
function send_form_index() {
  var result = confirm('Подтвердите действие');
  if (!result) {
        return;
  }

  var ds_index = document.form_index.DS_IND.value;
  server = "/SetFormIndex?ds_index=" + ds_index;
  request_new(server);
}



function send_form_settings() {
  var min_hot_temp = document.getElementById("MINT").value;
  var max_tank_temp = document.getElementById("MAXT").value;
  var max_steam_temp = document.getElementById("MAXS").value;
  var heating_rate = document.getElementById("HEATR").value;

  var s_min_hot_temp = "&min_hot_temp_h=" + min_hot_temp;
  var s_max_tank_temp = "&max_tank_temp_h=" + max_tank_temp;
  var s_max_steam_temp = "&max_steam_temp_h=" + max_steam_temp;
  var s_heating_rate = "&heating_rate_h=" + heating_rate;
  server =
    "/SetForm?" +
    s_min_hot_temp +
    s_max_tank_temp +
    s_max_steam_temp +
    s_heating_rate;
  request_new(server);
  setTimeout(load_once, 5000);
}

function sendbutton(button) {  //отправка значений кнопок
  //if (button = (42 || 43))
 // var result = confirm('Подтвердите действие');
  //if (!result) {
       // return;
  //}
  var server = "/button?state=" + button;
  request_new(server);
  if (button = 41) setTimeout(load_once, 3000);
  }

function invis() //скрывет лишнее поле 
{
  if (st_mode == 2) {
    document.getElementById("rect_table").style.display = "none";
  }
  else {
    document.getElementById("rect_table").style.display = "block";
  }
}

function sendDelS() {  // отправка уставки steam на сервер
  var delta_s = document.getElementById("DELTA_S").value;
  var delay_s = document.getElementById("DELAY_S").value;
  var server = "/DelS?delta_s=" + delta_s + "&delay_s=" + delay_s;
  request_new(server);
}

function sendDelP() {// отправка уставки pipe на сервер
    var delta_p = document.getElementById("DELTA_P").value;
  var delay_p = document.getElementById("DELAY_P").value;
  var server = "/DelP?delta_p=" + delta_p + "&delay_p=" + delay_p;
  request_new(server);
}


