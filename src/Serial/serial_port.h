/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <string>

typedef boost::shared_ptr<boost::asio::serial_port> serial_port_ptr;

#define SERIAL_PORT_READ_BUF_SIZE 256

namespace ah {
namespace io {
namespace serial {

//-----------------------------------------------------
// Serial Port
class SerialPort
{
public:
    SerialPort();
    virtual ~SerialPort();

    virtual bool start(const char *com_port_name, int baud_rate=9600);
    virtual void stop();

    int write_some(const std::string &buf);
    int write_some(const char *buf, const size_t size_in_bytes);

    char end_of_line_char() const;
    void end_of_line_char(const char &c);

public:
    static int get_port_number();
    static std::string get_port_name(const unsigned int &idx);
    static std::vector<std::string> get_port_names();
    static void print_devices();

protected:
    virtual void async_read_some();
    virtual void on_receive(const boost::system::error_code& ec, size_t bytes_transferred);
    virtual void on_receive(const std::string &data);

protected:
    boost::asio::io_service _io_service;
    serial_port_ptr _serial_port;
    boost::recursive_mutex _mutex_read;
    boost::recursive_mutex _mutex_write;

    char _end_of_line_char;
    char _read_buf_raw[SERIAL_PORT_READ_BUF_SIZE];
    std::string _read_buf_str;

private:
    SerialPort(const SerialPort &p);
    SerialPort &operator=(const SerialPort &p);
};

} //namespace serial
} //namespace io
} //namespace ah

#endif // __SERIAL_PORT_H__
