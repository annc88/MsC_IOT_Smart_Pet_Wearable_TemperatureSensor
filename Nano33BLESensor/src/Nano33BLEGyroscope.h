/*
  Nano33BLEGyroscope.h
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This class reads gyroscope data from the on board Nano 33 BLE
  Sense IMU using Mbed OS. It stores the results in a ring 
  buffer (within the Nano33BLESensorBuffer Class) which can be accessed
  in a manner with softer time constraints than other implementations. 

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*****************************************************************************/
/*INLCUDE GUARD                                                              */
/*****************************************************************************/
/* Update these names to match the name of the file */ 
#ifndef NANO33BLEGYROSCOPE_H_
#define NANO33BLEGYROSCOPE_H_

/*****************************************************************************/
/*INLCUDES                                                                   */
/*****************************************************************************/
#include "Nano33BLESensorBuffer.h"
#include "Thread.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/**
 * This macro is required. It defines the wait period between sensor reads.
 * Update to the value you need based on how fast the sensor can read data.  
 */
#define DEFAULT_GYROSCOPE_READ_PERIOD_MS                (8U)
#define DEFAULT_GYROSCOPE_THREAD_STACK_SIZE_BYTES       (1024U) 

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*CLASS DECLARATION                                                          */
/*****************************************************************************/
/**
 * This class defines the data types that the sensor will ultimately give us 
 * after a read operation. Update it to your sensor requirements and call it
 * whatever you like. Make sure the members are public.
 */

class Nano33BLEGyroscopeData
{
  public:
    float x;
    float y;
    float z;
    uint32_t timeStampMs;
};

/**
 * @brief This class reads gyroscope data from the on board Nano 33 BLE
 * Sense IMU using Mbed OS. It stores the results in a ring 
 * buffer (within the Nano33BLESensorBuffer Class) which can be accessed
 * in a manner with softer time constraints than other implementations. 
 * 
 */
class Nano33BLEGyroscope: public Nano33BLESensorBuffer<Nano33BLEGyroscopeData>
{
  public:
   /**
     * @brief Initialises the sensor and starts the Mbed OS Thread.
     * 
     */
    void begin()
    {
      init();
      readThread.start(mbed::callback(Nano33BLEGyroscope::readFunction, this));
    }

    Nano33BLEGyroscope(
      uint32_t readPeriod_ms = DEFAULT_GYROSCOPE_READ_PERIOD_MS,
      osPriority threadPriority = osPriorityNormal,
      uint32_t threadSize = DEFAULT_GYROSCOPE_THREAD_STACK_SIZE_BYTES) :
        readPeriod(readPeriod_ms),
        readThread(
        threadPriority,
        threadSize){};
  private:
    /**
     * @brief Initialises the accelerometer sensor.
     * 
     */
    void init(void);
    /**
     * @brief Takes one reading from the accelerometer sensor if a reading 
     * is available.
     * 
     */
    void read(void);

    static void readFunction(Nano33BLEGyroscope *instance)
    {
      while(1)
      {
          instance->read();
      }
    }

    uint32_t readPeriod;
    rtos::Thread readThread;
};

extern Nano33BLEGyroscope Gyroscope;
#endif /* NANO33BLEGYROSCOPE_H_ */