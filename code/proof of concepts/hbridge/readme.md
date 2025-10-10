# H-Bridge proof of concept

minimale hard- & software + stappenplan dat aantoont dat 2 motoren onafhankelijk van elkaar kunnen draaien, en (traploos) regelbaar zijn in snelheid en draairichting.

## benodigdheden

- Arduino leonardo
- DRV8833 Hbrug module
- 2x6V DC motoren
- voeding(bvb 2*18650 batterijen)


## stappenplan

1. verbind de connecties
    1. De + en - van de voeding met de VCC en GND van de Hbrug.
    2. De GND met de GND van de Arduino.
    3. De IN1, IN2, IN3 en IN4 met pin 10, 9, 6 en 5 van de Arduino respectievelijk.
    4. De OUT1, OUT2, OUT3 en OUT4 met de motors.
2. Upload Hbrug_POC.ino naar de arduino.
3. Kijk of de motoren in de volgende sequentie draaien:
    1. Beide motoren van 0 tot Max in +- 2,5 sec.
    2. Stoppen
    3. Beide motoren van 0 tot Max in de andere richting in +-2,5 sec.
    4. Stoppen
    5. Max beide in verschillende richtingen.
    6. Stoppen
    7. Max beide in verschillende richtingen, andere richting.
    8. Stoppen
