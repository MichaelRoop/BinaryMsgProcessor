// BinaryMsgMessages.h

#ifndef _BinaryMsgMessages_h
#define _BinaryMsgMessages_h

#include "BinaryMsgEnumerations.h"

//#include <Arduino.h>

//#include <cstdint>

////https://github.com/mcci-catena/MCCI-Catena-HS300x/issues/1
//#ifdef AVR // UNO etc
//# define NO_CSTDINT 1  // AVR arduino has no <cstdint>; but we're coding to portable C++. So substitute.
//#endif
//
//// unless we know otherwise, use the compiler's <cstdint>
//#ifndef NO_CSTDINT
//#include <cstdint>
//#else
//// no <cstdint> -- make sure std:: contains the things we need.
//# include <stdint.h>
//
//namespace std {
//	using ::int8_t;
//	using ::uint8_t;
//
//	using ::int16_t;
//	using ::uint16_t;
//
//	using ::int32_t;
//	using ::uint32_t;
//}
//
//#endif

// Handles DUE for printf
////https://forum.arduino.cc/t/compiler-confusion-headers/569022/2
//#ifdef __AVR__
//extern "C"
//{
//#include <stdio.h>
//}
//#else
//#include <stdio.h>
//#include <stdarg.h> // ARM tools don't include this in stdio.h WTF!
//#endif

//#include <stdint.h>


// Binary message template for all messages
template<typename T> struct MsgBinary {
public:
	uint8_t SOH = _SOH;
	uint8_t STX = _STX;
	uint16_t Size = 0;
	uint8_t DataType = typeUndefined;
	uint8_t Id = 0;
	T Value = 0;
	uint8_t ETX = _ETX;
	uint8_t EOT = _EOT;

	MsgBinary();
	MsgBinary(uint8_t id, T value);
private:
	void Init();
} __attribute__((packed));

//// Declare specializations on the Init
template<> void MsgBinary<bool>::Init();
template<> void MsgBinary<int8_t>::Init();
template<> void MsgBinary<int16_t>::Init();
template<> void MsgBinary<int32_t>::Init();
template<> void MsgBinary<uint8_t>::Init();
template<> void MsgBinary<uint16_t>::Init();
template<> void MsgBinary<uint32_t>::Init();
template<> void MsgBinary<float>::Init();

// Declare specializations on the constructor
template<> MsgBinary<bool>::MsgBinary();
template<> MsgBinary<int8_t>::MsgBinary();
template<> MsgBinary<int16_t>::MsgBinary();
template<> MsgBinary<int32_t>::MsgBinary();
template<> MsgBinary<uint8_t>::MsgBinary();
template<> MsgBinary<uint16_t>::MsgBinary();
template<> MsgBinary<uint32_t>::MsgBinary();
template<> MsgBinary<float>::MsgBinary();

template<> MsgBinary<bool>::MsgBinary(uint8_t id, bool value);
template<> MsgBinary<int8_t>::MsgBinary(uint8_t id, int8_t value);
template<> MsgBinary<int16_t>::MsgBinary(uint8_t id, int16_t value);
template<> MsgBinary<int32_t>::MsgBinary(uint8_t id, int32_t value);
template<> MsgBinary<uint8_t>::MsgBinary(uint8_t id, uint8_t value);
template<> MsgBinary<uint16_t>::MsgBinary(uint8_t id, uint16_t value);
template<> MsgBinary<uint32_t>::MsgBinary(uint8_t id, uint32_t value);
template<> MsgBinary<float>::MsgBinary(uint8_t id, float value);


// typedefs - pre packed via main template class
typedef MsgBinary<bool> MsgBool;
typedef MsgBinary<int8_t> MsgInt8;
typedef MsgBinary<int16_t> MsgInt16;
typedef MsgBinary<int32_t> MsgInt32;
typedef MsgBinary<uint8_t> MsgUInt8;
typedef MsgBinary<uint16_t> MsgUInt16;
typedef MsgBinary<uint32_t> MsgUInt32;
typedef MsgBinary<float> MsgFloat32;


#endif

