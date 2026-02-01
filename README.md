T![Bild_1_Fertiges Gerät](https://github.com/user-attachments/assets/b4304353-8f43-4a9e-9f8c-ff6eb5268660)

The circuits "pumps" 12V pulsed voltage into your 1.5 V alkaline batteryuntil a threshold voltage is reached. This process is controlled by a ESP32 microprocessor. In this case an ESP32 D1 mini is used. 

An Arduino sketch with one main-program and four subfunctions is available here. The project is based on home assistant. Home Assistant is necessary for the MQTT-comunication and as display for the whole process. 
You may kick out this part of the sketch and use it without MQTT-communication and home assistant integration.
Be sure to edit the WiFi and MQTT-credentials according to your needs.

KiCAD-schematic and a proven PCB-layout are provided,too.

Standard operation is like this:

- you insert up to 4 AA batteries
- the acccording LEDs begin to flash and you see on your Home Assistant dashboard, that it shows the actual voltage of your batteries.
- fresh up takes place and stops, as soon as 1.65 volts are reached for a least 10 times. You may go for another threshold voltage, if you like.

- Some really weak batteries will never show 1.65 volts, but they will be ok for appropriate use anyway.

