/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_REQ_SET_TORQUE_H__
#define __PACKET_REQ_SET_TORQUE_H__

#include "packet_req.h"

namespace ah {

class PacketReqSetTorque : public PacketReq
{
public:
	PacketReqSetTorque(short* tau) 
		: PacketReq(ID_CMD_SET_TORQUE)
	{
		memcpy(_tau, tau, sizeof(_tau[0]) * 16);
	}
	virtual ~PacketReqSetTorque() {}

private:
	friend class ah::io::serial::SerialMsg;
	short _tau[16];
};

} // namespace ah

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator<<(::ah::PacketReqSetTorque const& val)
{
	this->_header._fid = val._content_id;
	return
		*this << val._tau[0] << val._tau[1] << val._tau[2] << val._tau[3]
		<< val._tau[4] << val._tau[5] << val._tau[6] << val._tau[7]
		<< val._tau[8] << val._tau[9] << val._tau[10] << val._tau[11]
		<< val._tau[12] << val._tau[13] << val._tau[14] << val._tau[15];
}
}}}

#endif // __PACKET_REQ_SET_TORQUE_H__
