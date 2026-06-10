
//ESP32Cam Sender

\#include <WiFi.h>

\#include <HTTPClient.h>



\#include <vehicle-density\_inferencing.h>

\#include "edge-impulse-sdk/dsp/image/image.hpp"

\#include "esp\_camera.h"



// =============================

// WiFi Credentials of Receiver ESP32

// =============================

const char\* ssid = "Arch";

const char\* password = "ilvpizza";



// =============================

// Camera Model

// =============================

\#define CAMERA\_MODEL\_AI\_THINKER



\#if defined(CAMERA\_MODEL\_AI\_THINKER)

\#define PWDN\_GPIO\_NUM     32

\#define RESET\_GPIO\_NUM    -1

\#define XCLK\_GPIO\_NUM      0

\#define SIOD\_GPIO\_NUM     26

\#define SIOC\_GPIO\_NUM     27



\#define Y9\_GPIO\_NUM       35

\#define Y8\_GPIO\_NUM       34

\#define Y7\_GPIO\_NUM       39

\#define Y6\_GPIO\_NUM       36

\#define Y5\_GPIO\_NUM       21

\#define Y4\_GPIO\_NUM       19

\#define Y3\_GPIO\_NUM       18

\#define Y2\_GPIO\_NUM        5

\#define VSYNC\_GPIO\_NUM    25

\#define HREF\_GPIO\_NUM     23

\#define PCLK\_GPIO\_NUM     22

\#endif



\#define EI\_CAMERA\_RAW\_FRAME\_BUFFER\_COLS 320

\#define EI\_CAMERA\_RAW\_FRAME\_BUFFER\_ROWS 240

\#define EI\_CAMERA\_FRAME\_BYTE\_SIZE 3



static bool debug\_nn = false;

static bool is\_initialised = false;

uint8\_t \*snapshot\_buf;



static camera\_config\_t camera\_config = {

&#x20;   .pin\_pwdn = PWDN\_GPIO\_NUM,

&#x20;   .pin\_reset = RESET\_GPIO\_NUM,

&#x20;   .pin\_xclk = XCLK\_GPIO\_NUM,

&#x20;   .pin\_sccb\_sda = SIOD\_GPIO\_NUM,

&#x20;   .pin\_sccb\_scl = SIOC\_GPIO\_NUM,



&#x20;   .pin\_d7 = Y9\_GPIO\_NUM,

&#x20;   .pin\_d6 = Y8\_GPIO\_NUM,

&#x20;   .pin\_d5 = Y7\_GPIO\_NUM,

&#x20;   .pin\_d4 = Y6\_GPIO\_NUM,

&#x20;   .pin\_d3 = Y5\_GPIO\_NUM,

&#x20;   .pin\_d2 = Y4\_GPIO\_NUM,

&#x20;   .pin\_d1 = Y3\_GPIO\_NUM,

&#x20;   .pin\_d0 = Y2\_GPIO\_NUM,

&#x20;   .pin\_vsync = VSYNC\_GPIO\_NUM,

&#x20;   .pin\_href = HREF\_GPIO\_NUM,

&#x20;   .pin\_pclk = PCLK\_GPIO\_NUM,



&#x20;   .xclk\_freq\_hz = 20000000,

&#x20;   .ledc\_timer = LEDC\_TIMER\_0,

&#x20;   .ledc\_channel = LEDC\_CHANNEL\_0,



&#x20;   .pixel\_format = PIXFORMAT\_JPEG,

&#x20;   .frame\_size = FRAMESIZE\_QVGA,

&#x20;   .jpeg\_quality = 12,

&#x20;   .fb\_count = 1,

&#x20;   .fb\_location = CAMERA\_FB\_IN\_PSRAM,

&#x20;   .grab\_mode = CAMERA\_GRAB\_WHEN\_EMPTY,

};



bool ei\_camera\_init(void);

bool ei\_camera\_capture(uint32\_t img\_width, uint32\_t img\_height, uint8\_t \*out\_buf);

