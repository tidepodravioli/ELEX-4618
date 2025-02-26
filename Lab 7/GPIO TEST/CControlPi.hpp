/**
 * @file CControlPi.hpp
 * @author Rafael Banalan  A01367816 4S (abanalan@my.bcit.ca)
 * @brief 
 * @version 0.1
 * @date 2025-02-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include "CJoystickPosition.hpp"
#include "pigpio.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>

#ifndef BUTTON_DEBOUNCE_TIMEOUT
#define BUTTON_DEBOUNCE_TIMEOUT 50 ///< How long to wait before validating a button press (can be overriden)
#endif

#define BUTTON_NUM_OF_CHANNELS 2 ///< Number of button channels to account for (important for addressing arrays)

#define CH_SWITCH_S1 24 ///< GPIO pin for S1
#define CH_SWITCH_S2 23 ///< GPIO pin for S2

#define CH_RGBLED_RED_PIN 2
#define CH_RGBLED_GRN_PIN 3
#define CH_RGBLED_BLU_PIN 4

#define CH_JOYSTICK_X 0
#define CH_JOYSTICK_Y 1

#define SERVO_PORT0 0

#define ADC_SPI_CLK 11
#define ADC_SPI_MISO 9
#define ADC_SPI_MOSI 10
#define ADC_SPI_CS 8

/** @name RPI GPIO channels
 *  @brief These arrays define the channel numbers used in CControlPi. Note that a method may
 * only access one of these arrays, since not every channel set is needed for a given operation.
 * @{
 */
#define COUNT_DIGITAL_PINS 5
#define COUNT_ANALOG_PINS 2
#define COUNT_SERVO_PINS 1

static const int DIGITAL_IN_PINS[] = {CH_SWITCH_S1, CH_SWITCH_S2};
static const int DIGITAL_OUT_PINS[] = {CH_RGBLED_RED_PIN, CH_RGBLED_GRN_PIN, CH_RGBLED_BLU_PIN};

static const int ANALOG_PINS[] = {CH_JOYSTICK_X, CH_JOYSTICK_Y};
static const int SERVO_PINS[] = {SERVO_PORT0};
/**}@ */

using namespace std;

/**
 * @brief Represents a serial interface between the computer and a microcontroller.
 * 
 */
class CControlPi {
    private:    
        
        /**
         * @brief Initializes the RPI gpio pins relevant to the launchpad controls
         * 
         */
        void launchpadInit();
    
        /**
         * @brief Stores all of the times since each button was inactive
         * 
         */
        std::chrono::time_point<std::chrono::system_clock> m_lastDebounceTimes[BUTTON_NUM_OF_CHANNELS];
        
        /**
         * @brief Stores a bool for each button representing whether a button was just recently pressed or not
         * 
         */
        bool m_buttonActive[BUTTON_NUM_OF_CHANNELS];
    
    public:
        /**
         * @brief Construct a new CControl object
         *
         */
        CControlPi(bool &init);

        /**
         * @brief Destroys the CControl object
         * 
         */
        ~CControlPi();
    
        /**
         * @brief Gets DIGITAL data from the given RPI GPIO pin
         *
         * @param gpioChannel The RPI GPIO digital channel to get the data from
         * @param result Where to store the value
         */
        void get_data (int gpioChannel, int &result);
    
        /**
         * @brief Sets/writes data of the given type to the microcontroller. Automatically writes a 0 or 1
         * as a digital value to the given pin, unless PWM is true.
         * 
         * @param gpioChannel The RPI GPIO digital channel to write the data to
         * @param val The data being written (either 1/0 or in us for PWM)
         * @param pwm Whether or not to explicitly write val as PWM
         */
        void set_data (int gpioChannel, int val, bool pwm = false);

        /**
         * @brief Sets/writes data to the given servo channel
         * 
         * @param gpioChannel The  
         * @param val 
         */
        void set_servo(int gpioChannel, int val);
    
        /**
         * @brief Gets the position of the joystick(analog stick) as a raw ADC value
         * 
         * @param pass where to store a boolean of whether or not the serial communication was successful
         * @return CJoystickPosition 
         */
        CJoystickPosition get_analog();
    
         /**
          * @brief Checks if a button press was detected on the given channel
          * 
          * @param channel the RPI GPIO pin number to check
          * @param savech the array index storing this buttons current state (ranges from 0 to BUTTON_NUM_OF_CHANNELS -1)
          * @return true if the button was pressed even after debounce
          * @return false if the button wasn't pressed
          */
        bool get_button(int channel);

    };