/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_RES_TEMPERATURE_H__
#define __PACKET_RES_TEMPERATURE_H__

#include "packet_res.h"

namespace ah {

class PacketResTemperature : public PacketRes
{
public:
	PacketResTemperature() 
		: PacketRes(ID_RTR_TEMPERATURE)
	{}
	virtual ~PacketResTemperature() {}

	char temperature(unsigned int fid, unsigned int jid) const { return _temperature[fid * 4 + jid];  }
	char* temperature() const { return (char*)_temperature; }

private:
	friend class ah::io::serial::SerialMsg;
	char _temperature[16];
};

} // namespace wr

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator>>(::ah::PacketResTemperature& val)
{
	val._content_id = this->_header._fid;
	return 
		*this >> val._temperature[0] >> val._temperature[1] >> val._temperature[2] >> val._temperature[3]
		      >> val._temperature[4] >> val._temperature[5] >> val._temperature[6] >> val._temperature[7]
			  >> val._temperature[8] >> val._temperature[9] >> val._temperature[10] >> val._temperature[11]
			  >> val._temperature[12] >> val._temperature[13] >> val._temperature[14] >> val._temperature[15];
}
}}}

#endif // __PACKET_RES_HAND_INFO_H__
