
<!--

var xmlHttp=createXmlHttpObject();
function createXmlHttpObject(){
if(window.XMLHttpRequest){
xmlHttp=new XMLHttpRequest();
}
else{
xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
}
return xmlHttp;
}
function process(){
if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
xmlHttp.open('PUT','/data.json',true);
xmlHttp.send(null);
xmlHttp.onreadystatechange=handleServerResponse;
}
setTimeout('process()',1000);
}
function handleServerResponse(){ // чтение данных с сервера
if(xmlHttp.readyState==4 && xmlHttp.status==200){
var allData=JSON.parse(xmlHttp.responseText);
document.getElementById('CURRENTTIME').value=allData.CT;   
document.getElementById('RUNTIME').value=allData.RT;
document.getElementById('STEAMTEMP').value=allData.ST;
document.getElementById('STEAMTEMPF').value=allData.SF;
document.getElementById('STEAMTEMPS').value=allData.SS;
document.getElementById('WATERTEMP').value=allData.WT;
document.getElementById('TANKTEMP').value=allData.TT;
document.getElementById('TANKTEMPF').value=allData.TF;
document.getElementById('TANKTEMPS').value=allData.TS; 
document.getElementById('APRESS2').value=allData.AP;
document.getElementById('AIRTEMP').value=allData.AT;
document.getElementById('MEMFREE').value=allData.FM;
document.getElementById('WARNING').value=allData.WA;
document.getElementById('DELTAS').value=allData.DS;
document.getElementById('DELTAT').value=allData.DT;
document.getElementById('VERSION').value=allData.VR;
document.getElementById('MODEI').value=allData.MD;
}
}



