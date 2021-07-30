/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_REQ_SET_MOTION_H__
#define __PACKET_REQ_SET_MOTION_H__

#include "packet_req.h"

namespace ah {

class PacketReqSetMotion : public PacketReq
{
public:
	PacketReqSetMotion(char mid) 
		: PacketReq(ID_CMD_SET_MOTION)
		, _motion_id(mid)
	{}
	virtual ~PacketReqSetMotion() {}

private:
	friend class ah::io::serial::SerialMsg;
	char _motion_id;
};

} // namespace ah

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator<<(::ah::PacketReqSetMotion const& val)
{
	this->_header._fid = val._content_id;
	return
		*this << val._motion_id;
}
}}}

#endif // __PACKET_REQ_SET_MOTION_H__
