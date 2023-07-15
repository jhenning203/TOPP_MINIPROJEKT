/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-cam-projects-ebook/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h"             // disable brownout problems
#include "soc/rtc_cntl_reg.h"    // disable brownout problems
#include "esp_http_server.h"

// Replace with your network credentials
const char* ssid = "Topp";
const char* password = "MegaTopp";

#define PART_BOUNDARY "123456789000000000000987654321"

#define CAMERA_MODEL_AI_THINKER





  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22


static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
    <style>
    body{
      background-color: #888888;
    }


    .slidecontainer {
      width: 100%;
    }
    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 15px;
      border-radius: 5px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
      
    }
    .slider:hover {
      opacity: 1;
      
    }
  
    .slider::-webkit-slider-thumb {
      
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: red;
      cursor: pointer;
    }
    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: red;
      cursor: pointer;
      
    }


.circle-container {
  
  position: relative;
  width: 20em;
  height: 20em;
  border-radius: 50%;
  padding: 0;
  list-style: none;
  margin-top: 1.5em;
  left:3em;

 
}
.circle-container > * {
  display: block;
  position: absolute;
  top: 50%;
  left: 50%;
  margin: -3em;
  width: 6em;
  height: 6em;
}
.circle-element {
  background-color: rgb(0, 0, 0) !important;
  color:aliceblue;
}
.button-text {
  position: relative;
  top: 1em;
  
  text-align: center;
}
.circle-container > *:nth-of-type(1) {
  transform: rotate(10deg) translate(10em) rotate(-10deg);
}
.circle-container > *:nth-of-type(2) {
  transform: rotate(30deg) translate(9.4em) rotate(-30deg);
}
.circle-container > *:nth-of-type(3) {
  transform: rotate(50deg) translate(10em) rotate(-50deg);
}
.circle-container > *:nth-of-type(4) {
  transform: rotate(70deg) translate(10em) rotate(-70deg);
}
.circle-container > *:nth-of-type(5) {
  transform: rotate(90deg) translate(9.4em) rotate(-90deg);
}
.circle-container > *:nth-of-type(6) {
  transform: rotate(110deg) translate(10em) rotate(-110deg);
}
.circle-container > *:nth-of-type(7) {
  transform: rotate(130deg) translate(10em) rotate(-130deg);
}
.circle-container > *:nth-of-type(8) {
  transform: rotate(150deg) translate(9.4em) rotate(-150deg);
}
.circle-container > *:nth-of-type(9) {
  transform: rotate(170deg) translate(10em) rotate(-170deg);
}
.circle-container > *:nth-of-type(10) {
  transform: rotate(190deg) translate(10em) rotate(-190deg);
}
.circle-container > *:nth-of-type(11) {
  transform: rotate(210deg) translate(9.4em) rotate(-210deg);
}
.circle-container > *:nth-of-type(12) {
  transform: rotate(230deg) translate(10em) rotate(-230deg);
}
.circle-container > *:nth-of-type(13) {
  transform: rotate(250deg) translate(10em) rotate(-250deg);
}
.circle-container > *:nth-of-type(14) {
  transform: rotate(270deg) translate(9.4em) rotate(-270deg);
}
.circle-container > *:nth-of-type(15) {
  transform: rotate(290deg) translate(10em) rotate(-290deg);
}
.circle-container > *:nth-of-type(16) {
  transform: rotate(310deg) translate(10em) rotate(-310deg);
}
.circle-container > *:nth-of-type(17) {
  transform: rotate(330deg) translate(9.4em) rotate(-330deg);
}
.circle-container > *:nth-of-type(18) {
  transform: rotate(350deg) translate(10em) rotate(-350deg);
}

