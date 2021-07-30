/* Copyright 2017 Wonik Robotics. All rights reserved.
 *
 * This file is a proprietary of Wonik Robotics
 * and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF Wonik Robotics.
 */
#ifndef __SERIAL_CRC_H__
#define __SERIAL_CRC_H__

#include <cstddef> // std::size_t

namespace ah {
namespace io {
namespace serial {

extern "C" {
unsigned short crc16(char* buf, size_t size_in_bytes);
}

}
}
}

#endif // __SERIAL_CRC_H__
