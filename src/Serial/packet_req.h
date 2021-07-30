/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __PACKET_REQ_H__
#define __PACKET_REQ_H__

#include "packet.h"

namespace ah {

class PacketReq : public Packet
{
public:
    PacketReq(char content_id) 
		: Packet(content_id)
	{}
    virtual ~PacketReq() {}

private:
	friend class ah::io::serial::SerialMsg;
};

#define DECLARE_PKT_SIMPLE_REQ(cname, cid)                      \
    class PacketReq##cname : public PacketReq	{               \
		friend class ah::io::serial::SerialMsg;					\
        public: PacketReq##cname() : PacketReq(cid) {}	        \
	};

DECLARE_PKT_SIMPLE_REQ(SystemOn, ID_CMD_SYSTEM_ON)
DECLARE_PKT_SIMPLE_REQ(SystemOff, ID_CMD_SYSTEM_OFF)
DECLARE_PKT_SIMPLE_REQ(HandInfo, ID_RTR_HAND_INFO)
DECLARE_PKT_SIMPLE_REQ(SerialNo, ID_RTR_SERIAL)
DECLARE_PKT_SIMPLE_REQ(Encoder, ID_RTR_FINGER_POSE)
DECLARE_PKT_SIMPLE_REQ(IMU, ID_RTR_IMU_DATA)
DECLARE_PKT_SIMPLE_REQ(Temperature, ID_RTR_TEMPERATURE)

} // namespace ah

namespace ah {
namespace io {
namespace serial {
template<>
inline SerialMsg& SerialMsg::operator<<(::ah::PacketReq const& val)
{
	this->_header._fid = val._content_id;
	return *this;
}
}}}

#define DEFINE_PKT_SIMPLE_REQ(cname, cid)						\
    namespace ah {												\
	namespace io {												\
	namespace serial {											\
	template<>													\
    inline SerialMsg& SerialMsg::operator <<					\
	(::ah::PacketReq##cname const& val) {						\
		this->_header._fid = val._content_id;					\
		return *this;											\
	}															\
	}}}

DEFINE_PKT_SIMPLE_REQ(SystemOn, ID_CMD_SYSTEM_ON)
DEFINE_PKT_SIMPLE_REQ(SystemOff, ID_CMD_SYSTEM_OFF)
DEFINE_PKT_SIMPLE_REQ(HandInfo, ID_RTR_HAND_INFO)
DEFINE_PKT_SIMPLE_REQ(SerialNo, ID_RTR_SERIAL)
DEFINE_PKT_SIMPLE_REQ(Encoder, ID_RTR_FINGER_POSE)
DEFINE_PKT_SIMPLE_REQ(IMU, ID_RTR_IMU_DATA)
DEFINE_PKT_SIMPLE_REQ(Temperature, ID_RTR_TEMPERATURE)

#endif // __PACKET_REQ_H__
