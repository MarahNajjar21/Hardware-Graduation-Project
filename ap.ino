#include <AsyncPrinter.h>
#include <DebugPrintMacros.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncTCPbuffer.h>
#include <SyncClient.h>
#include <async_config.h>
#include <tcp_axtls.h>

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid     = "ESP8266-Access-Point";
const char* password = "123456789";

#define DHTPIN 5     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;  

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <!--how the content should be displayed inside the device-->
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Responsive-Product List Page, Club Monaco</title>
    <!--Page-Header-Icon-->
    <link rel="shortcut icon" href="images/brand-logo.png">
    <!--Style Sheet-CSS-->
    <link rel="stylesheet" type="text/css" href="test.css">

    <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">

    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>

    <!--Social media-->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/ionicons/2.0.1/css/ionicons.min.css">

</head>

<body>
    <header>
        <nav>
            <!--hamburger-->
            <div id="mySidenav" class="sidenav">
            </div>
            <span style="font-size:30px;cursor:pointer" onclick="openNav()">&#9776;</span>
            <!--brand name-->
            <label class="brand">V Veding Machine</label>
            <!--categories-->
            <div class="icons">
            </div>
        </nav>
    </header>

    <!--Pieces Section-->
    <main>
<section> 
  <div class="dropdown">
    <button class="dropbtn">Choose your allergy</button>
    <div class="dropdown-content">
      <a href="#">Allergy type 1</a>
      <a href="#">Allergy type 2</a>
      <a href="#">Allergy type 3</a>
      <a href="#">Allergy type 4</a>
    </div>
  </div>
</section>
     
<section>
  <div class="alert-bar">
    <ul class="list-inline">
      <li class="list-inline-item" id="my-button"><button onclick="setVal()">Row 1 Column 1</button></li>
      <li class="list-inline-item"><button>Row 1 Column 2</button></li>
      <li class="list-inline-item"><button>Row 2 Column 1</button></li>
      <li class="list-inline-item"><button>Row 2 Column 2</button></li>
  </ul>

  </div>
</section>
       
    </main>
   
    <!--Footer-->
    <div class="footer-basic">
        <footer>
            <p class="copyright">The Samertest V Vending Machine</p>
            <br>
            <div class="social">
                <a href="#" class="fa fa-instagram"></a>
                <a href="#" class="fa fa-facebook"></a>
                <a href="#" class="fa fa-twitter"></a>
                <a href="#" class="fa fa-pinterest"></a>

            </div>
            <ul class="list-inline">
                <li class="list-inline-item"><a href="#">SUPPORT</a></li>
                <li class="list-inline-item"><a href="#">EMAIL US</a></li>
                <li class="list-inline-item"><a href="#">SHIPPING AND FEATURES</a></li>
                <li class="list-inline-item"><a href="#">ABOUT CLUB</a></li>
                <li class="list-inline-item"><a href="#">FAQ</a></li>
            </ul>

            <p class="copyright">V Vending Machine © 2022</p>
        </footer>

        <!--firebase-->
        <script type="module">
          // Import the functions you need from the SDKs you need
          import { initializeApp } from "https://www.gstatic.com/firebasejs/9.15.0/firebase-app.js";
          import { getAnalytics } from "https://www.gstatic.com/firebasejs/9.15.0/firebase-analytics.js";
          // TODO: Add SDKs for Firebase products that you want to use
          // https://firebase.google.com/docs/web/setup#available-libraries
        
          // Your web app's Firebase configuration
          // For Firebase JS SDK v7.20.0 and later, measurementId is optional
          const firebaseConfig = {
            apiKey: "AIzaSyCnWC80a1QkyNrW5svTJTx7V3c5H-Sfujc",
            authDomain: "the-v-9d9da.firebaseapp.com",
            projectId: "the-v-9d9da",
            storageBucket: "the-v-9d9da.appspot.com",
            messagingSenderId: "680428782044",
            appId: "1:680428782044:web:7bc00921cd8123ee7dcf19",
            measurementId: "G-N4T09D9PHT"
          };
        
          // Initialize Firebase
          const app = initializeApp(firebaseConfig);
          const analytics = getAnalytics(app);

function writeData(){
var message=document.getElementById('message').value; 
database.ref('message/').push().set({
  message:message,
})
}

function getData(){
  
}

 // Get a reference to the button
 const button = document.getElementById('my-button');

// Add an event listener to the button that listens for the click event
button.addEventListener('click', function() {
  // Get the value of the button and print it in the serial monitor
  printValue(this.value);
});
function printValue(value) {
  // Print the value to the serial monitor
  Serial.println(value);
}
        </script>


<input type="text" id="message">
<button onclick="writeData()">Submit all</button>

</body>
</html>)rawliteral";

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  // Start server
  server.begin();
}
 
void loop(){  
}