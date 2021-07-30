/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __SERIAL_MSG_DISPATCHER_H__
#define __SERIAL_MSG_DISPATCHER_H__

#include "serial_port.h"
#include "serial_msg.h"
#include <map>
#include <functional>

namespace ah {
namespace io {
namespace serial {

//-----------------------------------------------------
// Serial Port with Message Displatcher
class SerialMsgDispatcher : public SerialPort
{
public:
    SerialMsgDispatcher();
    virtual ~SerialMsgDispatcher();

public:
	void subscribe(char content_id, std::function<void(SerialMsg&)> callback);
	int send(const SerialMsg& msg);
	int pack_n_send(SerialMsg& msg);

protected:
	virtual void async_read_some();
    virtual void on_receive(const boost::system::error_code& ec, size_t bytes_transferred);
    virtual void on_receive(SerialMsg &msg);

private:
	SerialMsg _recvMsg;
	char _read_buf_msg[SERIAL_PORT_READ_BUF_SIZE];
	unsigned int _read_buf_msg_len;
	unsigned int _bytes_received;
	typedef std::map<char, std::function<void(SerialMsg&)> > recv_map_t;
	recv_map_t _recv_callbacks;
};

} //namespace serial
} //namespace io
} //namespace ah

#endif // __SERIAL_MSG_DISPATCHER_H__
