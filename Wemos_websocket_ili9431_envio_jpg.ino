/* Ejemplo de comunicación WebSocket Servidor <---> Cliente. Escrito por Dani No www.esploradores.com
Este sofware está escrito bajo la licencia CREATIVE COMMONS con Reconocimiento - CompartirIgual
(CC BY-SA) https://creativecommons.org/
-Redistributions of source code must retain the above creative commons and this list of conditions 
 and the following disclaimer.
-Redistributions in binary form must reproduce the above creative commons notice, this list of 
 conditions and the following disclaimer in the documentation and/or other materials provided 
 with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN 
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <FS.h>
// JPEG decoder library
#include <JPEGDecoder.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

const char* imageFileName = "/camimage.jpg";

bool useStaticIP = false;
//const char* ssid = "Celular Andres Trabajo";
//const char* password = "Agustina";
const char* ssid = "Fibertel WiFi195 2.4GHz";
const char* password = "0146659536";

IPAddress IP(192,168,0,201);     
IPAddress gateway(192,168,0,1);   
IPAddress subnet(255,255,255,0);   


static const char PROGMEM INDEX_HTML[] = R"( 
<!DOCTYPE html>
<html class="no-js" lang="">
<head>
    <meta charset="utf-8">
    <meta http-equiv="x-ua-compatible" content="ie=edge">
    <title>ESP8266 WebSockets</title>
    <meta name="description" content="">
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>

<body>
    <h1>Elegir Archivo</h1>
</body>

<input type="file" id="files" name="files[]" multiple />
<output id="list"></output>
<p id='String File'></p>
<input type="text" id='miValor'>
<input type="text" id='miValor2'>
<script>
    var arrLen;
    var intBuffer = 200;
    function convertFromHex(hex) {
      var hex = hex.toString();//force conversion
      var str = '';
      for (var i = 0; i < hex.length; i += 2)
          str += String.fromCharCode(parseInt(hex.substr(i, 2), 16));
      return str;
    }

    function convertToHex(str) {
      var hex = '';
      for(var i=0;i<str.length;i++) {
          hex += ''+str.charCodeAt(i).toString(16);
      }
      return hex;
    }

    function handleFileSelect(evt) {
        var files = evt.target.files; // FileList object

        // Loop through the FileList and render image files as thumbnails.
        for (var i = 0, f; f = files[i]; i++) {

            // Only process image files.
            if (!f.type.match('image.*')) {
                continue;
            }

            var reader_bmp = new FileReader();
            var reader = new FileReader();
            var rawData = new ArrayBuffer();
            var rawBuffer = new ArrayBuffer();
            var j = 0;
            var rawString = '';
            var rawByte = '';

            // Closure to capture the file information.
            reader_bmp.onload = (function (theFile) {
                return function (e) {
                    // Render thumbnail.
                    rawData = e.target.result;
                    var span = document.createElement('span');
                    span.innerHTML = ['<img class="thumb" src="', rawData,
                        '" title="', escape(theFile.name), '"/>'].join('');
                    document.getElementById('list').insertBefore(span, null);
                };
            })(f);

            // Read in the image file as a data URL.
            var x = reader_bmp.readAsDataURL(f);
            //var x = reader.readAsBinaryString(f);

            // Closure to capture the file information.
            reader.onload = (function (theFile) {
                return function (e) {
                    // Render thumbnail.
                    rawData = e.target.result;
                    //var span = document.createElement('span');
                    //span.innerHTML = ['<img class="thumb" src="', rawData,
                    //    '" title="', escape(theFile.name), '"/>'].join('');
                    //document.getElementById('list').insertBefore(span, null);
                };
            })(f);

            // Read in the image file as a data URL.
            //var x = reader.readAsDataURL(f);
            var x = reader.readAsBinaryString(f);
            //var x = reader.readAsText(f);
            //var x = reader.readAsArrayBuffer(f);
            //var x = document.getElementById('miValor').value;
            document.getElementById('miValor').value = 'Por conectarse';
            var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
            var strSend = '00';
            connection.onopen = function () {
                connection.send('<BMP>');
                arrLen = rawData.length;
                document.getElementById('miValor').value = 'Iniciando el pasaje al buffer' + arrLen;
                document.getElementById('miValor2').value = '';
                for (var i = 0; i < arrLen; i++) {
                    rawByte = rawData[i];
                    strSend = convertToHex(rawByte);
                    if (strSend == '0'){
                      strSend = '00';
                    }
                    if (strSend == '1'){
                      strSend = '01';
                    }
                    if (strSend == '2'){
                      strSend = '02';
                    }
                    if (strSend == '3'){
                      strSend = '03';
                    }
                    if (strSend == '4'){
                      strSend = '04';
                    }
                    if (strSend == '5'){
                      strSend = '05';
                    }
                    if (strSend == '6'){
                      strSend = '06';
                    }
                    if (strSend == '7'){
                      strSend = '07';
                    }
                    if (strSend == '8'){
                      strSend = '08';
                    }
                    if (strSend == '9'){
                      strSend = '09';
                    }
                    if (strSend == 'a'){
                      strSend = '0a';
                    }
                    if (strSend == 'b'){
                      strSend = '0b';
                    }
                    if (strSend == 'c'){
                      strSend = '0c';
                    }
                    if (strSend == 'd'){
                      strSend = '0d';
                    }
                    if (strSend == 'e'){
                      strSend = '0e';
                    }
                    if (strSend == 'f'){
                      strSend = '0f';
                    }
                    if (i <= 53){
                      //document.getElementById('miValor2').value = document.getElementById('miValor2').value + ',' + strSend;
                      document.getElementById('miValor2').value = rawString;
                    }
                    rawString = rawString + strSend;
                    if (j == 2500) {
                       j = 0;
                       connection.send(rawString);
                       rawString = '';
                    }
                    j++;
                }
                //connection.send(rawData);
                if (j != 0) {
                  j = 0;
                  connection.send(rawString);
                  rawString = '';
                }    //connection.send(rawData[i]);
                connection.send('<END>');
                //connection.send(rawData);
               document.getElementById('miValor').value = 'Pase la coneccion :' + arrLen;
            }
        }
        connection.onmessage = function (event) {
            console.log('Servidor (recibe): ', event.data);
        }
        connection.onerror = function (error) {
            console.log('WebSocket Error!!!', error);
            document.getElementById('miValor').value = 'Websocket Error!!!';
        }
        document.getElementById('miValor').value = 'LLegue al Final' + arrLen;
    }


    document.getElementById('files').addEventListener('change', handleFileSelect, false);
</script>
</html>
)";

int tamRAW = 0;
int numFile = 0;
boolean isSaved = false;
uint8_t * archivo;
int archLon = 0;
File f;

ESP8266WebServer server (80);
WebSocketsServer webSocket = WebSocketsServer(8888);


boolean fileWrite(String name, uint8_t * content, int longitud){
  Serial.print(longitud);
  Serial.print(content[0],HEX);
  Serial.print(content[1],HEX);
  Serial.print(content[2],HEX);
  Serial.print(content[3],HEX);
  File file = SPIFFS.open(imageFileName, "w");
  if(!file){
    String errorMsg = "Can't open file";
    Serial.println(errorMsg);
    return false;
  }else{
    file.write(content,longitud);
    file.close();
    return true;
  }
}


boolean fileOpenWrite(String name, uint8_t * content, int longitud, char * mode){
  Serial.print(longitud);
  Serial.print(content[0],HEX);
  Serial.print(content[1],HEX);
  Serial.print(content[2],HEX);
  Serial.print(content[3],HEX);
  File file = SPIFFS.open(imageFileName, mode);
  if(!file){
    String errorMsg = "Can't open file";
    Serial.println(errorMsg);
    return false;
  }else{
    file.write(content,longitud);
    file.close();
    return true;
  }
}

#define BUFFPIXEL 20
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);
  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  //tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}

/*====================================================================================
  This sketch contains support functions to render the Jpeg images.

  Created by Bodmer 15th Jan 2017
  ==================================================================================*/

