# MySensors.Tools
Tools to support MySensors library development and usage.

## ChannelScannerRF24
A nRF24L01+ based WiFi Channel Scanner optimised for MySensors RF24 radios.  

The code is based on the Poor Man's Wireless 2.4GHz Scanner, created March 2011 by Rolf Henkel

This tool uses an nRF24L01+ connected to an Arduino AVR Uno/Nano/Mini:

```
Cables are:
    SS       -> 10
    MOSI     -> 11
    MISO     -> 12
    SCK      -> 13
and CE       ->  9 - do not change CE to other pin, it is hard coded
```

## TrafficDetectorRF24
A nRF24L01+ based WiFi Traffic Detector for logic analysers.  

The detector scans only one definable channel, typically the channel on which your MySensors transport network operates.  
A definable output pin can be connected to a LED or better to a logic analyser input.

The code is based on the Poor Man's Wireless 2.4GHz Scanner, created March 2011 by Rolf Henkel

This tool uses an nRF24L01+ connected to an Arduino AVR Uno/Nano/Mini:

```
Cables are:
    SS       -> 10
    MOSI     -> 11
    MISO     -> 12
    SCK      -> 13
and CE       ->  9 - do not change CE to other pin, it is hard coded
```

Set following defines:
- MY_CHANNEL to detect radio traffic on it  
- and DEBUG_PIN to connect with a LED to ground and/or better with a logic analyser input.

## Additional Links to External Tools
Here are some external links to additional cool tools very useful for your work 
with the MySensors library and RF24 transport network:

- [NRF24 Sniffer](https://www.mysensors.org/controller/sniffer)
- [MYSController](https://www.mysensors.org/controller/myscontroller)
- [nRF24Doctor](https://github.com/TechNovation01/nRF24Doctor) - see project description in [MySensors forum](https://forum.mysensors.org/topic/9178/nrf24doctor)