.circle-container > *:nth-of-type(24) {
  transform: rotate(60deg) translate(4em) rotate(-60deg);
}
.circle-container > *:nth-of-type(19) {
  transform: rotate(120deg) translate(4em) rotate(-120deg);
}
.circle-container > *:nth-of-type(20) {
  transform: rotate(180deg) translate(4em) rotate(-180deg);
}
.circle-container > *:nth-of-type(21) {
  transform: rotate(240deg) translate(4em) rotate(-240deg);
}
.circle-container > *:nth-of-type(22) {
  transform: rotate(300deg) translate(4em) rotate(-300deg);
}
.circle-container > *:nth-of-type(23) {
  transform: rotate(0deg) translate(4em) rotate(-0deg);
}



.circle-container div {
  display: block;
  width: 3.4em;
  height: 3.4em;
  margin-top:1.2em;
  margin-left:1.2em;
  top:2em;
  border-radius: 50%;
  
  background-color: rgb(255, 123, 71);

  transition: .15s;
}

    </style>
  
  </head>
  <body class="noselect" align="center" >





    <table id="mainTable" style="width:90%;margin:none;table-layout:fixed" CELLSPACING=0>






      
      <tr>
        <!--<img id="cameraImage" src="" style="width:400px;height:250px"></td>-->
        <td>
          <img src="" id="photo" style="width:400px;height:250px">
          <!--<div id="console" style="width:400px;height:250px;border: solid 5px black; overflow-y:scroll;" ></div>-->
        </td>
      </tr> 
      <tr>
        <td style="text-align:left"><b>Speed:</b></td>
        <td colspan=2>
         <div class="slidecontainer">
            <input type="range" min="65" max="90" value="150" class="slider" id="Speed" oninput='updateSlider("s",value)'>
          </div>
        </td>
      </tr>        
      <tr>
        <td style="text-align:left"><b>Light:</b></td>
        <td colspan=2>
          <div class="slidecontainer">
            <input type="range" min="65" max="90" value="0" class="slider" id="Light" oninput='updateSlider("k",value)'>
          </div>
        </td>   
      </tr>
      <tr>
        <td>
      <ul class='circle-container'>
        <li><div id="1" class="circle-element" ontouchstart="init_click('1');" ontouchend="exit_click('1');"><p class="button-text">1</p></div></li>
        <li><div id="2" class="circle-element" ontouchstart="init_click('2');" ontouchend="exit_click('2');"><p class="button-text">2</p></div></li>
        <li><div id="3" class="circle-element" ontouchstart="init_click('3');" ontouchend="exit_click('3');"><p class="button-text">3</p></div></li>
        <li><div id="4" class="circle-element" ontouchstart="init_click('4');" ontouchend="exit_click('4');"><p class="button-text">4</p></div></li>
        <li><div id="5" class="circle-element" ontouchstart="init_click('5');" ontouchend="exit_click('5');"><p class="button-text">5</p></div></li>
        <li><div id="6" class="circle-element" ontouchstart="init_click('6');" ontouchend="exit_click('6');"><p class="button-text">6</p></div></li>
        <li><div id="7" class="circle-element" ontouchstart="init_click('7');" ontouchend="exit_click('7');"><p class="button-text">7</p></div></li>
        <li><div id="8" class="circle-element" ontouchstart="init_click('8');" ontouchend="exit_click('8');"><p class="button-text">8</p></div></li>
        <li><div id="9" class="circle-element" ontouchstart="init_click('9');" ontouchend="exit_click('9');"><p class="button-text">9</p></div></li>
        <li><div id="a" class="circle-element" ontouchstart="init_click('a');" ontouchend="exit_click('a');"><p class="button-text">a</p></div></li>
        <li><div id="b" class="circle-element" ontouchstart="init_click('b');" ontouchend="exit_click('b');"><p class="button-text">b</p></div></li>
        <li><div id="c" class="circle-element" ontouchstart="init_click('c');" ontouchend="exit_click('c');"><p class="button-text">c</p></div></li>
        <li><div id="d" class="circle-element" ontouchstart="init_click('d');" ontouchend="exit_click('d');"><p class="button-text">d</p></div></li>
        <li><div id="e" class="circle-element" ontouchstart="init_click('e');" ontouchend="exit_click('e');"><p class="button-text">e</p></div></li>
        <li><div id="f" class="circle-element" ontouchstart="init_click('f');" ontouchend="exit_click('f');"><p class="button-text">f</p></div></li>
        <li><div id="g" class="circle-element" ontouchstart="init_click('g');" ontouchend="exit_click('g');"><p class="button-text">g</p></div></li>
        <li><div id="h" class="circle-element" ontouchstart="init_click('h');" ontouchend="exit_click('h');"><p class="button-text">h</p></div></li>
        <li><div id="0" class="circle-element" ontouchstart="init_click('0');" ontouchend="exit_click('0');"><p class="button-text">0</p></div></li>
      
        <li><div id="p" class="circle-inner-element" ontouchstart="init_click('p');" ontouchend="exit_click('p');"><p class="button-text">Drop</p></div></li>
        <li><div id="m" class="circle-inner-element" ontouchstart="toggleAuto()" ><p class="button-text">Auto</p></div></li>
        <li><div id="r" class="circle-inner-element" ontouchstart="init_click('r');" ontouchend="exit_click('r');" ><p class="button-text">Right</p></div></li>
        <li><div id="l" class="circle-inner-element" ontouchstart="init_click('l');" ontouchend="exit_click('l');" ><p class="button-text">Left</p></div></li>
        <li><div id="v" class="circle-inner-element" ontouchstart="toggleCam()" ><p class="button-text">Cam</p></div></li>
        <li><div id="w" class="circle-inner-element" ontouchstart="init_click('w');" ontouchend="exit_click('w');"><p class="button-text">Spit</p></div></li>
      </ul>
    </td>
    </tr>

     
    </table>
  
    <script>
      var interval;
      var interval_active = false;
      var last_command = "";
      var enabled_cam = false;
      var enabled_auto = false

      function loop(){
        //log(last_command)
        write_command(last_command)
        //var strBytes = last_command.charCodeAt(0)
        //console.log(strBytes)
      }
      
      function init_click(command){
        document.getElementById(command).style.backgroundColor = "green"
        last_command = command
        if(!interval_active){
          interval_active = true
          loop()
          interval = setInterval(loop, 300)
        }
        
      }

      function exit_click(command){
        document.getElementById(command).style.backgroundColor = "orange"
        interval_active = false
        clearInterval(interval)
      }

      function updateSlider(type, val){
        byte_val = String.fromCharCode(val)
        last_command = type+byte_val
        loop()
      }
      function toggleCam(){
        if(enabled_cam){
          enabled_cam = false
          last_command = "v0"
          document.getElementById("v").style.backgroundColor = "orange"
        }
        else{
          enabled_cam = true
          last_command = "v1"
          document.getElementById("v").style.backgroundColor = "green"
        }
        loop()
      }
      function toggleAuto(){
        if(enabled_auto){
          enabled_auto = false
          last_command = "m0"
          document.getElementById("m").style.backgroundColor = "orange"
        }
        else{
          enabled_auto = true
          last_command = "m1"
          document.getElementById("m").style.backgroundColor = "green"
        }
        loop()
      }




      function log(text){
        document.getElementById("console").innerHTML += text+"<br>"
      }
      
      function write_command(command) {
        console.log(x)
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/action?go=" + command, true);
        xhr.send();
      }
      
      window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
      
    
    </script>
  </body>    
</html>
)rawliteral";

static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
    //Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}

static esp_err_t cmd_handler(httpd_req_t *req){
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
  
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf){
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  sensor_t * s = esp_camera_sensor_get();
  int res = 0;
  
  if(strlen(variable)>0) {
    Serial.println(variable);
  }

  else {
    res = -1;
  }

  if(res){
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

void startCameraServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri       = "/action",
    .method    = HTTP_GET,
    .handler   = cmd_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };
  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
  }
  config.server_port += 1;
  config.ctrl_port += 1;
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
  
  // Start streaming web server
  startCameraServer();
}

void loop() {
  
}
