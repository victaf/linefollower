# Gebruiksaanwijzing

### opladen / vervangen batterijen
Soldeer de verbindingen van de 18650 lader pcb met een 18650 houder. Steek de 18650 batterij in de juiste oriëtatie in de houder. Sluit de pcb aan op een USB-C lader.
### draadloze communicatie
#### verbinding maken
Download eerst de "bluetooth serial terminal" app.
Steek de batterijen in de wagen en zet de bluetooth van je smartphone aan en maak verbinding met HC-05, de code kun je vinden op de website waar je de module hebt gekocht (meestal 0000 of 1234).
Open de bluetooth serial terminal app en druk op het stekker icoontje rechtsboven.

#### commando's
debug
start  
stop  
set cycle [µs]  
set power [0..255]  
set diff [0..1]  
set kp [0..]  
set ki [0..]  
set kd [0..]  
calibrate black  
calibrate white  

### kalibratie
Zet de wagen op een volledig zwart oppervlak en stuur het commando "calibrate black".
Zet daarna de wagen op een compleet wit oppervlak en stuur het commando "calibrate white".
Klaar.
### settings
De robot rijdt stabiel met volgende parameters:  
Cycle time: 2000, Power: 140, diff: 0.6, kp: 70, ki: 0.5, kd: 1.1 
### start/stop button
De auto heeft momenteel geen werkende Start/stop knop. Gebruik de commandos. De auto stopt ook automatisch als hij van de baan rijd.