static int ei\_camera\_get\_data(size\_t offset, size\_t length, float \*out\_ptr);



void setup() {

&#x20; delay(3000);

&#x20; Serial.begin(115200);

&#x20; delay(1000);

&#x20; 



&#x20; Serial.println("Program Started");

&#x20; Serial.print("PSRAM Size: ");

&#x20; Serial.println(ESP.getPsramSize());

Serial.println();

Serial.println("ESP32-CAM Sender Starting");



&#x20; if (!ei\_camera\_init()) {

&#x20;   Serial.println("Camera init failed");

&#x20;   return;

&#x20; }



&#x20; Serial.println("Camera initialized");



&#x20; WiFi.begin(ssid, password);



&#x20; Serial.print("Connecting to Receiver WiFi");

&#x20; while (WiFi.status() != WL\_CONNECTED) {

&#x20;   delay(500);

&#x20;   Serial.print(".");

&#x20; }



&#x20; Serial.println();

&#x20; Serial.println("Connected to Receiver WiFi");

&#x20; Serial.print("Sender IP: ");

&#x20; Serial.println(WiFi.localIP());

}



void loop() {



&#x20; snapshot\_buf = (uint8\_t\*)malloc(EI\_CAMERA\_RAW\_FRAME\_BUFFER\_COLS \* EI\_CAMERA\_RAW\_FRAME\_BUFFER\_ROWS \* EI\_CAMERA\_FRAME\_BYTE\_SIZE);



&#x20; if(snapshot\_buf == nullptr) {

&#x20;   Serial.println("Failed to allocate image buffer");

&#x20;   delay(3000);

&#x20;   return;

&#x20; }



&#x20; ei::signal\_t signal;

&#x20; signal.total\_length = EI\_CLASSIFIER\_INPUT\_WIDTH \* EI\_CLASSIFIER\_INPUT\_HEIGHT;

&#x20; signal.get\_data = \&ei\_camera\_get\_data;



&#x20; if (!ei\_camera\_capture(EI\_CLASSIFIER\_INPUT\_WIDTH, EI\_CLASSIFIER\_INPUT\_HEIGHT, snapshot\_buf)) {

&#x20;   Serial.println("Image capture failed");

&#x20;   free(snapshot\_buf);

&#x20;   delay(3000);

&#x20;   return;

&#x20; }



&#x20; ei\_impulse\_result\_t result = { 0 };



&#x20; EI\_IMPULSE\_ERROR err = run\_classifier(\&signal, \&result, debug\_nn);



&#x20; if (err != EI\_IMPULSE\_OK) {

&#x20;   Serial.println("Classifier failed");

&#x20;   free(snapshot\_buf);

&#x20;   delay(3000);

&#x20;   return;

&#x20; }



&#x20; float max\_value = 0.0;

&#x20; int max\_index = 0;



&#x20; for (uint16\_t i = 0; i < EI\_CLASSIFIER\_LABEL\_COUNT; i++) {

&#x20;   float value = result.classification\[i].value;



&#x20;   Serial.print(ei\_classifier\_inferencing\_categories\[i]);

&#x20;   Serial.print(": ");

&#x20;   Serial.println(value);



&#x20;   if (value > max\_value) {

&#x20;     max\_value = value;

&#x20;     max\_index = i;

&#x20;   }

&#x20; }



&#x20; const char\* detected\_label = ei\_classifier\_inferencing\_categories\[max\_index];



&#x20; float density\_weight = 1.0;



if (strcmp(detected\_label, "Medium\_vehicleDensity") == 0) {

&#x20; density\_weight = 2.0;

}

else if (strcmp(detected\_label, "High\_vehicleDensity") == 0) {

&#x20; density\_weight = 3.0;

}



&#x20; int estimated\_time = (int)(density\_weight \* max\_value \* 30.0);



&#x20; if (estimated\_time < 5) estimated\_time = 5;

&#x20; if (estimated\_time > 60) estimated\_time = 60;



&#x20; Serial.println();

&#x20; Serial.print("Detected Density: ");

&#x20; Serial.println(detected\_label);

&#x20; Serial.print("Confidence: ");

&#x20; Serial.println(max\_value);

&#x20; Serial.print("Estimated Green Time: ");

&#x20; Serial.print(estimated\_time);

&#x20; Serial.println(" sec");



&#x20; if (WiFi.status() == WL\_CONNECTED) {

&#x20;   HTTPClient http;



&#x20;   //String url = "http://192.168.4.1/update?time=" + String(estimated\_time);



&#x20;   String url = "http://192.168.4.1/update?density=" + String(detected\_label) + 

&#x20;            "\&confidence=" + String(max\_value, 3) + 

&#x20;            "\&time=" + String(estimated\_time);



&#x20;   Serial.print("Sending URL: ");

&#x20;   Serial.println(url);



&#x20;   http.begin(url);



&#x20;   int httpCode = http.GET();



&#x20;   if (httpCode > 0) {

&#x20;     Serial.print("HTTP Response Code: ");

&#x20;     Serial.println(httpCode);



&#x20;     String response = http.getString();

&#x20;     Serial.println(response);

&#x20;   }

&#x20;   else {

&#x20;     Serial.print("HTTP Request Failed: ");

&#x20;     Serial.println(httpCode);

&#x20;   }



&#x20;   http.end();

&#x20; }



&#x20; free(snapshot\_buf);



&#x20; delay(10000);

}



