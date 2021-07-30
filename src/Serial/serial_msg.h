/* Copyright Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __SERIAL_MSG_H__
#define __SERIAL_MSG_H__

#include <string.h> //memcpy

// ---------------------------------------------------------------------
// protocol header constants
//
#define HEADER_SOF          0xA0
#define HEADER_LEN          4


// ---------------------------------------------------------------------
// protocol footer constants
//
#define FOOTER_EOF			0xED
#define CRC_LEN				2
#define FOOTER_LEN			3

// ---------------------------------------------------------------------
// other constants
//
#define SERIAL_MSG_BUF_SIZE 256


namespace ah {
namespace io {
namespace serial {

//-----------------------------------------------------
// Serial Message
class SerialMsg
{
public:
	SerialMsg();
	SerialMsg(const SerialMsg& msg);
    virtual ~SerialMsg();

	SerialMsg& begin(); // get ready to read data from message.

	bool pack(); // calculates payload length and CRC checksum. set those at the apropriate buffer position and get ready to send it out to data stream.
    bool pack(const char*& buf, size_t &size_in_bytes); // calculates payload length and CRC checksum. set those at the apropriate buffer position and get ready to send it out to data stream.
	bool unpack(const char* buf, size_t size_in_bytes); // check CRC and unpack buffer to construct message header.

	SerialMsg& reset(); // reset message header and the position for read/write to the begining of message buffer.

	const char* buffer() const; // returns buffer.
	size_t buffer_len() const; // returns buffer length of whole message frame including header, payload, and crc.

	char payload_len() const; // returns the length of payload.
	void payload_len(const char len); // manually set the length of payload.
	char device_id() const; // returns device id.
	char content_id() const; // returns content(function) id. returns -1 if it is a heartbeat message.
	

protected:
	char* payload() const; // returns payload buffer.

public: // data manipulation api's:
	/** 
	 * Overloaded operator for writing various types of data into this message.
	 * @code 
	 * SerialMsg msg;
	 * msg.allocate();
	 * msg.begin();
	 * char a='a'; short b=1; int c=2; double d=2.3;
	 * msg << a << b << c << d ;
	 * msg.end();
	 * @endcode
	 * @param[in] val Data to write.
	 * @return It returns the reference to this instance.
	 * @see operator<<(const char* const& str), operator<<(char* const& str), operator<<(const std::string& str)
	 */
    template<typename T>
	SerialMsg&		operator<<(T const& val)
	{
		size_t size = sizeof(T);
		if (_buf_ptr + size > _buf_end)
			return *this;

        memcpy(_buf_ptr, &val, size);
		_buf_ptr += size;
		return *this;
	}

	/** 
	 * Overloaded operator for reading various types of data from this message.
	 * @param[out] val Data read from this message.
	 * @return It returns the reference to this instance.
	 * @see operator>>(std::string& str), operator>>(char *&buff)
	 */
	template<typename T>
	SerialMsg&		operator>>(T& val)
	{
		size_t size = sizeof(T);
		if (_buf_ptr + size > _buf_end)
			return *this;

        memcpy(&val, _buf_ptr, size);
		_buf_ptr += size;

		return *this;
	}

public: // message frame buffer definition:
	typedef struct SerialMsgHeader_
	{
		char _sof;
        char _did;
		char _fid;
        char _payload_len;

		SerialMsgHeader_() :
			_sof((char)HEADER_SOF),
			_payload_len(0)
		{}
	} SerialMsgHeader;

private:
	char _buf[SERIAL_MSG_BUF_SIZE]; // message buffer including message header.
	char *_buf_ptr; // current read/write position.
	char *_buf_end;
	SerialMsgHeader _header;
	char _crc_h; // checksum is encoded as big-endian!
	char _crc_l;
	char _eof;

public:
#ifdef _DEBUG
	void dump() const; // dump this message.
#else
	void dump() const {}
#endif
};

} //namespace serial
} //namespace io
} //namespace ah

#endif // __SERIAL_MSG_H__
