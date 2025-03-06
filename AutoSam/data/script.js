//<!--
var xmlHttp = createXmlHttpObject(); //запрос
var allData;
var modeVisible = 0;
var loaded = 0;
var distPlug = 1; // индикатор что загружен индекс
var valveStatus;

function process() {
  //цикл выполнения
  if (xmlHttp.readyState == 0 || xmlHttp.readyState == 4) {
    xmlHttp.open("PUT", "/data.json", true);
    xmlHttp.send(null);
    xmlHttp.onreadystatechange = handleServerResponse;
    valve_status();
    mode_inf();

    if (distPlug == 1) {
      invis();
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
    modeVisible = allData.MOD;
    //document.getElementById("MODEI").value = allData.MOD;
    //dist
    if (distPlug == 1) {
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
      document.getElementById("AUTOSTATUS").value = allData.AS;
      document.getElementById("MEMFREE").value = allData.FM;
      valveStatus = allData.VS;
      //modeVisible = allData.MOD;

    }
    // settings
    loaded = 1;
  }
}

function mode_inf() {
  if (modeVisible == 1) {
    document.getElementById("MODEI").value = " Рект ";
  }
  if (modeVisible == 2) {
    document.getElementById("MODEI").value = " Дист ";
  }
  else {
    document.getElementById("MODEI").value = "ERR";
  }
}


function valve_status() {
  if (valveStatus == 1) {
    document.getElementById("VALVE").value = "Открыт";
  } else {
    document.getElementById("VALVE").value = "Закрыт";
  }
}

function load_once() {
  if (loaded == 1) {
    document.getElementById("MINT").value = allData.MIT;
    document.getElementById("MAXT").value = allData.MTT;
    document.getElementById("MAXS").value = allData.MST;
    document.getElementById("HEATR").value = allData.HR;
  } else setTimeout(load_once, 1000);
}

function send_form_mode() {
  //отправка значения статуса
  var autosam_mode = document.form1.MODE.value;
  var s_autosam_mode = "/SetForm?autosam_mode_h=" + autosam_mode;
  server = s_autosam_mode;
  request_new(server);
  //setTimeout(load_once, 5000);
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

function sendbutton(button) {
  //отправка значений кнопок
  var server = "/button?state=" + button;
  request_new(server);
}

function invis() {
  //скрывет лишнее поле 

  if (modeVisible == 2) {
    document.getElementById("rect_table").style.display = "none";
  }

  else {
    document.getElementById("rect_table").style.display = "block";
  }

}



function sendDelS() {
  // отправка уставки steam на сервер
  var delta_s = document.getElementById("DELTA_S").value;
  var delay_s = document.getElementById("DELAY_S").value;
  var server = "/DelS?delta_s=" + delta_s + "&delay_s=" + delay_s;
  request_new(server);
}

function sendDelP() {
  // отправка уставки pipe на сервер
  var delta_p = document.getElementById("DELTA_P").value;
  var delay_p = document.getElementById("DELAY_P").value;
  var server = "/DelP?delta_p=" + delta_p + "&delay_p=" + delay_p;
  request_new(server);
}

function includeHTML() {
  // Скрипт включения в страницу https://www.w3schools.com/howto/howto_html_include.asp https://html5css.ru/howto/howto_html_include.php
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
      xhttp.onreadystatechange = function () {
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
      };
      xhttp.open("GET", file, true);
      xhttp.send();
      /*exit the function:*/
      return;
    }
  }
}
