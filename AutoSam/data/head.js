function sendbutton(button) { //отправка значений кнопок
    var server = "/button?state=" + button;
    request_new(server);
}


function request_new(server) {
    var request = new XMLHttpRequest();
    request.open("GET", server, true);
    request.send();
    if (request.readyState != 4 && request.status != 200) {
        //  alert('Status= ' +request.status + ': ' + request.statusText + ', readyState= ' + request.readyState);
    }
}


function includeHTML() { // Скрипт включения в страницу https://www.w3schools.com/howto/howto_html_include.asp https://html5css.ru/howto/howto_html_include.php
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