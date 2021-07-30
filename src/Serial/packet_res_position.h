/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_RES_POSITION_H__
#define __PACKET_RES_POSITION_H__

#include "packet_res.h"

namespace ah {

class PacketResJointPosition : public PacketRes
{
public:
	PacketResJointPosition() 
		: PacketRes(ID_RTR_FINGER_POSE)
	{}
	virtual ~PacketResJointPosition() {}

	short jpose(unsigned int fid, unsigned int jid) const { return _jpose[fid * 4 + jid]; }
	short* jpose() const { return (short*)_jpose; }

private:
	friend class ah::io::serial::SerialMsg;
	short _jpose[16];
};

} // namespace wr

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator>>(::ah::PacketResJointPosition& val)
{
	val._content_id = this->_header._fid;
	return 
		*this >> val._jpose[0] >> val._jpose[1] >> val._jpose[2] >> val._jpose[3]
		      >> val._jpose[4] >> val._jpose[5] >> val._jpose[6] >> val._jpose[7]
			  >> val._jpose[8] >> val._jpose[9] >> val._jpose[10] >> val._jpose[11]
			  >> val._jpose[12] >> val._jpose[13] >> val._jpose[14] >> val._jpose[15];
}
}}}

#endif // __PACKET_RES_POSITION_H__
