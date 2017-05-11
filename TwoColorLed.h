/*
 * TwoColorLed.h
 * 
 * Copyright 2017 Zoltan DeWitt
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * Requires:
 *      ESP8266 Arduino: https://github.com/esp8266/Arduino
 * 
 * This library for ESP8266 Arduino controls a bi-color LED
 * Needs two PWM pins, and allows for color mixing and blinking
 * Each instance needs a timer, of which there are a limited number
 * available to use. See the ESP8266 docs for more info.
 */

#include "Arduino.h"
#include "user_interface.h"

#ifndef _TWO_COLOR_LED_H
#define _TWO_COLOR_LED_H

class TwoColorLed {

public:
    /* Constructor requires two PWM pins attached to the LED, and a flag indicating if
     * the LED is active low (cathode drive, common anode)
     */
    TwoColorLed( const uint8_t driveAPin, const uint8_t driveBPin, const bool activeLow = false );
    
    /* Set the LED's color
     * 0 is full driveA, 1023 is full driveB
     */
    void setColor( uint16_t color );
    
    /* Set the level between 0-1023
     */
    void setLevel( uint16_t level );
    
    /* Set the blink period. 0 resets to constant. onTime, offTime is in milliseconds.
     * If offTime is not specified, it is set equal to onTime
     */
    void setBlink( uint16_t onTime, uint16_t offTime = 0 );
    
    /* Read the current values back
     */
    uint16_t readColor();
    uint16_t readLevel();
    
    // public functions for ESP8266 timer interface
    void timerCallback();
    static void led_callback_service( void *ptr );
    
private:
    void updatePins();
    
    const uint8_t _driveAPin;
    const uint8_t _driveBPin;
    const bool _activeLow;
    uint16_t _color;
    uint16_t _level;
    uint16_t _onTime;
    uint16_t _offTime;
    os_timer_t _timer;
    bool _on;
    
};

#endif  // _TWO_COLOR_LED_H
