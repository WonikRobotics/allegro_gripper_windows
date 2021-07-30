/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#include <iostream>
#include "serial_msg_dispatcher.h"

namespace ah {
namespace io {
namespace serial {

SerialMsgDispatcher::SerialMsgDispatcher() :
	_read_buf_msg_len(0),
	_bytes_received(0)
{
	end_of_line_char(FOOTER_EOF);
}

SerialMsgDispatcher::~SerialMsgDispatcher()
{
}

void SerialMsgDispatcher::async_read_some()
{
    if (_serial_port.get() == NULL || !_serial_port->is_open())
        return;

    _serial_port->async_read_some(
                boost::asio::buffer(_read_buf_raw, SERIAL_PORT_READ_BUF_SIZE),
                boost::bind(
                    &SerialMsgDispatcher::on_receive,
                    this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void SerialMsgDispatcher::on_receive(const boost::system::error_code& ec, size_t bytes_transferred)
{
    boost::recursive_mutex::scoped_lock lock(_mutex_read);

    if (_serial_port.get() == NULL || !_serial_port->is_open())
        return;

    if (ec) {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
        async_read_some();
        return;
    }

    //std::cout << "SerialMsgDispatcher::on_receive(" << bytes_transferred << ") ";
    //for (size_t i=0; i<bytes_transferred; i++) std::cout << " 0x" << std::hex << (int)((unsigned char*)_read_buf_raw)[i];
    //std::cout << std::endl;

	for (unsigned int i=0; i<bytes_transferred; i++) {
		char c = _read_buf_raw[i];
		_read_buf_msg[_bytes_received++] = c;

		if (_bytes_received == 1 && _read_buf_msg[0] != (char)HEADER_SOF) {
			_bytes_received = 0;
			continue;
		}
		else if (_bytes_received == HEADER_LEN) {
			_read_buf_msg_len = ((SerialMsg::SerialMsgHeader*)_read_buf_msg)->_payload_len + HEADER_LEN + FOOTER_LEN;
		}
		else if (_bytes_received == _read_buf_msg_len) {
			if (_recvMsg.unpack(_read_buf_msg, _read_buf_msg_len))
				on_receive(_recvMsg);
			_bytes_received = 0;
			_read_buf_msg_len = 0;
		}
	}

    async_read_some();
}

void SerialMsgDispatcher::on_receive(SerialMsg &msg)
{
    //std::cout << "SerialMsgDispatcher::on_receive(SerialMsg&) : ";
	//msg.dump();
	//std::cout << std::endl;

	recv_map_t::iterator callback = _recv_callbacks.find(msg.content_id());
	if (callback != _recv_callbacks.end())
		callback->second(msg);
}

void SerialMsgDispatcher::subscribe(char content_id, std::function<void(SerialMsg&)> callback)
{
	_recv_callbacks.insert(std::pair<char, std::function<void(SerialMsg&)> >(content_id, callback));
}

int SerialMsgDispatcher::send(const SerialMsg& msg)
{
	return write_some(msg.buffer(), msg.buffer_len());
}

int SerialMsgDispatcher::pack_n_send(SerialMsg& msg)
{
	msg.pack();
	return write_some(msg.buffer(), msg.buffer_len());
}

} //namespace serial
} //namespace io
} //namespace ah
