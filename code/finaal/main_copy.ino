#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial1
#define Baudrate 9600

#define MotorLeftForward 5
#define MotorLeftBackward 6
#define MotorRightForward 9
#define MotorRightBackward 10


const int Drukknop = 7;//interupt
const int LED = 13; //Led

//interupt:
volatile bool runState = false;
volatile unsigned long lastDebounceTime = 0;  // Tijd voor debouncing
unsigned long debounceDelay = 50000;  // Debounce-tijd in milliseconden


SerialCommand sCmd(SerialPort);
bool debug;
bool run;                   
unsigned long previous, calculationTime;


const int sensor[] = {A5, A4, A3, A2, A1, A0};


struct param_t
{
  unsigned long cycleTime;
  int black[6];
  int white[6];
  float diff;
  float kp;
  int power;
  float ki;
  float kd;

} params;

int normalised[6];
float debugposition;
float output;
float error;
float iTerm = 0;
float lastErr;
int powerLeft;
int powerRight;


void setup()
{

  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT); 

  SerialPort.begin(Baudrate);

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.addCommand("run", onRun);
  sCmd.addCommand("stop", onStop);
  sCmd.addCommand("testmotors", testmotors);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);
  SerialPort.println("ready");

  //Interupt
  pinMode(Drukknop, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Drukknop), Interrupt, FALLING);

  pinMode(LED, OUTPUT);

}


void loop()
{
  sCmd.readSerial();
 
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;
    //fill normalised array bvb [992 834 18 769 1117 1126]
    for (int i = 0; i < 6; i++)
    {
      normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 1000);
      
    }

    //interpolatie
    int index = 0;
    float position;

    //find smallest value in normalized array
    for (int i = 1; i < 6; i++) if (normalised[i] < normalised[index]) index = i;

    //dit is voor als de kar niet meer over de lijn rijd
    if (normalised[index] > 900) run = false;
    

    if (index == 0) position = -30;
    else if (index == 5) position = 30;
    else
    { 
      // elementen naast laagste waarde in normelised array: 834 18 769
      int sNul = normalised[index]; // 18
      int sMinEen = normalised[index-1]; // 834
      int sPlusEen = normalised[index+1]; // 769

      
      float b = sPlusEen - sMinEen; // -65
      b = b / 2; // -32.5

      
      float a = sPlusEen - b - sNul; // 769 -(-32.5) - 18 = 783.5

      position = -b / (2 * a); // 32.5/(2*783.5) = 0.02074
      position += index; // = 2.02074
      position -= 2.5; // = -1.47925

      position *= 7.85; //afstand sensoren uit elkaar waarschijnlijk aanpassen // -11.612188

    }
    
    debugposition = position; //-11.612
    
    //P-regelaar
    error = -position; //11.612
    output = error * params.kp; //simpel kp = 1 => output = 11.612

    //I-Regelaar
    iTerm += params.ki * error; // 0
    iTerm = constrain(iTerm, -510, 510); //0
    output += iTerm; //11.612

    //D-Regelaar
    output += params.kd * (error - lastErr); // 11.612
    lastErr = error; //11.612

    output = constrain(output, -510, 510);

    powerLeft = 0;
    powerRight = 0;


    if (run) if (output >= 0)
    {
      powerLeft = constrain(params.power + params.diff * output, -255, 255); //10+1*11.612 = 21.612
      powerRight = constrain(powerLeft - output, -255, 255); // 21.612-11.612 = 10
      powerLeft = powerRight + output; // 10+11.612 = 21.612
    }
    else
    {
      powerRight = constrain(params.power - params.diff * output, -255, 255);
      powerLeft = constrain(powerRight + output, -255, 255);
      powerRight = powerLeft - output;
        
    }

    analogWrite(MotorLeftForward, powerLeft > 0 ? powerLeft : 0);
    analogWrite(MotorLeftBackward, powerLeft < 0 ? -powerLeft : 0);
    analogWrite(MotorRightForward, powerRight > 0 ? powerRight : 0);
    analogWrite(MotorRightBackward, powerRight < 0 ? -powerRight : 0);
  }
  
  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;

}

void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}


void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
  if (strcmp(param, "cycle") == 0)
  {
    long newCycleTime = atol(value);
    float ratio = ((float) newCycleTime) / ((float) params.cycleTime);

    params.ki *= ratio;
    params.kd /= ratio;

    params.cycleTime = newCycleTime;
  }

  else if (strcmp(param, "ki") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.ki = atof(value) * cycleTimeInSec;
  }

  else if (strcmp(param, "kd") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.kd = atof(value) / cycleTimeInSec;
  }

  else if (strcmp(param, "power") == 0) params.power = atol(value);
  else if (strcmp(param, "diff") == 0) params.diff = atof(value);
  else if (strcmp(param, "kp") == 0) params.kp = atof(value);

  EEPROM_writeAnything(0, params);
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
  SerialPort.print("black: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.black[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("white: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.white[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("Normalised: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(normalised[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("position: ");
  SerialPort.println(debugposition);

  SerialPort.print("Power: ");
  SerialPort.println(params.power);

  SerialPort.print("diff: ");
  SerialPort.println(params.diff);

  SerialPort.print("kp: ");
  SerialPort.println(params.kp);

  float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
  float ki = params.ki / cycleTimeInSec;
  SerialPort.print("Ki: ");
  SerialPort.println(ki);

  float kd = params.kd * cycleTimeInSec;
  SerialPort.print("Kd: ");
  SerialPort.println(kd);

  SerialPort.print("RUN: ");
  SerialPort.println(run);

  SerialPort.print("powerleft: ");
  SerialPort.println(powerLeft);

  SerialPort.print("powerright: ");
  SerialPort.println(powerRight);
  
  SerialPort.println(" ");
  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  calculationTime = 0;
}

void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black...");
    for (int i = 0; i < 6; i++) params.black[i] = analogRead(sensor[i]);
    SerialPort.println("done");
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white...");
    for (int i =0; i < 6; i++) params.white[i] = analogRead(sensor[i]);
    SerialPort.println("done");
  }

  EEPROM_writeAnything(0, params);
}

void onRun()
{
  run = true;
  digitalWrite(LED, HIGH);
}

void onStop()
{
  run = false;
  digitalWrite(LED, LOW);
}

void testmotors(){
    analogWrite(MotorLeftForward, 255);
    analogWrite(MotorLeftBackward, 0);
    analogWrite(MotorRightForward, 255);
    analogWrite(MotorRightBackward, 0);
}

void Interrupt()
{
  // Lees de status van de drukknop
  int buttonState = digitalRead(Drukknop);

  // Debounce de drukknop met millis
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Als de knop opgaand is, wissel de status van de LED
    if (buttonState == 1) {
      runState = !runState;

      // Schakel de LED in of uit op basis van de nieuwe status
      digitalWrite(LED, runState);
      run = runState;
      iTerm = 0;
    }

    // Update de tijd van de laatste debouncing
    lastDebounceTime = millis();
  }
  
}