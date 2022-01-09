#include <Arduino.h>

// S -> A0
// - -> ground

class BatteryMonitor
{

    int signal;
    float R1 = 30000.0;
    float R2 = 7500.0;
    bool debug = false;

public:
    BatteryMonitor(int input_signal, bool debug)
    {
        signal = input_signal;
        debug = debug;
    }

    void setup()
    {
        pinMode(signal, INPUT);
        Serial.print("DC VOLTMETER");
    }

    void loop()
    {
        int value = analogRead(signal);
        float vout = (value * 3.0) / 1024.0;
        float vin = vout / (R2 / (R1 + R2));
        if (debug)
        {
            Serial.print("INPUT V= ");
            Serial.println(vin);
        }
    }
};
