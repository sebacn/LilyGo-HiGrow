#ifndef _extlog_influxdb_h
#define _extlog_influxdb_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <InfluxDbClient.h>
#include "ca_cert.h"
#include "locallog.hpp"
#include "common.h"

#define NOT_SET_MODE 0
#define CONFIG_MODE 1
#define VALIDATING_MODE 2
#define OPERATING_MODE 3

#define WRITE_PRECISION WritePrecision::S
#define MAX_BATCH_SIZE 70
#define WRITE_BUFFER_SIZE 100

#define max_readings 24

// Server certificate in PEM format, placed in the program (flash) memory to save RAM
constexpr char const *ROOT_CA_INFLUXDB = ROOT_CA_POSITIONSTACK;

const char MTLS_CERT[] PROGMEM =  R"EOF(
-----BEGIN CERTIFICATE-----
CERT
-----END CERTIFICATE-----)EOF";

const char MTLS_PKEY[] PROGMEM =  R"EOF(
-----BEGIN PRIVATE KEY-----
PKEY
-----END PRIVATE KEY-----)EOF";

struct LogSettings {
    // InfluxDB 2 server or cloud url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
    String INFLUXDB_URL; // = "influxdb-url"
    // InfluxDB 2 server or cloud API authentication token (Use: InfluxDB UI -> Load Data -> Tokens -> <select token>)
    String INFLUXDB_TOKEN; // "token"
    // InfluxDB 2 organization name or id (Use: InfluxDB UI -> Settings -> Profile -> <name under tile> )
    String INFLUXDB_ORG; // "org"
    // InfluxDB 2 bucket name (Use: InfluxDB UI -> Load Data -> Buckets)
    String INFLUXDB_BUCKET; // "bucket"

    void print()
    {
        llog_d("INFLUXDB_URL: %s", INFLUXDB_URL.c_str());
        llog_d("INFLUXDB_BUCKET: %s", INFLUXDB_BUCKET.c_str());
        llog_d("INFLUXDB_ORG: %s", INFLUXDB_ORG);
        llog_d("INFLUXDB_TOKEN: %s", INFLUXDB_TOKEN);
    }
        
};

LogSettings logSettings;

InfluxDBClient client;
// Single InfluxDB instance

int writeLogInfo(higrow_sensors_event_buff_t *sensors_events){

    int cnt = 0;

    llog_d("InfluxDBClient: writeLogInfo");

    if (!sensors_events->next())
    {
        return cnt;
    }

    if (   logSettings.INFLUXDB_URL == ""
        || logSettings.INFLUXDB_ORG == ""
        || logSettings.INFLUXDB_BUCKET == ""
        || logSettings.INFLUXDB_TOKEN == "")
    {
        llog_d("InfluxDBClient: missing configuration");
        return cnt;
    }

    client.setWriteOptions(WriteOptions().writePrecision(WRITE_PRECISION).batchSize(MAX_BATCH_SIZE).bufferSize(WRITE_BUFFER_SIZE));
    client.setConnectionParams(logSettings.INFLUXDB_URL, logSettings.INFLUXDB_ORG, logSettings.INFLUXDB_BUCKET, logSettings.INFLUXDB_TOKEN, ROOT_CA_INFLUXDB, MTLS_CERT, MTLS_PKEY);
    client.setInsecure(false);

    llog_d("InfluxDBClient UTCTimestamp (s): %s", String(millis()));

    // Check server connection
    if (client.validateConnection()) 
    {
        llog_d("InfluxDBClient Connected OK to: %s", client.getServerUrl());

        while (sensors_events->next())
        {
            higrow_sensors_event_t val = sensors_events->peek();

            Point pointDevice("T-Higrow-AB7872");
            pointDevice.addTag("Battery_Ok", "1");

            //pointDevice.addField("Timestamp", val.timestamp);
            pointDevice.addField("Light", val.light);
            pointDevice.addField("Soil", val.soli);
            pointDevice.addField("Salt", val.salt);
            pointDevice.addField("Voltage", val.voltage);
            pointDevice.addField("Temperature", val.temperature);
            pointDevice.setTime(val.timestamp);  //set the time

            // Write point
            if (!client.writePoint(pointDevice))
            {
                llog_e("InfluxDBClient write failed: %s", client.getLastErrorMessage());
                break;
            }
            cnt++;
        } 

        if (!client.isBufferEmpty()) {
            llog_d("InfluxDBClient flush buffer"); //Write all remaining points to db
            client.flushBuffer();
        }
    } 
    else 
    {
        llog_e("InfluxDB connection failed: %s", client.getLastErrorMessage());
    }

    return cnt;
}

#endif