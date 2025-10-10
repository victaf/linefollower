// Define the control inputs

const int motorA_FW_pin 10;
const int motorA_BW_pin 9;
const int motorB_FW_pin 6;
const int motorB_BW_pin 5;

void setup(void)
{
  // Set all the motor control inputs to OUTPUT
  pinMode(motorA_FW_pin, OUTPUT);
  pinMode(motorA_BW_pin, OUTPUT);
  pinMode(motorB_FW_pin, OUTPUT);
  pinMode(motorB_BW_pin, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(motorA_FW_pin, LOW);
  digitalWrite(motorA_BW_pin, LOW);
  digitalWrite(motorB_FW_pin, LOW);
  digitalWrite(motorB_BW_pin, LOW);

  // Initialize the serial UART at 9600 baud
  Serial.begin(9600);
}

void loop(void)
{
  // Generate a fixed motion sequence to demonstrate the motor modes.

  // Ramp speed up.
  for (int i = 0; i < 255; i++) {
    spin_and_wait(i, i, 10);
  }
  // Full speed forward.
  spin_and_wait(255,255,2000);

  // Stop.
  spin_and_wait(0,0,2000);

  // Ramp speed into full reverse.
  for (int i = 0; i < 255 ; i++) {
    spin_and_wait(- i,- i, 10);
  }

  // Full speed reverse.
  spin_and_wait(-255,-255,2000);

  // Stop.
  spin_and_wait(0,0,2000);

  // Full speed, forward, turn, reverse, and turn for a two-wheeled base.
  spin_and_wait(-255, 255, 2000);
  spin_and_wait(0, 0, 1000);
  spin_and_wait(255, -255, 2000);
  spin_and_wait(0, 0, 1000);
}

/// Set the current on a motor channel using PWM and directional logic.
///
/// \param pwm    PWM duty cycle ranging from -255 full reverse to 255 full forward
/// \param IN1_PIN  pin number xIN1 for the given channel
/// \param IN2_PIN  pin number xIN2 for the given channel
void set_motor_pwm(int pwm, int IN1_PIN, int IN2_PIN)
{
  if (pwm < 0) {  // reverse speeds
    analogWrite(IN1_PIN, -pwm);
    digitalWrite(IN2_PIN, LOW);

  } else { // stop or forward
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, pwm);
  }
}

/// Set the current on both motors.
///
/// \param pwm_A  motor A PWM, -255 to 255
/// \param pwm_B  motor B PWM, -255 to 255
void set_motor_currents(int pwm_A, int pwm_B)
{
  set_motor_pwm(pwm_A, motorA_FW_pin, motorA_BW_pin);
  set_motor_pwm(pwm_B, motorB_FW_pin, motorB_BW_pin);

  // Print a status message to the console.
  Serial.print("Set motor A PWM = ");
  Serial.print(pwm_A);
  Serial.print(" motor B PWM = ");
  Serial.println(pwm_B);
}

/// Simple primitive for the motion sequence to set a speed and wait for an interval.
///
/// \param pwm_A  motor A PWM, -255 to 255
/// \param pwm_B  motor B PWM, -255 to 255
/// \param duration delay in milliseconds
void spin_and_wait(int pwm_A, int pwm_B, int duration)
{
  set_motor_currents(pwm_A, pwm_B);
  delay(duration);
}