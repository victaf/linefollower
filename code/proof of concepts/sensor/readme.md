# Sensoren proof of concept

minimale hard- en software die aantoont dat minimaal 6 sensoren onafhankelijk van elkaar kunnen uitgelezen worden (geen calibratie, normalisatie of interpolatie). Hierbij moet een zo groot mogelijk bereik van de AD converter benut worden (indien van toepassing)

## stappenplan:
1. ### verbidingen:
  - D2-7 aan A0-5
  - VCC en IR aan 5V
  - GND aan GND
2. ### software:
  - Upload sensor_POC naar de arduino
### 3. testen:
  - Op de seriele monitor zou he de waarde van elk van de 6 sensoren moeten zien staan