bool ei\_camera\_init(void) {



&#x20; if (is\_initialised) return true;



&#x20; esp\_err\_t err = esp\_camera\_init(\&camera\_config);



&#x20; if (err != ESP\_OK) {

&#x20;   Serial.printf("Camera init failed with error 0x%x\\n", err);

&#x20;   return false;

&#x20; }



&#x20; is\_initialised = true;

&#x20; return true;

}



bool ei\_camera\_capture(uint32\_t img\_width, uint32\_t img\_height, uint8\_t \*out\_buf) {



&#x20; if (!is\_initialised) {

&#x20;   Serial.println("Camera not initialized");

&#x20;   return false;

&#x20; }



&#x20; camera\_fb\_t \*fb = esp\_camera\_fb\_get();



&#x20; if (!fb) {

&#x20;   Serial.println("Camera capture failed");

&#x20;   return false;

&#x20; }



&#x20; bool converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT\_JPEG, snapshot\_buf);



&#x20; esp\_camera\_fb\_return(fb);



&#x20; if (!converted) {

&#x20;   Serial.println("Image conversion failed");

&#x20;   return false;

&#x20; }



&#x20; if ((img\_width != EI\_CAMERA\_RAW\_FRAME\_BUFFER\_COLS) || (img\_height != EI\_CAMERA\_RAW\_FRAME\_BUFFER\_ROWS)) {

&#x20;   ei::image::processing::crop\_and\_interpolate\_rgb888(

&#x20;     out\_buf,

&#x20;     EI\_CAMERA\_RAW\_FRAME\_BUFFER\_COLS,

&#x20;     EI\_CAMERA\_RAW\_FRAME\_BUFFER\_ROWS,

&#x20;     out\_buf,

&#x20;     img\_width,

&#x20;     img\_height

&#x20;   );

&#x20; }



&#x20; return true;

}



static int ei\_camera\_get\_data(size\_t offset, size\_t length, float \*out\_ptr) {



&#x20; size\_t pixel\_ix = offset \* 3;

&#x20; size\_t pixels\_left = length;

&#x20; size\_t out\_ptr\_ix = 0;



&#x20; while (pixels\_left != 0) {

&#x20;   out\_ptr\[out\_ptr\_ix] = (snapshot\_buf\[pixel\_ix + 2] << 16) + (snapshot\_buf\[pixel\_ix + 1] << 8) + snapshot\_buf\[pixel\_ix];



&#x20;   out\_ptr\_ix++;

&#x20;   pixel\_ix += 3;

&#x20;   pixels\_left--;

&#x20; }



&#x20; return 0;

}
