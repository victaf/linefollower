# start/stop interrupt proof of concept
minimale hard- en software die de correcte werking van een start/stop drukknop aantoont, gebruik makend van een hardware interrupt
## stappenplan
### 1. verbidingen:
  - 1 kant van de knop aan pin 7.
  - de 2e kant van de knop aan GND.
### 2. software:
  - Upload interrupt_POC.ino naar de arduino.
### 3. testen:
  - De LED op de PCB van de Arduino gaat knipperen. Als op de knop gedrukt wordt zal hij stoppen met knipperen. Als wederom op de knop gedrukt wordt zal hij terug knipperen.
