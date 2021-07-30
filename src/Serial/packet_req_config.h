/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_REQ_CONFIG_H__
#define __PACKET_REQ_CONFIG_H__

#include "packet_req.h"

namespace ah {

class PacketReqConfig : public PacketReq
{
public:
	PacketReqConfig(char flag, unsigned char device_id, unsigned int baudrate) 
		: PacketReq(ID_CMD_CONFIG)
		, _flag(flag)
		, _device_id(device_id)
		, _baudrate(baudrate)
	{
		
	}
	virtual ~PacketReqConfig() {}

private:
	friend class ah::io::serial::SerialMsg;
	char _flag;
	unsigned char _device_id;
	unsigned int _baudrate;
};

} // namespace ah

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator<<(::ah::PacketReqConfig const& val)
{
	this->_header._fid = val._content_id;
	return
		*this << val._flag 
		      << val._device_id 
		      << val._baudrate;
}
}}}

#endif // __PACKET_REQ_CONFIG_H__
