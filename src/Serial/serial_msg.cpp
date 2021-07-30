/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#include <iostream>
#include <cstring>
#include "serial_msg.h"
#include "crc.h"

namespace ah {
namespace io {
namespace serial {

SerialMsg::SerialMsg()
{
	reset();
}

SerialMsg::SerialMsg(const SerialMsg& msg)
{
	reset();
	memcpy(_buf, msg._buf, SERIAL_MSG_BUF_SIZE);
	_header = msg._header;
	_crc_h = msg._crc_h;
	_crc_l = msg._crc_l;
}

SerialMsg::~SerialMsg()
{
}

SerialMsg& SerialMsg::begin()
{
	_buf_ptr = payload();
	return *this;
}

bool SerialMsg::pack()
{
	_header._payload_len = _buf_ptr - payload();
	memcpy(_buf, &_header, HEADER_LEN);

	unsigned short crc = crc16(_buf, (size_t)(_header._payload_len + HEADER_LEN));
	_buf_ptr[0] = _crc_h = (0xff00 & crc) >> 8;
	_buf_ptr[1] = _crc_l = (0x00ff & crc);
	_buf_ptr[2] = FOOTER_EOF;

	return true;
}

bool SerialMsg::pack(const char*& buf, size_t &size_in_bytes)
{
	if (!pack())
		return false;

	buf = buffer();
	size_in_bytes = buffer_len();
	return true;
}

bool SerialMsg::unpack(const char* buf, size_t size_in_bytes)
{
	if (size_in_bytes > SERIAL_MSG_BUF_SIZE)
		return false; // invalid size

	if (buf[size_in_bytes - 1] != FOOTER_EOF)
		return false; // invalid EOF character

	unsigned short crc_check = crc16((char*)buf, size_in_bytes);
	if (crc_check)
		return false; // crc error!

	memcpy(_buf, buf, size_in_bytes);
	memcpy(&_header, _buf, HEADER_LEN);
	_crc_h = _buf[size_in_bytes - 3];
	_crc_l = _buf[size_in_bytes - 2];
	_eof = _buf[size_in_bytes - 1];

	return true;
}

SerialMsg& SerialMsg::reset()
{
	_header._payload_len = 0;
	_buf_ptr = payload();
	_buf_end = (char*)_buf + SERIAL_MSG_BUF_SIZE - FOOTER_LEN;
	return *this;
}

const char* SerialMsg::buffer() const
{
	return (const char*)_buf;
}

size_t SerialMsg::buffer_len() const
{
	return (size_t)(_header._payload_len + HEADER_LEN + FOOTER_LEN);
}

char* SerialMsg::payload() const
{
	return (char*)_buf + HEADER_LEN;
}

char SerialMsg::payload_len() const
{
	return _header._payload_len;
}

void SerialMsg::payload_len(const char len)
{
	_header._payload_len = len;
}

char SerialMsg::device_id() const
{
	return _header._did;
}

char SerialMsg::content_id() const
{
	return _header._fid;
}

#ifdef _DEBUG
void SerialMsg::dump() const
{
	size_t n = 0;
	for (auto h : _buf) {
		if (n >= buffer_len()) break;
		std::cout << "0x" << std::hex << (int)((unsigned char*)_buf)[n++] << " ";
	}
	std::cout << std::endl;
}
#endif

} //namespace serial
} //namespace io
} //namespace ah
