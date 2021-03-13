# MySensors.Tools
Tools to support MySensors library development and usage.

## ChannelScannerRF24
A nRF24L01+ based WiFi Channel Scanner optimised for MySensors RF24 radios.  
Uses an nRF24L01+ connected to an Arduino AVR Uno/Nano/Mini

´´´
Cables are:
    SS       -> 10
    MOSI     -> 11
    MISO     -> 12
    SCK      -> 13
and CE       ->  9 - do not change CE to other pin, it is hard coded
´´´

## TrafficDetector24
A nRF24L01+ based WiFi Traffic Detector for logic analysers.  
Uses an nRF24L01+ connected to an Arduino AVR Uno/Nano/Mini

´´´
Cables are:
    SS       -> 10
    MOSI     -> 11
    MISO     -> 12
    SCK      -> 13
and CE       ->  9 - do not change CE to other pin, it is hard coded
´´´

Define:
- MY_CHANNEL to detect radio traffic on it  
- and DEBUG_PIN to connect with a LED to ground and/or better with a logic analyser input.
