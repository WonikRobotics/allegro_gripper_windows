/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_REQ_SET_PERIOD_H__
#define __PACKET_REQ_SET_PERIOD_H__

#include "packet_req.h"

namespace ah {

	class PacketReqSetPeriod : public PacketReq
	{
	public:
		PacketReqSetPeriod(unsigned short ms_position, unsigned short ms_imu, unsigned short ms_temperature, unsigned short ms_status)
			: PacketReq(ID_CMD_SET_PERIOD)
			, _ms_position(ms_position)
			, _ms_imu(ms_imu)
			, _ms_temperature(ms_temperature)
			, _ms_status(ms_status)
		{}
		virtual ~PacketReqSetPeriod() {}

	private:
		friend class ah::io::serial::SerialMsg;
		unsigned short _ms_position;
		unsigned short _ms_imu;
		unsigned short _ms_temperature;
		unsigned short _ms_status;
	};

} // namespace ah

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator<<(::ah::PacketReqSetPeriod const& val)
{
	this->_header._fid = val._content_id;
	return
		*this << val._ms_position 
				<< val._ms_imu 
				<< val._ms_temperature 
				<< val._ms_status;
}
}}}

#endif // __PACKET_REQ_SET_PERIOD_H__
