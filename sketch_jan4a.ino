#include <NTPClient.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>

const char* ssid = "Snacks On The Way";
const char* password = "123456789";

const char index_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <style>
      html {
        font-family: Helvetica;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      .button {
        background-color: #4CAF50;
        border: none;
        color: white;
        padding: 16px 40px;
        text-decoration: none;
        font-size: 30px;
        margin: 2px;
        cursor: pointer;
      }
      .button2 {
        background-color: #555555;
      }
    </style>
  </head>
  <body>
    <h1>Snacks On The Way</h1>
    <p>
      <a href='javascript:sendRequest("button1")'>
        <button class='button'>Item 1</button>
      </a>
    </p>
    <p>
      <a href='javascript:sendRequest("button2")'>
        <button class='button'>Item 2</button>
      </a>
    </p>
    <p>
      <a href='javascript:sendRequest("button3")'>
        <button class='button'>Item 3</button>
      </a>
    </p>
    <p>
      <a href='javascript:sendRequest("button4")'>
        <button class='button'>Item 4</button>
      </a>
    </p>
    <p id="button1Label"></p>
    <p id="button2Label"></p>
    <p id="button3Label"></p>
    <p id="button4Label"></p>

    <input type="password" id="passwordInput" placeholder="Enter password to restock item 1" style="display: none">

    <script>

    let button1ClickCount = 0;
    let button2ClickCount = 0;
    let button3ClickCount = 0;
    let button4ClickCount = 0;
    
    let password = "12345mm";
  
    document.getElementById("passwordInput").addEventListener("keyup", function(event) {
    if (event.keyCode === 13) {
      event.preventDefault();
      checkPassword();
    }
  });
  
  function checkPassword() {
    if (document.getElementById("passwordInput").value === password) {
      button1ClickCount = 0;
      alert("Password accepted. Item 1 is now restocked");
      document.getElementById("button1Label").style.display = "none";
      document.getElementById("passwordInput").style.display = "none";
    } else {
      alert("Incorrect password. Please try again.");
    }
  }

  function sendRequest(button) {
    var confirmation;
    if (button == "button1") {
      button1ClickCount++;
    if (button1ClickCount >= 4) {
      document.getElementById("button1Label").innerHTML = "Item 1 is out of stock";
      document.getElementById("passwordInput").style.display = "block";
    } else {
    document.getElementById("passwordInput").style.display = "none";
  }
      confirmation = confirm("Allergy Alert! This item contains milk, do you want to proceed?");
    } else if (button == "button2") {
      button2ClickCount++;
    if (button2ClickCount >= 4) {
      document.getElementById("button2Label").innerHTML = "Item 2 is out of stock";
      button2ClickCount = 0;
    }
      confirmation = confirm("AAllergy Alert! This item contains Peanut butter, do you want to proceed?");
    } else if (button == "button3") {
      button3ClickCount++;
    if (button3ClickCount >= 4) {
      document.getElementById("button3Label").innerHTML = "Item 3 is out of stock";
      button3ClickCount = 0;
    }

      confirmation = confirm("Allergy Alert! This item contains nuts, do you want to proceed?");
    } else if (button == "button4") {
      button4ClickCount++;
    if (button4ClickCount >= 4) {
      document.getElementById("button4Label").innerHTML = "Item 4 is out of stock";
      button4ClickCount = 0;
    }
      confirmation = confirm("Allergy Alert! This item contains wheat, do you want to proceed?");
    }
    if (confirmation) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          console.log(this.responseText);
        }
      };
      xhttp.open("GET", "/" + button, true);
      xhttp.send();
    }
  }
</script>

  </body>
</html>
)rawliteral";

boolean requestProcessed = false;
WiFiServer server(80);  // Set the server to listen on port 80

void setup() {
  WiFi.mode(WIFI_AP);           // Set ESP8266 as an access point
  WiFi.softAP(ssid, password);  // Create an access point with the specified SSID and password
  Serial.begin(115200);

  // Set up the server and handle incoming HTTP requests
  server.begin();
  //Serial.println("Server started")
}

void loop() {
  // Check for incoming client connections
  WiFiClient client = server.available();
  if (client) {
    // A client has connected
    if (!requestProcessed) {
      // Read the request
      String request = client.readStringUntil('\r');

      // Check if the request is a GET request
      if (request.indexOf("GET /") >= 0) {
        // Send the response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");  // Remove this line
        client.println("");
        client.print(index_html);
        requestProcessed = true;  // Set the flag to true
      }
      if (request.indexOf("GET /button1") >= 0) {
        // Print a message to the serial
        Serial.println("o");
        requestProcessed = true;  // Set the flag to true
        //notification
        //sendTelegramMessage("Hello from ESP8266!");
      }

      else if (request.indexOf("GET /button2") >= 0) {
        // Print a message to the serial
        Serial.println("w");
        requestProcessed = true;  // Set the flag to true
      }

      else if (request.indexOf("GET /button3") >= 0) {
        // Print a message to the serial
        Serial.println("t");
        requestProcessed = true;  // Set the flag to true
      }

      else if (request.indexOf("GET /button4") >= 0) {
        // Print a message to the serial
        Serial.println("f");
        requestProcessed = true;  // Set the flag to true
      }

      // Reset the flag
      requestProcessed = false;
    }

    // Close the connection
    client.stop();
  }
}
