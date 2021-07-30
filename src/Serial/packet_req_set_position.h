/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_REQ_SET_POSITION_H__
#define __PACKET_REQ_SET_POSITION_H__

#include "packet_req.h"

namespace ah {

class PacketReqSetPosition : public PacketReq
{
public:
	PacketReqSetPosition(short* jpose) 
		: PacketReq(ID_CMD_SET_POSE)
	{
		memcpy(_jpose, jpose, sizeof(_jpose[0]) * 16);
	}
	virtual ~PacketReqSetPosition() {}

private:
	friend class ah::io::serial::SerialMsg;
	short _jpose[16];
};

} // namespace ah

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator<<(::ah::PacketReqSetPosition const& val)
{
	this->_header._fid = val._content_id;
	return
		*this << val._jpose[0] << val._jpose[1] << val._jpose[2] << val._jpose[3]
			  << val._jpose[4] << val._jpose[5] << val._jpose[6] << val._jpose[7]
			  << val._jpose[8] << val._jpose[9] << val._jpose[10] << val._jpose[11]
			  << val._jpose[12] << val._jpose[13] << val._jpose[14] << val._jpose[15];
}
}}}

#endif // __PACKET_REQ_SET_POSITION_H__
