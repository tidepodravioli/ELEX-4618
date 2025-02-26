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
         * @brief Gets data of the given type from the microcontroller.
         * 
         * @param type The type of data to get
         * @param channel The channel to get the data from
         * @param result Where to store the value
         * @return true if communication was successful, data was retrieved, and acknowledgement received.
         * @return false otherwise
         */
        void get_data (int gpioChannel, int &result);
    
        /**
         * @brief Sets/writes data of the given type to the microcontroller.
         * 
         * @param type The type of data that is being sent
         * @param channel The channel to get write the data to
         * @param val The data being written
         */
        void set_data (int gpioChannel, int val);
    
        /**
         * @brief Gets the position of the joystick(analog stick) as a raw ADC value
         * 
         * @param pass where to store a boolean of whether or not the serial communication was successful
         * @return CJoystickPosition 
         */
        CJoystickPosition get_analog();
    
        /**
         * @brief Checks if a button press was detected on SW1 on the microcontroller
         * 
         * @param pass where to store a boolean of whether or not the serial communication was successful
         * @return true if the button was pressed
         * @return false if the button wasn't pressed
         */
        bool get_button(int channel, int savech = 0);
    
        /**
         * @brief Finds and connects to the first serial port that responds with an acknowledge message
         * 
         * @return true if a port was found
         * @return false if a port was not found
         */
        bool findPort();
    
        /**
         * @brief Sends a "check if alive" message to the presently connected serial port 
         * 
         * @return true if the microcontroller responded
         * @return false if no response was heard
         */
        bool checkPort();
    };