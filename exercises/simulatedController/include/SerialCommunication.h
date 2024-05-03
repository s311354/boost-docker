
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

using namespace std;

namespace common {

class SerialCommunication {
private:
    boost::asio::io_service io;
    boost::asio::serial_port port;

public:
    SerialCommunication(const std::string& portName, unsigned int baudRate)
        : port(io, portName) {
        port.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
    }

    void write(const std::string& data);

    std::string read(unsigned int size);
};

} /* namespace common */

#endif /* SERIALCOMMUNICATION_H */