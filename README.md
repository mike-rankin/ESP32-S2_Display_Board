# ESP32-S2_Display_Board
ESP32-S2 1.54" x 1.54" TFT Display Board

![Front](https://user-images.githubusercontent.com/4991664/145102131-17a9757d-165e-4482-8819-c4992952e86b.JPG)
![Back](https://user-images.githubusercontent.com/4991664/145102137-908d4c00-e192-4ad1-9886-9b82323de95c.jpg)

This ESP32 internet of things device was an experiment to try out the new ESP32-S2 processor from Espressif. It is a cheaper option to the ESP32 Pico D4 that I've been using for years. One big advantage is built in USB interface chip with a disadvantage of requiring and external 40Mhz crystal. The Pico D4 had a built in crystal. Connecting other I2C devices is possible using the QwiiC header and larger power supply. Instead of a 500mA LDO, this board has a 1amp switching power supply. The only sensors on this board is an ambiely light sensor on a small tab beside the display and a temperature/humidity sensor that is thermally isolated as much as possible on the other side of the tab.

Hardware on this board:<br/> 
-ESP32-S2FH2 processor<br/> 
-SHTC3-TR-2.5KS temperature/humidity sensor<br/>
-BH1750FVI -ambient light sensor<br/>
-ER-TFT1.54-1 240x240 TFT LCD<br/>
-RT8096CHGJ5 3.3V Switching Power Supply<br/>
-MCP73831T-2ACI LiPO Battery Charger<br/>
-XB5353A LiPo Battery Protection<br/>
-QwiiC i2c Connector<br/>
