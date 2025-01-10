[![latest](https://img.shields.io/github/v/release/GyverLibs/GyverPID.svg?color=brightgreen)](https://github.com/GyverLibs/GyverPID/releases/latest/download/GyverPID.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/GyverPID.svg)](https://registry.platformio.org/libraries/gyverlibs/GyverPID)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverPID?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# GyverPID
GyverPID - PID controller library for Arduino
- One calculation time is about 70 μs
- Operation mode by value or by its change (for integrating processes)
- Returns the result by the built-in timer or in manual mode
- Built-in coefficient calibrators
- Operation mode by error and by measurement error
- Built-in optimizers of the integral sum

### Compatibility
Compatible with all Arduino platforms (Arduino functions are used)

### Documentation
The library has [extended documentation](https://alexgyver.ru/GyverPID/)

## Contents
- [Installation](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback connection](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **GyverPID** and installed via the library manager in:
- Arduino IDE
- Arduino IDE v2
- PlatformIO
- [Download the library](https://github.com/GyverLibs/GyverPID/archive/refs/heads/main.zip) .zip archive for manual installation:
- Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
- Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
- Unzip and put in *Documents/Arduino/libraries/*
- (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features
- Via the IDE library manager: find the library as during installation and click "Update"
- Manually: **delete the folder with the old version**, and then put the new one in its place. "Replacement" is not allowed: sometimes new versions delete files that remain after replacement and can lead to errors!

<a id="init"></a>
## Initialization
```cpp
// You can initialize an object in three ways:
GyverPID regulator; // initialize without settings (everything to zeros, dt 100 ms)
GyverPID regulator(kp, ki, kd); // initialize with coefficients. dt will be 100 ms by default
GyverPID regulator(kp, ki, kd, dt); // initialize with coefficients and dt (in milliseconds)
```

<a id="usage"></a>
## Usage
See [documentation](https://alexgyver.ru/GyverPID/)
```cpp
datatype setpoint = 0; // a given value that the controller should support
datatype input = 0; // a signal from a sensor (for example, the temperature we are regulating)
datatype output = 0; // output from the controller to the control device (for example, the PWM value or the servo rotation angle)

datatype getResult(); // returns a new value when called (if we use our own timer with a period of dt!)
datatype getResultTimer(); // returns a new value no earlier than after dt milliseconds (built-in timer with a period of dt)
void setDirection(boolean direction); // direction of regulation: NORMAL (0) or REVERSE (1)
void setMode(boolean mode); // mode: operation based on the input error ON_ERROR (0) or on a change in ON_RATE (1)
void setLimits(int min_output, int max_output); // output value limit (for example, for PWM we set 0-255)
void setDt(int16_t new_dt); // setting the sampling time (for getResultTimer)
float Kp = 0.0;
float Ki = 0.0;
float Kd = 0.0;
```

<a id="example"></a>
## Example
See other examples in **examples**!
```cpp
/*
Example of PID controller operation in automatic mode using the built-in timer
Let's imagine that on pin 3 we have a heating coil connected via a MOSFET,
we control it with a PWM signal
And there is some abstract temperature sensor that is affected by the coil
*/

// before connecting the library, you can add settings:
// will make some calculations integer,
