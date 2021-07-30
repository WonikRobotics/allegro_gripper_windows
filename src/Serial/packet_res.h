/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_RES_H__
#define __PACKET_RES_H__

#include "packet.h"

namespace ah {

class PacketRes : public Packet
{
public:
	PacketRes(char content_id) 
		: Packet(content_id)
	{}
	virtual ~PacketRes() {}

private:
	friend class ah::io::serial::SerialMsg;
};

class PacketSimpleRes : public PacketRes
{
public:
	PacketSimpleRes(char content_id, unsigned char result) 
		: PacketRes(content_id)
		, _res_code(result)
	{}
	virtual ~PacketSimpleRes() {}

private:
	friend class ah::io::serial::SerialMsg;
	unsigned char _res_code;
};

#define DECLARE_PKT_SIMPLE_RES(cname, cid)                      \
	class PacketRes##cname : public PacketSimpleRes	{           \
		public: PacketRes##cname();								\
	};

DECLARE_PKT_SIMPLE_RES(SystemOn, ID_CMD_SYSTEM_ON)
DECLARE_PKT_SIMPLE_RES(SystemOff, ID_CMD_SYSTEM_OFF)
DECLARE_PKT_SIMPLE_RES(SetTorque, ID_CMD_SET_TORQUE)
DECLARE_PKT_SIMPLE_RES(SetPosition, ID_CMD_SET_POSE)
DECLARE_PKT_SIMPLE_RES(SetMotion, ID_CMD_SET_MOTION)
DECLARE_PKT_SIMPLE_RES(SetPeriod, ID_CMD_SET_PERIOD)
DECLARE_PKT_SIMPLE_RES(Config, ID_CMD_CONFIG)

} // namespace ah

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator>>(::ah::PacketRes& val)
{
	val._content_id = this->_header._fid;
	return *this;
}

template<>
inline SerialMsg& SerialMsg::operator>>(::ah::PacketSimpleRes& val)
{
	val._content_id = this->_header._fid;
	return 
		*this >> val._res_code;
}
}}}

#endif // __PACKET_RES_H__
