#pragma once

#include <Arduino.h>

typedef struct {
    uint32_t timestamp;     /**< time is in milliseconds */
    float temperature;      /**< temperature is in degrees centigrade (Celsius) */
    float light;            /**< light in SI lux units */
    float pressure;         /**< pressure in hectopascal (hPa) */
    float humidity;         /**<  humidity in percent */
    float altitude;         /**<  altitude in m */
    float voltage;           /**< voltage in volts (V) */
    uint8_t soli;           //Percentage of soil
    uint8_t salt;           //Percentage of salt
} higrow_sensors_event_t;

typedef struct {
    int ptr_w;
    int ptr_r;

    higrow_sensors_event_t sensors_events_buff[150];

    bool next()
    {
        return ptr_w != ptr_r;
    }

    void put(higrow_sensors_event_t _val)
    {
        sensors_events_buff[ptr_w] = _val;

        incPtr(&ptr_w);
    }

    higrow_sensors_event_t peek()
    {
        higrow_sensors_event_t ret = sensors_events_buff[ptr_r];
        
        incPtr(&ptr_r);

        return ret;
    }

    void incPtr(int *ptr)
    {
        *ptr++;

        if (*ptr >= 150)
        {
            *ptr = 0;
        }
    }

} higrow_sensors_event_buff_t;
