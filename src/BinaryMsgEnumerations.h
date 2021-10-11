// BinaryMsgEnumerations.h

#ifndef _BinaryMsgEnumerations_h
#define _BinaryMsgEnumerations_h

#include "BinaryMsgDefines.h"

// Corresponds to data types on App
enum MsgDataType {
	typeUndefined = 0,
	typeBool = 1,
	typeInt8 = 2,
	typeUInt8 = 3,
	typeInt16 = 4,
	typeUInt16 = 5,
	typeInt32 = 6,
	typeUInt32 = 7,
	typeFloat32 = 8,
	//typeString = 9,
	typeInvalid = 9
};


#endif

