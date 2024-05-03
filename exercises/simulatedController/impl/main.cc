#include <fstream>
#include <sstream>
#include <iostream>

#include "./SimulatedController.h"
#include "./SerialCommunication.h"

int main(int argc, char *argv[])
{
    using namespace common;

    // Create a simulated controller with Serial communication
    SimulatedController controllerSerial(CommunicationType::Serial, "/dev/ttyUSB0");
    controllerSerial.communicate();

    // Set a digital output
    controllerSerial.writeIO(DIGITAL_OUTPUT, 0, true);

    // Set an analog output
    controllerSerial.writeIO(ANALOG_OUTPUT, 1, 3.5);

    controllerSerial.printState();

    return 0;
}
