// Function to set cookies, copied from https://www.w3schools.com/js/js_cookies.asp
function setCookie(cname, cvalue, days) {
    var expires = "";
    if (days) {
        var date = new Date();
        date.setTime(date.getTime() + (days*24*60*60*1000));
        expires = "; expires=" + date.toUTCString();
    }
    document.cookie = cname + "=" + (cvalue || "")  + expires + "; path=/";
}

// Function to obtain cookies, copied from https://www.w3schools.com/js/js_cookies.asp
function getCookie(cname) {
    var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for(var i = 0; i <ca.length; i++) {
      var c = ca[i];
      while (c.charAt(0) == ' ') {
        c = c.substring(1);
      }
      if (c.indexOf(name) == 0) {
        return c.substring(name.length, c.length);
      }
    }
    return "";
}

// Initializes default theme
function init_theme(){
    let current_theme = getCookie("theme");
    if (current_theme === "") {
        setCookie("theme", "dark", 30);
    } else {
        set_theme(current_theme);
    }
}

function set_theme(theme) {
    if (theme === "dark") {
        document.getElementById("body").className = "bootstrap-dark";
    } else {
        document.getElementById("body").className = "bootstrap";
    }
}

function toggle_theme() {
    let current_theme = getCookie("theme");
    if (current_theme === "light") {
        setCookie("theme", "dark", 30);
    } else {
        setCookie("theme", "light", 30);
    }
    current_theme = getCookie("theme");
    set_theme(current_theme);
}

init_theme();