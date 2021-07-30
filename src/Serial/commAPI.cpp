

/*======================*/
/*       Includes       */
/*======================*/
//system headers
#include <stdio.h>
#include <errno.h>
#ifndef _WIN32
#include <inttypes.h>
#include <pthread.h>
#include <syslog.h>
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <malloc.h>
#include <assert.h>

#include <iostream>
#include <functional>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/thread.hpp>

#include "canDef.h"
#include "canAPI.h"
#include "packet.h"
#include "serial_msg_dispatcher.h"

using namespace ah;
using namespace ah::io::serial;

CANAPI_BEGIN

#define COMM_ERROR_OK		(0)
#define COMM_ERROR			(-1)

/*=========================================*/
/*       Global file-scope variables       */
/*=========================================*/
static SerialMsgDispatcher	_serial;
static unsigned char		_device_id = 0;

/*==========================================*/
/*       MACRO (serial message handling)    */
/*==========================================*/
#define DECLARE_SERIAL_MESSAGE_HANDLER(msg_name)        \
    void onSerial##msg_name(PacketRes##msg_name &)

#define DEFINE_SERIAL_MESSAGE_HANDLER(msg_name, msg_id) \
    case (msg_id):                                      \
    {                                                   \
        PacketRes##msg_name res;                        \
        msg.begin() >> res;                             \
        onSerial##msg_name(res);                        \
    }                                                   \
    break;                                              \

#define SUBSCRIBE_SERIAL_MESSAGE(msg_id)                \
    _serial.subscribe(msg_id, std::bind(&onSerialMsg, std::placeholders::_1));

/*==========================================*/
/*       Private functions prototypes       */
/*==========================================*/
void onSerialSystemOn(PacketResSystemOn &msg)
{

}
void onSerialSystemOff(PacketResSystemOff &msg)
{

}
void onSerialHandInfo(PacketResHandInfo &msg)
{

}
void onSerialSerialNo(PacketResSerialNo &msg)
{

}

void onSerialMsg(SerialMsg &msg)
{
	char content_id;
	Packet response;
	msg.begin() >> response;

	switch (response.content_id())
	{
		DEFINE_SERIAL_MESSAGE_HANDLER(SystemOn, ID_CMD_SYSTEM_ON)
		DEFINE_SERIAL_MESSAGE_HANDLER(SystemOff, ID_CMD_SYSTEM_OFF)
		//DEFINE_SERIAL_MESSAGE_HANDLER(SetTorque, ID_CMD_SET_TORQUE)
		//DEFINE_SERIAL_MESSAGE_HANDLER(SetPosition, ID_CMD_SET_POSE)
		//DEFINE_SERIAL_MESSAGE_HANDLER(SetPeriod, ID_CMD_SET_PERIOD)
		//DEFINE_SERIAL_MESSAGE_HANDLER(Config, ID_CMD_CONFIG)
		//DEFINE_SERIAL_MESSAGE_HANDLER(SetMotion, ID_CMD_SET_MOTION)
		DEFINE_SERIAL_MESSAGE_HANDLER(HandInfo, ID_RTR_HAND_INFO)
		DEFINE_SERIAL_MESSAGE_HANDLER(SerialNo, ID_RTR_SERIAL)
		//DEFINE_SERIAL_MESSAGE_HANDLER(JointPosition, ID_RTR_FINGER_POSE)
		//DEFINE_SERIAL_MESSAGE_HANDLER(IMU, ID_RTR_IMU_DATA)
		//DEFINE_SERIAL_MESSAGE_HANDLER(Temperature, ID_RTR_TEMPERATURE)
	}
}

int commReadMsg(int bus, int *id, int *len, unsigned char *data, int blocking) {
	TPCANMsg CANMsg;
	TPCANTimestamp CANTimeStamp;
	TPCANStatus Status = PCAN_ERROR_OK;
	char strMsg[256];
	int i;

	// We execute the "Read" function of the PCANBasic
	Status = CAN_Read(canDev[bus], &CANMsg, &CANTimeStamp);
	if (Status != PCAN_ERROR_OK)
	{
		if (Status != PCAN_ERROR_QRCVEMPTY)
		{
			CAN_GetErrorText(Status, 0, strMsg);
			printf("canReadMsg(): CAN_Read() failed with error %u\n", Status);
			printf("%s\n", strMsg);
		}

		return Status;
	}

	*id = (CANMsg.ID & 0xfffffffc) >> 2;
	*len = CANMsg.LEN;
	for (i = 0; i < CANMsg.LEN; i++)
		data[i] = CANMsg.DATA[i];

	return COMM_ERROR_OK;
}

int commSendMsg(int bus, SerialMsg msg, int blocking)
{
	int rtn = _serial.pack_n_send(msg);
	if (rtn == 0)
	{
		printf("commSend(): failed to send a message\n");
		return COMM_ERROR;
	}
	return COMM_ERROR_OK;
}


