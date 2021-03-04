//// This library has been modified for suitable with DEV KIT PIC16F877A
//// It is used for "4-wire type stepper motor"
////
//// This library use Timer1 to calculate time had over, unit microsecond
//// It base KIT DEV PIC 16F877A, have Quartz Oscillator 20MHz
////    Timer1_freq = MCU_freq / {4 * Prescaler * (65536 - TMR1)}
////    1/0.000001  = 20,000,000 / (4 * 1 * (65,536 - 65,531))
////
//// NOTE: THE DEFAULT INTEGER VARIABLE DECLARATIONS IN CCS ARE "unsigned"
////
//// AUTHOR: TRUNG_KURO
#ifndef STEPPER_H
#define STEPPER_H

#use timer (timer=1, tick=1us, bits=32, noisr)

////////////////////////////////////////////////////////////////////////////////////////////////////////
//// FUNCTION EXTERNAL

//// Set pin method:
//// Set 4 pin of motor to driver
//// "numberOfSteps" (default: 200 pulse) for "4-wire type stepper motor" with 1.8deg/pulse
void stepper(int16 numberOfSteps, int8 motorPin1, int8 motorPin2, int8 motorPin3, int8 motorPin4);

//// Speed setter method:
//// Sets the motor speed in rotations per minute (RPMs)
//// This function doesn't make the motor turn, just sets the speed at which it will when you call step()
//// "rpms": the speed at which the motor should turn in rotations per minute - a positive number
void setSpeed(int16 whatSpeed);

//// Mover method:
//// Turns the motor a specific number of steps, at a speed determined by the most recent call to setSpeed()
//// "steps": the number of steps to turn the motor - positive to turn one direction, negative to turn the other
void step(signed int16 numberOfSteps);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//// FUNCTION INTERNAL

void stepMotor(int8 thisStep);

int1 direction;                 // Direction of rotation
unsigned int32 step_delay;      // Delay between steps, in ms, based on speed
int16 number_of_steps;          // Total number of steps this motor can take
int16 step_number;              // Which step the motor is on

// Motor pin numbers:
int8 motor_pin_1;
int8 motor_pin_2;
int8 motor_pin_3;
int8 motor_pin_4;

unsigned int32 last_step_time;  // Time stamp in us of when the last step was taken

// -------------------------------------------------------------------------------------------------- //

/*
 * Constructor for 4-pin version
 * Sets which wires should control the motor
 */
void stepper(int16 numberOfSteps, int8 motorPin1, int8 motorPin2, int8 motorPin3, int8 motorPin4)
{
  step_number = 0;                  // Which step the motor is on
  direction = 0;                    // Motor direction
  last_step_time = 0;               // Time stamp in us of the last step taken
  number_of_steps = numberOfSteps;  // Total number of steps for this motor

  // Pins of PIC for the motor control connection:
  motor_pin_1 = motorPin1;
  motor_pin_2 = motorPin2;
  motor_pin_3 = motorPin3;
  motor_pin_4 = motorPin4;
}

/*
 * Sets the speed in revs per minute
 */
void setSpeed(int16 whatSpeed)
{
  step_delay = 60 * 1000 * 1000 / number_of_steps / whatSpeed;
}

/*
 * Moves the motor numberOfSteps steps
 * If the number is negative, the motor moves in the reverse direction
 */
void step(signed int16 numberOfSteps)
{
  // How many steps to take ?
  int16 steps_left;

  // Determine direction based on whether steps_to_mode is + or -:
  if (numberOfSteps > 0)
  {
    steps_left = numberOfSteps;
    direction = 1;
  }
  if (numberOfSteps < 0)
  {
    steps_left = -numberOfSteps;
    direction = 0;
  }

  // Decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
    unsigned int32 now = get_ticks();
    // Move only if the appropriate delay has passed:
    if (now - last_step_time >= step_delay)
    {
      // Get the timeStamp of when you stepped:
      last_step_time = now;
      // Increment or decrement the step number,
      // Depending on direction:
      if (direction == 1)
      {
        step_number++;
        if (step_number == number_of_steps) { step_number = 0; }
      }
      else
      {
        if (step_number == 0) { step_number = number_of_steps; }
        step_number--;
      }
      // Decrement the steps left:
      steps_left--;
      // Step the motor to step number 0, 1, ...
      stepMotor(step_number % 4);
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
void stepMotor(int8 thisStep)
{
  switch (thisStep)
  {
    case 0:  // 1010
      output_high(motor_pin_1);
      output_low(motor_pin_2);
      output_high(motor_pin_3);
      output_low(motor_pin_4);
    break;
    case 1:  // 0110
      output_low(motor_pin_1);
      output_high(motor_pin_2);
      output_high(motor_pin_3);
      output_low(motor_pin_4);
    break;
    case 2:  //0101
      output_low(motor_pin_1);
      output_high(motor_pin_2);
      output_low(motor_pin_3);
      output_high(motor_pin_4);
    break;
    case 3:  //1001
      output_high(motor_pin_1);
      output_low(motor_pin_2);
      output_low(motor_pin_3);
      output_high(motor_pin_4);
    break;
  }
}

#endif
