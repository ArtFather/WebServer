var xmlHttp=createXmlHttpObject();
function createXmlHttpObject(){
   
 if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
 }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}

function loadconfig() {
   var xhr = new XMLHttpRequest();
   xhr.open('GET', 'config.json', false);
   xhr.send();
   if (xhr.status != 200) {
      alert('Ошибка ' + xhr.status + ': ' + xhr.statusText);
   } else {
      var obj = JSON.parse(xhr.responseText);
      var new_string = "";
      data = document.getElementsByTagName('body')[0].innerHTML;
      for (var key in obj) {
         new_string = data.replace(new RegExp('{{' + key + '}}', 'g'), obj[key]);
         data = new_string
      }
      document.getElementsByTagName('body')[0].innerHTML = new_string;
   }
}

 function restart(submit,texts){
    if (confirm(texts)) {
	   server = "/restart?device=ok";
     send_request(submit,server);
     return true;
    }
    else {
     return false;
    }
      }

function val(id) {
   var v = document.getElementById(id).value;
   return v;
}

function send_request(submit, server) {
   request = new XMLHttpRequest();
   request.open("POST", server, true);
   request.send();
}

function set_ssid(submit) {
   server = "/ssid?ssid=" + val('ssid') + "&password=" + encodeURIComponent(val('password'));
   send_request(submit, server);
   alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
}

function set_ssidAP(submit) {
   server = "/ssidAP?ssidAP=" + val('ssidAP') + "&passwordAP=" + encodeURIComponent(val('passwordAP'));
   send_request(submit, server);
   alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
}

