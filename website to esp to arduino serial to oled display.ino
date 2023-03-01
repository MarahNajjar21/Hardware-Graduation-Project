#include <ESP8266WiFi.h>

const char* ssid     = "Snacks On The Way";
const char* password = "123456789";

const char index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
<title>ESP8266 Access Point</title>
</head>
<body>
<h1>Hello, World!</h1>
<button onclick='sendRequest("button1")'>Button 1</button>
<button onclick='sendRequest("button2")'>Button 2</button>
<button onclick='sendRequest("button3")'>Button 3</button>
<button onclick='sendRequest("button4")'>Button 4</button>
<script>
function sendRequest(button) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      console.log(this.responseText);
    }
  };
  xhttp.open('GET', '/' + button, true);
  xhttp.send();
}
</script>
</body>
</html>
)rawliteral";

WiFiServer server(80); // Set the server to listen on port 80

void setup() {
  WiFi.mode(WIFI_AP); // Set ESP8266 as an access point
  WiFi.softAP(ssid, password); // Create an access point with the specified SSID and password
  Serial.begin(115200);

  // Set up the server and handle incoming HTTP requests
  server.begin();
  //Serial.println("Server started");
}

void loop() {
  // Check for incoming client connections
  WiFiClient client = server.available();
  if (client) {
    // A client has connected
    //Serial.println("New client connected");

    // Read the request
    String request = client.readStringUntil('\r');
    //Serial.println(request);

    // Check if the request is a GET request
    if (request.indexOf("GET /") >= 0) {
      // Send the response
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.print(index_html);
    }

    // Check if the request is a GET /button1 request
    if (request.indexOf("GET /button1") >= 0) {
      // Print a message to the serial
      Serial.write('o');
    }

    // Check if the request is a GET /button2 request
    else if (request.indexOf("GET /button2") >= 0) {
      // Print a message to the serial
      Serial.write('t');
    }

    // Check if the request is a GET /button3 request
    else if (request.indexOf("GET /button3") >= 0) {
      // Print a message to the serial
      Serial.write('r');
    }

    // Check if the request is a GET /button4 request
    else if (request.indexOf("GET /button4") >= 0) {
      // Print a message to the serial
      Serial.write('f');
    }

    // Close the client connection
    client.stop();
    //Serial.println("Client disconnected");
  }
}