/*========================================*/
/*       Public functions (COMM API)      */
/*========================================*/
int command_can_open(int ch)
{
	assert(ch >= 0 && ch < MAX_BUS);

	// Initialize Serial communication:
	SUBSCRIBE_SERIAL_MESSAGE(ID_CMD_SYSTEM_ON);
	SUBSCRIBE_SERIAL_MESSAGE(ID_CMD_SYSTEM_OFF);
	//SUBSCRIBE_SERIAL_MESSAGE(ID_CMD_SET_TORQUE);
	//SUBSCRIBE_SERIAL_MESSAGE(ID_CMD_SET_POSE);
	//SUBSCRIBE_SERIAL_MESSAGE(ID_CMD_SET_PERIOD);
	//SUBSCRIBE_SERIAL_MESSAGE(ID_CMD_CONFIG);
	//SUBSCRIBE_SERIAL_MESSAGE(ID_CMD_SET_MOTION);
	SUBSCRIBE_SERIAL_MESSAGE(ID_RTR_HAND_INFO);
	SUBSCRIBE_SERIAL_MESSAGE(ID_RTR_SERIAL);
	//SUBSCRIBE_SERIAL_MESSAGE(ID_RTR_FINGER_POSE);
	//SUBSCRIBE_SERIAL_MESSAGE(ID_RTR_IMU_DATA);
	//SUBSCRIBE_SERIAL_MESSAGE(ID_RTR_TEMPERATURE);

	std::string com_port_name = "COM3";
	int baud_rate = 460800;

	printf("<< Comm: Open Channel...\n");
	bool rtn = _serial.start(com_port_name.c_str() , baud_rate);
	if (rtn == false)
	{
		printf("<< Comm: Error!\n");
	}

	SerialMsg send_msg;
	send_msg.begin() << PacketReqHandInfo(); commSendMsg(ch, send_msg, TRUE);
	send_msg.begin() << PacketReqSerialNo(); commSendMsg(ch, send_msg, TRUE);

	printf("\t- Done\n");
	return COMM_ERROR_OK;
}

int command_can_open_ex(int ch, int type, int index)
{
	return command_can_open(ch);
}

int command_can_reset(int ch)
{
	return -1;
}

int command_can_close(int ch)
{
	assert(ch >= 0 && ch < MAX_BUS);

	printf("<< Comm: Close...\n");
	
	_serial.stop();

	printf("\t- Done\n");
	return COMM_ERROR_OK;
}

int command_can_set_id(int ch, unsigned char can_id)
{
	_device_id = can_id;
	return COMM_ERROR_OK;
}

int command_servo_on(int ch)
{
	assert(ch >= 0 && ch < MAX_BUS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqSystemOn();
	return commSendMsg(ch, send_msg, TRUE);
}

int command_servo_off(int ch)
{
	assert(ch >= 0 && ch < MAX_BUS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqSystemOff();
	return commSendMsg(ch, send_msg, TRUE);
}

int command_set_torque(int ch, int findex, short* pwm)
{
	assert(ch >= 0 && ch < MAX_BUS);
	assert(findex >= 0 && findex < NUM_OF_FINGERS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqSetTorque(pwm);
	return commSendMsg(ch, send_msg, TRUE);
}

int command_set_pose(int ch, int findex, short* jposition)
{
	assert(ch >= 0 && ch < MAX_BUS);
	assert(findex >= 0 && findex < NUM_OF_FINGERS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqSetPosition(jposition);
	return commSendMsg(ch, send_msg, TRUE);
}

int command_set_period(int ch, short* period)
{
	assert(ch >= 0 && ch < MAX_BUS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqSetPeriod(period[0], period[1], period[2], period[3]);
	return commSendMsg(ch, send_msg, TRUE);
}

int command_set_device_id(int ch, unsigned char did)
{
	assert(ch >= 0 && ch < MAX_BUS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqConfig(0x01, did, 0);
	return commSendMsg(ch, send_msg, TRUE);
}

int command_set_rs485_baudrate(int ch, unsigned int baudrate)
{
	assert(ch >= 0 && ch < MAX_BUS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqConfig(0x02, 0, baudrate);
	return commSendMsg(ch, send_msg, TRUE);
}

int request_hand_information(int ch)
{
	assert(ch >= 0 && ch < MAX_BUS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqHandInfo();
	return commSendMsg(ch, send_msg, TRUE);
}

int request_hand_serial(int ch)
{
	assert(ch >= 0 && ch < MAX_BUS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqSerialNo();
	return commSendMsg(ch, send_msg, TRUE);
}

int request_finger_pose(int ch, int findex)
{
	assert(ch >= 0 && ch < MAX_BUS);
	assert(findex >= 0 && findex < NUM_OF_FINGERS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqEncoder();
	return commSendMsg(ch, send_msg, TRUE);
}

int request_imu_data(int ch)
{
	assert(ch >= 0 && ch < MAX_BUS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqIMU();
	return commSendMsg(ch, send_msg, TRUE);
}

int request_temperature(int ch, int sindex)
{
	assert(ch >= 0 && ch < MAX_BUS);
	assert(sindex >= 0 && sindex < NUM_OF_TEMP_SENSORS);

	SerialMsg send_msg;
	send_msg.begin() << PacketReqTemperature();
	return commSendMsg(ch, send_msg, TRUE);
}

int get_message(int ch, int* id, int* len, unsigned char* data, int blocking)
{
	/*int err;
	err = canReadMsg(ch, id, len, data, blocking);*/
	return -1;
}


CANAPI_END
