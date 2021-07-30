/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_H__
#define __PACKET_H__

#include "serial_msg.h"
#include "canDef.h"

namespace ah {

class Packet
{
public:
	Packet(char content_id) 
		: _content_id(content_id)
	{}
	Packet() {}
	virtual ~Packet() {}

	char content_id() { return _content_id; }

private:
	friend class ah::io::serial::SerialMsg;
	char _content_id;
};

} // namespace ah

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator<<(::ah::Packet const& val)
{
	this->_header._fid = val._content_id;
	return *this;
}

template<>
inline SerialMsg& SerialMsg::operator>>(::ah::Packet& val)
{
	val._content_id = this->_header._fid;
	return *this;
}
}}}

#include "packet_req.h"
#include "packet_req_config.h"
#include "packet_req_set_motion.h"
#include "packet_req_set_period.h"
#include "packet_req_set_position.h"
#include "packet_req_set_torque.h"
#include "packet_res.h"
#include "packet_res_hand_info.h"
#include "packet_res_imu.h"
#include "packet_res_position.h"
#include "packet_res_serial_no.h"
#include "packet_res_temperature.h"

#endif // __PACKET_H__
