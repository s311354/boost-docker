#include <iostream>
#include <string>
#include <vector>
#include <math.h> 


#ifndef SIMULATEDCONTROLLER_H
#define SIMULATEDCONTROLLER_H

using namespace std;

namespace common {

enum class CommunicationType {
    Serial,
    TCP_IP
};

enum IOType {
    DIGITAL_INPUT,
    DIGITAL_OUTPUT,
    ANALOG_INPUT,
    ANALOG_OUTPUT
};

class SimulatedController {
private:

    std::vector<bool> digitalInputs;
    std::vector<bool> digitalOutputs;
    std::vector<double> analogInputs;
    std::vector<double> analogOutputs;

    // Number of IO channels for each type
    static constexpr int numChannels = 3;

    // Resolution and range for analog inputs/outputs
    static constexpr double analogResolution = 0.25;
    static constexpr double analogRangeMin = -5.0;
    static constexpr double analogRangeMax = 5.0;

    // Communication parameters
    CommunicationType communicationType;
    std::string serialPort;
    std::string ipAddress;

public:
    // Constructor
    SimulatedController(CommunicationType type, const std::string& portOrIP)
        : communicationType(type), serialPort(""), ipAddress("") {
        if (type == CommunicationType::Serial) {
            serialPort = portOrIP;
        } else {
            ipAddress = portOrIP;
        }

        // Initialize digital inputs/outputs to false
        digitalInputs.resize(numChannels, false);
        digitalOutputs.resize(numChannels, false);

        // Initialize analog inputs/outputs to 0.0
        analogInputs.resize(numChannels, 0.0);
        analogOutputs.resize(numChannels, 0.0);
    }

    // read from an IO channel
    bool readIO(IOType type, int index) const;

    // write to an IO channel
    void writeIO(IOType type, int index, bool value);
    void writeIO(IOType type, int index, double value);

    // print the state of the controller
    void printState() const;

    // communicate via Serial
    void communicateViaSerial();

    // communicate via TCP/IP
    void communicateViaTCP_IP();

    // perform communication based on the selected communication type
    void communicate();
};

} /* namespace common */

#endif /* SIMULATEDCONTROLLER_H */
