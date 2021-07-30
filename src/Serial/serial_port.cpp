/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#include <iostream>
#include "serial_port.h"

namespace ah {
namespace io {
namespace serial {

SerialPort::SerialPort() :
    _end_of_line_char('\n')
{
}

SerialPort::~SerialPort()
{
}

bool SerialPort::start(const char *com_port_name, int baud_rate)
{
    boost::system::error_code ec;

    if (_serial_port) {
        std::cout << "error: port is already opened..." << std::endl;
        return false;
    }

    _serial_port = serial_port_ptr(new boost::asio::serial_port(_io_service));
    _serial_port->open(com_port_name, ec);
    if (ec) {
        std::cout << "error: _serial_port->open() failed...com_port_name="
                     << com_port_name << ", e=" << ec.message().c_str() << std::endl;
        return false;
    }
    std::cout << "serial port (" << com_port_name << ") is opened..." << std::endl;

    _serial_port->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    _serial_port->set_option(boost::asio::serial_port_base::character_size(8));
    _serial_port->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    _serial_port->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    _serial_port->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

	async_read_some();

    boost::thread io_thread(boost::bind(&boost::asio::io_service::run, &_io_service));

    return true;
}

void SerialPort::stop()
{
    boost::recursive_mutex::scoped_lock lock(_mutex_read);

    if (_serial_port) {
        _serial_port->cancel();
        _serial_port->close();
        _serial_port.reset();
    }
    _io_service.stop();
    _io_service.reset();
}

void SerialPort::async_read_some()
{
    if (_serial_port.get() == NULL || !_serial_port->is_open())
        return;

    _serial_port->async_read_some(
                boost::asio::buffer(_read_buf_raw, SERIAL_PORT_READ_BUF_SIZE),
                boost::bind(
                    &SerialPort::on_receive,
                    this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void SerialPort::on_receive(const boost::system::error_code& ec, size_t bytes_transferred)
{
    boost::recursive_mutex::scoped_lock lock(_mutex_read);

    if (_serial_port.get() == NULL || !_serial_port->is_open())
        return;

    if (ec) {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
        async_read_some();
        return;
    }

    std::cout << "on_receive(" << bytes_transferred << ") ";
    for (size_t i=0; i<bytes_transferred; i++) std::cout << " 0x" << std::hex << (int)((unsigned char*)_read_buf_raw)[i];
    std::cout << std::endl;

    for (unsigned int i=0; i<bytes_transferred; i++) {
        char c = _read_buf_raw[i];
        if (c == _end_of_line_char) {
            on_receive(_read_buf_str);
            _read_buf_str.clear();
        }
        else {
            _read_buf_str += c;
        }
    }

    async_read_some();
}

void SerialPort::on_receive(const std::string &data)
{
    std::cout << "SerialPort::on_receive() : " << data << std::endl;
}

int SerialPort::write_some(const std::string &buf)
{
    return write_some(buf.c_str(), buf.size());
}

int SerialPort::write_some(const char *buf, const size_t size_in_bytes)
{
    boost::recursive_mutex::scoped_lock lock(_mutex_write);

    boost::system::error_code ec;
    if (_serial_port.get() == NULL) return -1;
	if (size_in_bytes == 0) return 0;

    /*std::cout << "write_some(" << size_in_bytes << ") ";
    for (size_t i=0; i<size_in_bytes; i++) std::cout << " 0x" << std::hex << (int)((unsigned char*)buf)[i];
    std::cout << std::endl;*/

	return _serial_port->write_some(boost::asio::buffer(buf, size_in_bytes), ec);
}

char SerialPort::end_of_line_char() const
{
    return _end_of_line_char;
}

void SerialPort::end_of_line_char(const char &c)
{
    _end_of_line_char = c;
}

std::vector<std::string> SerialPort::get_port_names()
{
    std::vector<std::string> names;
//#ifdef _WIN32
#if 0
    BOOL rv;
    DWORD size;
    GUID guid[1];
    HDEVINFO hdevinfo;
    DWORD idx = 0;
    SP_DEVINFO_DATA devinfo_data;
    devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);
    int count = 0;

    rv = SetupDiClassGuidsFromName("Ports", (LPGUID)&guid, 1, &size) ;
    if (!rv) {
        std::cout << "error : SetupDiClassGuidsFromName() failed..." << std::endl;
        return names;
    }

    hdevinfo = SetupDiGetClassDevs(&guid[0], NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
    if (hdevinfo == INVALID_HANDLE_VALUE) {
        std::cout << "error : SetupDiGetClassDevs() failed..." << std::endl;
        return names;
    }

    while(SetupDiEnumDeviceInfo(hdevinfo, idx++, &devinfo_data)) {
        char friendly_name[MAX_PATH];
        char port_name[MAX_PATH];
        DWORD prop_type;
        DWORD type = REG_SZ;
        HKEY hKey = NULL;

        rv = ::SetupDiGetDeviceRegistryProperty(hdevinfo, &devinfo_data, SPDRP_FRIENDLYNAME, &prop_type,
                                                (LPBYTE)friendly_name, sizeof(friendly_name), &size);
        if (!rv) {
            std::cout << "error : SetupDiGetDeviceRegistryProperty() failed..." << std::endl;
            continue;
        }

        hKey = ::SetupDiOpenDevRegKey(hdevinfo, &devinfo_data, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
        if (!hKey) continue;

        size = sizeof(port_name);
        rv = ::RegQueryValueEx(hKey, "PortName", 0, &type, (LPBYTE)&port_name, &size);
        ::RegCloseKey(hKey);

        names.push_back(port_name);
    }

    SetupDiDestroyDeviceInfoList(hdevinfo);
#endif
    return names;
}

int SerialPort::get_port_number()
{
    std::vector<std::string> names = get_port_names();
    return names.size();
}

std::string SerialPort::get_port_name(const unsigned int &idx)
{
    std::vector<std::string> names = get_port_names();
    if (idx >= names.size()) return std::string();
    return names[idx];
}

void SerialPort::print_devices()
{
    std::cout << "SerialPort::print_devices()" << std::endl;
    int n = SerialPort::get_port_number();
    for (int i=0; i<n; i++) {
        std::string name = SerialPort::get_port_name(i);
        std::cout << "\t" << name.c_str() << std::endl;
    }
}

} //namespace serial
} //namespace io
} //namespace ah