// Return the minimum of two values a and b
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

//====================================================================================
//   Print information decoded from the Jpeg image
//====================================================================================
void jpegInfo() {

  Serial.println("===============");
  Serial.println("JPEG image info");
  Serial.println("===============");
  Serial.print  ("Width      :"); Serial.println(JpegDec.width);
  Serial.print  ("Height     :"); Serial.println(JpegDec.height);
  Serial.print  ("Components :"); Serial.println(JpegDec.comps);
  Serial.print  ("MCU / row  :"); Serial.println(JpegDec.MCUSPerRow);
  Serial.print  ("MCU / col  :"); Serial.println(JpegDec.MCUSPerCol);
  Serial.print  ("Scan type  :"); Serial.println(JpegDec.scanType);
  Serial.print  ("MCU width  :"); Serial.println(JpegDec.MCUWidth);
  Serial.print  ("MCU height :"); Serial.println(JpegDec.MCUHeight);
  Serial.println("===============");
  Serial.println("");
}

//====================================================================================
//   Decode and render the Jpeg image onto the TFT screen
//====================================================================================
void jpegRender(int xpos, int ypos) {

  // retrieve infomration about the image
  uint16_t  *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while ( JpegDec.read()) {

    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      for (int h = 1; h < win_h-1; h++)
      {
        memcpy(pImg + h * win_w, pImg + (h + 1) * mcu_w, win_w << 1);
      }
    }

    // draw image MCU block only if it will fit on the screen
    if ( ( mcu_x + win_w) <= tft.width() && ( mcu_y + win_h) <= tft.height())
    {
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg); // esto se usa para TFT_eSPI
      //tft.drawRGBBitmap(mcu_x, mcu_y, pImg, win_w, win_h);
    }

    else if ( ( mcu_y + win_h) >= tft.height()) JpegDec.abort();

  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime; // Calculate the time it took

  // print the results to the serial port
  Serial.print  ("Total render time was    : "); Serial.print(drawTime); Serial.println(" ms");
  Serial.println("=====================================");

}


