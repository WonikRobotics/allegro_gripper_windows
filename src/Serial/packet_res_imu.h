/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_RES_IMU_H__
#define __PACKET_RES_IMU_H__

#include "packet_res.h"

namespace ah {

class PacketResIMU : public PacketRes
{
public:
	PacketResIMU() 
		: PacketRes(ID_RTR_IMU_DATA)
	{}
	virtual ~PacketResIMU() {}

	unsigned short x() const { return _wxyz[1]; }
	unsigned short y() const { return _wxyz[2]; }
	unsigned short z() const { return _wxyz[3]; }
	unsigned short w() const { return _wxyz[0]; }
	unsigned short* wxyz() const { return (unsigned short*)_wxyz; }

private:
	friend class ah::io::serial::SerialMsg;
	unsigned short _wxyz[4];
};

} // namespace wr

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator>>(::ah::PacketResIMU& val)
{
	val._content_id = this->_header._fid;
	return 
		*this >> val._wxyz[0]
		      >> val._wxyz[1]
			  >> val._wxyz[2]
			  >> val._wxyz[3];
}
}}}

#endif // __PACKET_RES_IMU_H__
