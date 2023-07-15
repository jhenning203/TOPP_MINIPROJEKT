# How to upload the Code
- Add repo to arduino ide
  - https://dl.espressif.com/dl/package_esp32_index.json
  - http://arduino.esp8266.com/stable/package_esp8266com_index.json
- install esp32 board
- select the board
- Setup
  - choose board esp32 wrover module
  - 40MHz
  - Fash Mode QIO
  - Partition Scheme: Huge App
- Correctly Wire it
![grafik](https://github.com/jhenning203/TOPP_MINIPROJEKT/assets/137420847/bfbc74fd-19a8-4783-be0b-cef6e3bae50e)
- upload the code.
- Unplug the power to reset the ESP
- remove the reset wire from esp
- Press reset buttons
# What does it do
- The code provides a website and sends a videostream to it
- The Signals recieved from the Website are transfered to the arduino
- to connect open a hotspot on your smartphone
  - name:  Topp
  - password: MegaTopp
- The code outputs the ip of the esp via Serial monitor (default: 1.)
- enter the ip address in your browser
- use the web interface   
