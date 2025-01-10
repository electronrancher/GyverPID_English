/*
GyverPID - PID controller library for Arduino
Documentation: https://alexgyver.ru/gyverpid/
GitHub: https://github.com/GyverLibs/GyverPID
Features:
- Time of one calculation is about 70 μs
- Operation mode by value or by its change (for integrating processes)
- Returns the result by the built-in timer or in manual mode
- Built-in coefficient calibrators
- Operation mode by error and by measurement error
- Built-in optimizers of the integral sum

AlexGyver, alex@alexgyver.ru
https://alexgyver.ru/
MIT License

Versions:
v1.1 - defines removed
v1.2 - defines returned
v1.3 - calculations accelerated, library simplified
v2.0 - the logic of operation is slightly rethought, the code is improved, simplified and simplified
v2.1 - integral moved to public
v2.2 - calculation optimization
v2.3 - added PID_INTEGRAL_WINDOW mode
v2.4 - implementation added to class
v3.0
- Added integral component optimization mode (see doc)
- Added automatic coefficient calibrators (see examples and doc)
v3.1 - fixed ON_RATE mode, added automatic limitation of int. sum
v3.2 - a bit of optimization, added getResultNow
v3.3 - in tuners you can pass another Stream class handler for debugging
*/

#ifndef _GyverPID_h
#define _GyverPID_h
#include <Arduino.h>

#if defined(PID_INTEGER) // calculations with integers
typedef int16_t pidtype;
#else // calculations with float numbers
typedef float pidtype;
#endif

#define NORMAL 0
#define REVERSE 1
#define ON_ERROR 0
#define ON_RATE 1

class GyverPID {
public:
// ==== pidtype is float or int, depending on the selected one (see integer_calc example) ====
GyverPID() {}

// kp, ki, kd, dt
GyverPID(float new_kp, float new_ki, float new_kd, uint32_t new_dt = 100) {
setDt(new_dt);
Kp = new_kp;
Ki = new_ki;
Kd = new_kd;
}

// direction of regulation: NORMAL (0) or REVERSE (1)
void setDirection(bool direction) {
_direction = direction;
}

// mode: operation on input error ON_ERROR (0) or on change ON_RATE (1)
void setMode(bool mode) {
_mode = mode;
}

// output value limit (for example, for PWM we set 0-255)
void setLimits(int16_t min_output, int16_t max_output) {
_minOut = min_output;
_maxOut = max_output;
}

// setting the sampling time (for getResultTimer)
void setDt(uint32_t new_dt) {
_dt_s = new_dt / 1000.0f;
_dt = new_dt;
}

pidtype setpoint = 0; // the specified value that the controller should support
pidtype input = 0; // signal from the sensor (for example, the temperature that we regulate)
pidtype output = 0; // output from the controller to the control device (for example, the PWM value or the servo rotation angle)
float Kp = 0.0; // P coefficient
float Ki = 0.0; // I coefficient
float Kd = 0.0; // D coefficient
float integral = 0.0; // integral sum

// returns a new value when called (if we use our own timer with a period of dt!)
pidtype getResult() {
pidtype error = setpoint - input; // regulation error
pidtype delta_input = prevInput - input; // input signal change over dt
prevInput = input; // remember the previous one
if (_direction) { // direction change
error = -error;
delta_input = -delta_input;
}
output = _mode ? 0 : (error * Kp); // proportional component
output += delta_input * Kd / _dt_s; // differential component

#if (PID_INTEGRAL_WINDOW > 0)
// EXPERIMENTAL MODE OF INTEGRAL WINDOW
if (++t >= PID_INTEGRAL_WINDOW) t = 0; // rewind t
integral -= errors[t]; // subtract old
errors[t] = error * Ki * _dt_s; // store in array
integral += errors[t]; // add new
#else
integral += error * Ki * _dt_s; // normal summation of int. sum
#endif

#ifdef PID_OPTIMIZED_I
// EXPERIMENTAL MODE OF LIMITING INTEGRAL SUM
output = constrain(output, _minOut, _maxOut);
if (Ki != 0) integral = constrain(integral, (_minOut - output) / (Ki * _dt_s), (_maxOut - output) / (Ki * _dt_s));
#endif

if (_mode) integral += delta_input * Kp; // mode is proportional to speed
integral = constrain(integral, _minOut, _maxOut); // limit the int. sum
output += integral; // integral component
output = constrain(output, _minOut, _maxOut); // limit the output
return output;
}

// returns a new value no earlier than dt milliseconds (built-in timer with period dt)
pidtype getResultTimer() {
if (millis() - pidTimer >= _dt) {
pidTimer = millis();
getResult();
}
return output;
}

// calculates the output based on the real time elapsed between function calls
pidtype getResultNow() {
setDt(millis() - pidTimer);
pidTimer = millis();
return getResult();
}

private:
uint32_t _dt = 100; // iteration time in ms
float _dt_s = 0.1; // iteration time in s
bool _mode = 0, _direction = 0;
int16_t _minOut = 0, _maxOut = 255;
pidtype prevInput = 0;
uint32_t pidTimer = 0;
#if (PID_INTEGRAL_WINDOW > 0)
pidtype errors[PID_INTEGRAL_WINDOW];
uint16_t t = 0;
#endif
};
#endif
