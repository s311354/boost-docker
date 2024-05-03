#include "./SimulatedController.h"

namespace common {

    bool SimulatedController::readIO(IOType type, int index) const {
        switch (type) {
            case DIGITAL_INPUT:
                if (index >= 0 && index < numChannels)
                    return digitalInputs[index];
                break;
            case DIGITAL_OUTPUT:
                if (index >= 0 && index < numChannels)
                    return digitalOutputs[index];
                break;
            case ANALOG_INPUT:
                if (index >= 0 && index < numChannels)
                    return analogInputs[index];
                break;
            case ANALOG_OUTPUT:
                if (index >= 0 && index < numChannels)
                    return analogOutputs[index];
                break;
            default:
                    return false;
                break;
        }
        return false; // Error: Invalid type or index
    }

    void SimulatedController::writeIO(IOType type, int index, bool value) {
        switch (type) {
            case DIGITAL_OUTPUT:
                if (index >= 0 && index < numChannels)
                    digitalOutputs[index] = value;
                break;
            default:
                break;
        }
    }

   void SimulatedController::writeIO(IOType type, int index, double value) {
        switch (type) {
            case ANALOG_OUTPUT:
                if (index >= 0 && index < numChannels) {
                    // Ensure value is within range
                    if (value < analogRangeMin) value = analogRangeMin;
                    if (value > analogRangeMax) value = analogRangeMax;

                    // Round value to nearest multiple of analogResolution
                    double roundedValue = round(value / analogResolution) * analogResolution;
                    analogOutputs[index] = roundedValue;
                }
                break;
            default:
                break;
        }
    }

    void SimulatedController::printState() const {
        std::cout << "Digital Inputs:";
        for (bool input : digitalInputs) {
            std::cout << " " << input;
        }
        std::cout << std::endl;

        std::cout << "Digital Outputs:";
        for (bool output : digitalOutputs) {
            std::cout << " " << output;
        }
        std::cout << std::endl;

        std::cout << "Analog Inputs:";
        for (double input : analogInputs) {
            std::cout << " " << input;
        }
        std::cout << std::endl;

        std::cout << "Analog Outputs:";
        for (double output : analogOutputs) {
            std::cout << " " << output;
        }
        std::cout << std::endl;
    }

    void SimulatedController::communicateViaSerial() {
        std::cout << "Communicating via Serial on port: " << serialPort << std::endl;
        // Implement serial communication logic here
    }

    void SimulatedController::communicateViaTCP_IP() {
        std::cout << "Communicating via TCP/IP on IP address: " << ipAddress << std::endl;
        // Implement TCP/IP communication logic here
    }

    void SimulatedController::communicate() {
        if (communicationType == CommunicationType::Serial) {
            communicateViaSerial();
        } else {
            communicateViaTCP_IP();
        }
    }
} /* namespace common */
