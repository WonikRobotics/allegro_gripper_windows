/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_RES_HAND_INFO_H__
#define __PACKET_RES_HAND_INFO_H__

#include "packet_res.h"

namespace ah {

class PacketResHandInfo : public PacketRes
{
public:
	PacketResHandInfo() 
		: PacketRes(ID_RTR_HAND_INFO)
	{}
	virtual ~PacketResHandInfo() {}

	unsigned short hw_ver() const { return _hw_ver; }
	unsigned short fw_ver() const { return _fw_ver; }
	char is_left() const { return _left_hand;  }
	char temperature() const { return _temperature;  }
	char status() const { return _status;  }

private:
	friend class ah::io::serial::SerialMsg;
	unsigned short _hw_ver;
	unsigned short _fw_ver;
	char _left_hand;
	char _temperature;
	char _status;
};

} // namespace wr

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator>>(::ah::PacketResHandInfo& val)
{
	val._content_id = this->_header._fid;
	return 
		*this >> val._hw_ver
		      >> val._fw_ver
			  >> val._left_hand
			  >> val._temperature
			  >> val._status;
}
}}}

#endif // __PACKET_RES_HAND_INFO_H__