//====================================================================================
//   Opens the image file and prime the Jpeg decoder
//====================================================================================
void drawJpeg(const char *filename, int xpos, int ypos) {

  Serial.println("===========================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("===========================");

  // Open the named file (the Jpeg decoder library will close it after rendering image)
  fs::File jpegFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
  //File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
 
  if ( !jpegFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  // Use one of the three following methods to initialise the decoder:
  boolean decoded = JpegDec.decodeFsFile(jpegFile); // Pass a SPIFFS file handle to the decoder,
  //boolean decoded = JpegDec.decodeSdFile(jpegFile); // or pass the SD file handle to the decoder,
  //boolean decoded = JpegDec.decodeFsFile(filename);  // or pass the filename (leading / distinguishes SPIFFS files)
                                   // Note: the filename can be a String or character array type
  if (decoded) {
    // print information about the image to the serial port
    jpegInfo();

    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}
//====================================================================================
//   Open a Jpeg file and send it to the Serial port in a C array compatible format
//====================================================================================
void createArray(const char *filename) {

  // Open the named file
  fs::File jpgFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
  //File jpgFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
 
  if ( !jpgFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  uint8_t data;
  byte line_len = 0;
  Serial.println("");
  Serial.println("// Generated by a JPEGDecoder library example sketch:");
  Serial.println("// https://github.com/Bodmer/JPEGDecoder");
  Serial.println("");
  Serial.println("#if defined(__AVR__)");
  Serial.println("  #include <avr/pgmspace.h>");
  Serial.println("#endif");
  Serial.println("");
  Serial.print  ("const uint8_t ");
  while (*filename != '.') Serial.print(*filename++);
  Serial.println("[] PROGMEM = {"); // PROGMEM added for AVR processors, it is ignored by Due

  while ( jpgFile.available()) {

    data = jpgFile.read();
    Serial.print("0x"); if (abs(data) < 16) Serial.print("0");
    Serial.print(data, HEX); Serial.print(",");// Add value and comma
    line_len++;
    if ( line_len >= 32) {
      line_len = 0;
      Serial.println();
    }

  }

  Serial.println("};\r\n");
  jpgFile.close();
}
//====================================================================================


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Conectado a la URL: %d.%d.%d.%d - %s\n", num, ip[0], ip[1], ip[2], ip[3], payload); 
      }
      break;
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Desconectado!\n", num);
      break;
    case WStype_BIN: //WStype_TEXT:
      //webSocket.sendTXT(num, payload);      //Devuelve el mensaje recibido al cliente
      //String str = (char*)payload;
      int longitud = length;
      if (longitud == 10000) {
        Serial.print(longitud);
        isSaved = false;
        if (archLon == 0){
          isSaved = fileOpenWrite(imageFileName, payload, longitud, "w");
          isSaved = false;
        }
        else {
          isSaved = fileOpenWrite(imageFileName, payload, longitud, "a");
          isSaved = false;
        }
        archLon = archLon + longitud;
      }
      else {
        if (archLon == 0){
          isSaved = fileOpenWrite(imageFileName, payload, longitud, "w");
        }
        else {
          isSaved = fileOpenWrite(imageFileName, payload, longitud, "a");
        }
        archLon = archLon + longitud;
        Serial.print(archLon);
        //isSaved = fileWrite(imageFileName, archivo, archLon);
        archLon = 0;
        Serial.print(isSaved);
      }
      Serial.print("Mostrando JPG");
      if(isSaved){
        drawJpeg(imageFileName, 0, 0);        // draw it
      }
      break;
  }
}

  
void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
 

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(D3, OUTPUT); //HSPI SS
  Serial.println("Ejemplo de SPIFFS muy básico, escribe 10 líneas en el sistema de archivos y luego las lee\n");
  SPIFFS.begin();
  //Las siguientes líneas se deben hacer SOLAMENTE UNA VEZ !!!!!
  //Cuando SPIFFS está formateado UNA VEZ, puede comentar estas líneas!!
  Serial.println("Espere 30 segundos para que se formatee el SPIFFS"); 
  SPIFFS.format();
  Serial.println("Spiffs formateado"); 
 
  // cambio de direccion
  WiFi.mode(WIFI_STA);
  WiFi.config(IP, gateway, subnet);
  WiFi.begin(ssid, password);
  
//  if(useStaticIP) WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) 
  { 
    delay(100);  
    Serial.print('.'); 
  }

  Serial.println("");
  Serial.print("Iniciado STA:\t");
  Serial.println(ssid);
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  //WiFi.softAP(ssid, password);
  //IPAddress myIP = WiFi.softAPIP(); 
  IPAddress myIP = WiFi.localIP();
  
  Serial.print("IP del access point: ");
  Serial.println(myIP);

  Serial.println("Arrancando el display");
  // OR use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  //digitalWrite(D3,HIGH); // Arranco el BL del display
  tft.init();         
  tft.fillScreen(TFT_BLUE);
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess) importantisimo para que funcione el jpg si no se pone queda distorsionado
  tft.setRotation(1);
  tft.print(myIP);
  
   // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  // handle index
  server.on("/", []() {
      server.send_P(200, "text/html", INDEX_HTML);
  });
  server.begin();
  Serial.println("WebServer iniciado...");
  //pinMode(LED, OUTPUT); 
  //digitalWrite(LED, 0); 
  //analogWriteRange(255);
}
void loop() {
    webSocket.loop();
    server.handleClient();
}
