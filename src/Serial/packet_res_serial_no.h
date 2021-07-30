/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_RES_SERIAL_H__
#define __PACKET_RES_SERIAL_H__

#include "packet_res.h"

namespace ah {

class PacketResSerialNo : public PacketRes
{
public:
	PacketResSerialNo() 
		: PacketRes(ID_RTR_SERIAL)
	{
		_serial_no[8] = '\0';
	}
	virtual ~PacketResSerialNo() {}

	unsigned char* serial_no() const { return (unsigned char*)_serial_no; }

private:
	friend class ah::io::serial::SerialMsg;
	unsigned char _serial_no[9]; // "SAH0x0________"
};

} // namespace wr

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator>>(::ah::PacketResSerialNo& val)
{
	val._content_id = this->_header._fid;
	return
		*this >> val._serial_no[0] >> val._serial_no[1] >> val._serial_no[2] >> val._serial_no[3]
			  >> val._serial_no[4] >> val._serial_no[5] >> val._serial_no[6] >> val._serial_no[7];
}
}}}

#endif // __PACKET_RES_SERIAL_H__
