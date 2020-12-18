 var heap,temp1,temp2;
    var reloadPeriod = 1000;
    var running = true;
    
    function loadValues(){
      if(!running) return;
      var xh = new XMLHttpRequest();
      xh.onreadystatechange = function(){
        if (xh.readyState == 4){
          if(xh.status == 200) {
            var res = JSON.parse(xh.responseText);
            temp2.add(res.FM);
            temp1.add(res.ST);
            digi.add(res.TT);
            if(running) setTimeout(loadValues, reloadPeriod);
          } else running = false;
        }
      };
      xh.open('PUT','/data.json',true);
      xh.send(null);
    };
    
    function run(){
      if(!running){
        running = true;
        loadValues();
      }
    }
    
    function onBodyLoad(){
      var refreshInput = document.getElementById("refresh-rate");
      refreshInput.value = reloadPeriod;
      refreshInput.onchange = function(e){
        var value = parseInt(e.target.value);
        reloadPeriod = (value > 0)?value:0;
        e.target.value = reloadPeriod;
      }
      var stopButton = document.getElementById("stop-button");
      stopButton.onclick = function(e){
        running = false;
      }
      var startButton = document.getElementById("start-button");
      startButton.onclick = function(e){
        run();
      }
      
      // Example with 10K thermistor
      //function calcThermistor(v) {
      //  var t = Math.log(((10230000 / v) - 10000));
      //  t = (1/(0.001129148+(0.000234125*t)+(0.0000000876741*t*t*t)))-273.15;
      //  return (t>120)?0:Math.round(t*10)/10;
      //}
      //temp = createGraph(document.getElementById("analog"), "Temperature", 100, 128, 10, 40, false, "cyan", calcThermistor);

      // хз, высота, мин, макс, заполнение
      temp1 = createGraph(document.getElementById("temp1"), "Analog Input", 100, 128, 0, 100, false, "cyan");
      temp2 = createGraph(document.getElementById("temp2"), "Analog Input", 100, 128, 0, 100, false, "cyan");
      heap = createGraph(document.getElementById("heap"), "Current Heap", 100, 250, 20000, 40000, true, "orange");
          run();
    }