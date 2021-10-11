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

#ifdef BINARY_MSG_DEBUG
enum MsgError {
	err_NoErr = 0,
	err_InvalidType = 1,
	err_InvalidHeaderSize = 2,
	err_StartDelimiters = 3,
	err_InvalidSizeField = 4,
	err_InvalidPayloadSizeField = 5,
	err_InvalidDataTypeForRegisteredId = 6,
	err_CallbackNotRegisteredForId = 7
};
#endif // BINARY_MSG_DEBUG




#endif

