/*
 Name:		BinaryMsgProcessor.cpp
 Created:	10/11/2021 10:47:42 AM
 Author:	Michael
 Editor:	http://www.visualmicro.com
*/

#include "BinaryMsgProcessor.h"

#ifdef BINARY_MSG_DEBUG
struct MsgHeader {
public:
	uint8_t SOH = _SOH;
	uint8_t STX = _STX;
	uint16_t Size = 0;
	uint8_t DataType = typeUndefined;
	uint8_t Id = 0;
};

// Err message

ErrMsg::ErrMsg() {
	this->Init();
}

void ErrMsg::Init() {
	this->SOH = 0;
	this->STX = 0;
	this->Size = 0;
	this->DataType = typeUndefined;
	this->PayloadSize = 0;
	this->RequiredSize = 0;
	this->Id = 0;
	this->Error = err_NoErr;
}
#endif // BINARY_MSG_DEBUG



uint8_t BinaryMsgProcessor::inMsgIds[MAX_IN_ID_REG][2];
uint8_t BinaryMsgProcessor::currentIdListNextPos;

// TODO - remove - record of how to use when not typedef
//void (*BinaryMsgProcessor::MyBoolFunc) (bool) = NULL;
BinaryMsgProcessor::msgFuncPtrBool BinaryMsgProcessor::ptrBool = NULL;
BinaryMsgProcessor::msgFuncPtrInt8 BinaryMsgProcessor::ptrInt8 = NULL;
BinaryMsgProcessor::msgFuncPtrInt16 BinaryMsgProcessor::ptrInt16 = NULL;
BinaryMsgProcessor::msgFuncPtrInt32 BinaryMsgProcessor::ptrInt32 = NULL;
BinaryMsgProcessor::msgFuncPtrUInt8 BinaryMsgProcessor::ptrUInt8 = NULL;
BinaryMsgProcessor::msgFuncPtrUInt16 BinaryMsgProcessor::ptrUInt16 = NULL;
BinaryMsgProcessor::msgFuncPtrUInt32 BinaryMsgProcessor::ptrUInt32 = NULL;
BinaryMsgProcessor::msgFuncPtrFloat32 BinaryMsgProcessor::ptrFloat32 = NULL;
#ifdef BINARY_MSG_DEBUG
BinaryMsgProcessor::errEventPtr BinaryMsgProcessor::errCallback = NULL;
ErrMsg BinaryMsgProcessor::errMsg;
#endif // BINARY_MSG_DEBUG



BinaryMsgProcessor::BinaryMsgProcessor() {
	BinaryMsgProcessor::currentIdListNextPos = 0;
}

#ifndef SECTION_PUBLIC_METHODS

bool BinaryMsgProcessor::RegisterInMsgId(uint8_t id, MsgDataType dataType) {
	if (BinaryMsgProcessor::currentIdListNextPos < MAX_IN_ID_REG) {
		if (dataType > typeUndefined && dataType < typeInvalid) {
			inMsgIds[currentIdListNextPos][0] = id;
			inMsgIds[currentIdListNextPos][1] = dataType;
			BinaryMsgProcessor::currentIdListNextPos++;
		}
	}
	return false;
}

#ifdef BINARY_MSG_DEBUG
void BinaryMsgProcessor::RegisterErrHandler(errEventPtr ptr) {
	BinaryMsgProcessor::errCallback = ptr;
}
#endif // BINARY_MSG_DEBUG


void BinaryMsgProcessor::RegisterInMsgHandler_Bool(msgFuncPtrBool ptr) {
	BinaryMsgProcessor::ptrBool = ptr;
}


void BinaryMsgProcessor::RegisterInMsgHandler_Int8(msgFuncPtrInt8 ptr) {
	BinaryMsgProcessor::ptrInt8 = ptr;
}


void BinaryMsgProcessor::RegisterInMsgHandler_Int16(msgFuncPtrInt16 ptr) {
	BinaryMsgProcessor::ptrInt16 = ptr;
}


void BinaryMsgProcessor::RegisterInMsgHandler_Int32(msgFuncPtrInt32 ptr) {
	BinaryMsgProcessor::ptrInt32 = ptr;
}


void BinaryMsgProcessor::RegisterInMsgHandler_UInt8(msgFuncPtrUInt8 ptr) {
	BinaryMsgProcessor::ptrUInt8 = ptr;
}


void BinaryMsgProcessor::RegisterInMsgHandler_UInt16(msgFuncPtrUInt16 ptr) {
	BinaryMsgProcessor::ptrUInt16 = ptr;
}


void BinaryMsgProcessor::RegisterInMsgHandler_UInt32(msgFuncPtrUInt32 ptr) {
	BinaryMsgProcessor::ptrUInt32 = ptr;
}


void BinaryMsgProcessor::RegisterInMsgHandler_Float32(msgFuncPtrFloat32 ptr) {
	BinaryMsgProcessor::ptrFloat32 = ptr;
}

#endif // !SECTION_PUBLIC_METHODS


bool BinaryMsgProcessor::ValidateHeader(uint8_t* buff, uint8_t dataSize) {

#ifdef BINARY_MSG_DEBUG
	BinaryMsgProcessor::errMsg.Init();

	if (dataSize < MSG_HEADER_SIZE) {
		BinaryMsgProcessor::errMsg.Size = 0;
		BinaryMsgProcessor::errMsg.Error = err_InvalidHeaderSize;
		return BinaryMsgProcessor::RaiseError(&errMsg);
	}

	MsgHeader h;
	memcpy(&h, buff, sizeof(MsgHeader));

	errMsg.SOH = *(buff + SOH_POS);
	errMsg.STX = *(buff + STX_POS);
	if (errMsg.SOH != _SOH || errMsg.STX != _STX) {
		errMsg.Error = err_StartDelimiters;
		return BinaryMsgProcessor::RaiseError(&errMsg);
	}

	errMsg.DataType = (MsgDataType)(*(buff + TYPE_POS));
	// Get size and validate the number against the data type
	if (!(errMsg.DataType > typeUndefined) && (errMsg.DataType < typeInvalid)) {
		errMsg.Error = err_InvalidType;
		return BinaryMsgProcessor::RaiseError(&errMsg);
	}

	errMsg.Size = BinaryMsgProcessor::GetSizeFromHeader(buff);
	if (errMsg.Size == 0) {
		errMsg.Error = err_InvalidSizeField;
		return BinaryMsgProcessor::RaiseError(&errMsg);
	}

	errMsg.PayloadSize = GetPayloadSize(errMsg.DataType);
	errMsg.RequiredSize = MSG_HEADER_SIZE + MSG_FOOTER_SIZE + errMsg.PayloadSize;
	if (errMsg.Size != errMsg.RequiredSize) {
		errMsg.Error = err_InvalidPayloadSizeField;
		return BinaryMsgProcessor::RaiseError(&errMsg);
	}

	errMsg.Id = BinaryMsgProcessor::GetIdFromHeader(buff);
	// validate id and expected data type
	for (int i = 0; i < BinaryMsgProcessor::currentIdListNextPos; i++) {
		// Found registered ID
		if (BinaryMsgProcessor::inMsgIds[i][0] == errMsg.Id) {
			if (BinaryMsgProcessor::inMsgIds[i][1] == errMsg.DataType) {
				return true;
			}
			// Msg data type not same as registered for ID
			errMsg.Error = err_InvalidDataTypeForRegisteredId;
			return BinaryMsgProcessor::RaiseError(&errMsg);
		}
	}

	errMsg.Error = err_CallbackNotRegisteredForId;
	return BinaryMsgProcessor::RaiseError(&errMsg);
#else
	// TODO - look at registering a simple error return with err type?
	if (dataSize < MSG_HEADER_SIZE) {
		return false;
	}

	if (*(buff + SOH_POS) != _SOH || *(buff + STX_POS) != _STX) {
		return false;
	}

	uint8_t dataType = (MsgDataType)(*(buff + TYPE_POS));
	// Get size and validate the number against the data type
	if (!(dataType > typeUndefined) && (dataType < typeInvalid)) {
		return false;
	}

	uint16_t size = BinaryMsgProcessor::GetSizeFromHeader(buff);
	if (size == 0) {
		return false;
	}

	if (size != MSG_HEADER_SIZE + MSG_FOOTER_SIZE + GetPayloadSize((MsgDataType)dataType)) {
		return false;
	}

	uint8_t id = BinaryMsgProcessor::GetIdFromHeader(buff);
	// validate id and expected data type
	for (int i = 0; i < BinaryMsgProcessor::currentIdListNextPos; i++) {
		// Found registered ID
		if (BinaryMsgProcessor::inMsgIds[i][0] == id) {
			if (BinaryMsgProcessor::inMsgIds[i][1] == dataType) {
				return true;
			}
			// Msg data type not same as registered for ID
			return false;
		}
	}

	return false;
#endif // BINARY_MSG_DEBUG
}


uint16_t BinaryMsgProcessor::GetSizeFromHeader(uint8_t* buff) {
	uint16_t size = 0;
	memcpy(&size, (buff + SIZE_POS), sizeof(uint16_t));
	// Message size must be at least big enough for header, footer and minimum 1 byte payload
	if (size < (MSG_HEADER_SIZE + MSG_FOOTER_SIZE + 1)) {
		size = 0;
	}
	return size;
}

MsgDataType BinaryMsgProcessor::GetDataTypeFromHeader(uint8_t* buff) {
	return (MsgDataType)(*(buff + TYPE_POS));
}


uint8_t BinaryMsgProcessor::GetIdFromHeader(uint8_t* buff) {
	return (MsgDataType)(*(buff + ID_POS));
}


bool BinaryMsgProcessor::ValidateMessage(uint8_t* buff, int length) {
	if (BinaryMsgProcessor::ValidateHeader(buff, length)) {
		if (BinaryMsgProcessor::GetSizeFromHeader(buff) == length) {
			return RaiseRegisteredEvents(buff);
		}
	}
	return false;
}

#ifdef BINARY_MSG_DEBUG
bool BinaryMsgProcessor::RaiseError(ErrMsg* msg) {
	if (BinaryMsgProcessor::errCallback != NULL) {
		BinaryMsgProcessor::errCallback(msg);
	}
	// Return false to optimize use to exit bool methods
	return false;
}
#endif // BINARY_MSG_DEBUG


bool BinaryMsgProcessor::RaiseBool(uint8_t id, uint8_t* buff, uint8_t offset) {
	if (ptrBool != NULL) {
		bool bVal;
		memcpy(&bVal, (buff + offset), sizeof(bool));
		ptrBool(id, bVal);
		return true;
	}
	return false;
}


bool BinaryMsgProcessor::RaiseInt8(uint8_t id, uint8_t* buff, uint8_t offset) {
	if (ptrInt8 != NULL) {
		int8_t val;
		memcpy(&val, (buff + offset), sizeof(int8_t));
		ptrInt8(id, val);
		return true;
	}
	return false;
}


bool BinaryMsgProcessor::RaiseInt16(uint8_t id, uint8_t* buff, uint8_t offset) {
	if (ptrInt16 != NULL) {
		int16_t val;
		memcpy(&val, (buff + offset), sizeof(int16_t));
		ptrInt16(id, val);
		return true;
	}
	return false;
}


bool BinaryMsgProcessor::RaiseInt32(uint8_t id, uint8_t* buff, uint8_t offset) {
	if (ptrInt32 != NULL) {
		int32_t val;
		memcpy(&val, (buff + offset), sizeof(int32_t));
		ptrInt32(id, val);
		return true;
	}
	return false;
}


bool BinaryMsgProcessor::RaiseUInt8(uint8_t id, uint8_t* buff, uint8_t offset) {
	if (ptrUInt8 != NULL) {
		uint8_t val;
		memcpy(&val, (buff + offset), sizeof(uint8_t));
		ptrUInt8(id, val);
		return true;
	}
	return false;
}


bool BinaryMsgProcessor::RaiseUInt16(uint8_t id, uint8_t* buff, uint8_t offset) {
	if (ptrUInt16 != NULL) {
		uint16_t val;
		memcpy(&val, (buff + offset), sizeof(uint16_t));
		ptrUInt16(id, val);
		return true;
	}
	return false;
}


bool BinaryMsgProcessor::RaiseUInt32(uint8_t id, uint8_t* buff, uint8_t offset) {
	if (ptrUInt32 != NULL) {
		uint32_t val;
		memcpy(&val, (buff + offset), sizeof(uint32_t));
		ptrUInt32(id, val);
		return true;
	}
	return false;
}


bool BinaryMsgProcessor::RaiseFloat32(uint8_t id, uint8_t* buff, uint8_t offset) {
	if (ptrFloat32 != NULL) {
		float val;
		memcpy(&val, (buff + offset), sizeof(float));
		ptrFloat32(id, val);
		return true;
	}
	return false;
}



bool BinaryMsgProcessor::RaiseRegisteredEvents(uint8_t* buff) {
	uint8_t offset = VALUE_POS;
	uint8_t id = BinaryMsgProcessor::GetIdFromHeader(buff);
	switch (BinaryMsgProcessor::GetDataTypeFromHeader(buff)) {
	case typeBool:
		return RaiseBool(id, buff, offset);
	case typeInt8:
		return RaiseInt8(id, buff, offset);
	case typeUInt8:
		return RaiseUInt8(id, buff, offset);
	case typeInt16:
		return RaiseInt16(id, buff, offset);
	case typeUInt16:
		return RaiseUInt16(id, buff, offset);
	case typeInt32:
		return RaiseInt32(id, buff, offset);
	case typeUInt32:
		return RaiseUInt32(id, buff, offset);
	case typeFloat32:
		return RaiseFloat32(id, buff, offset);
	case typeUndefined:
	case typeInvalid:
	default:
		// TODO - show error
		// Should not happen raise err and return true to avoid unregietered error
		//RaiseError(err_InvalidType, (uint16_t)BinaryMsgProcessor::GetDataTypeFromHeader(buff));
		return true;
	}
}


byte BinaryMsgProcessor::GetPayloadSize(MsgDataType dt) {
	switch (dt) {
	case typeBool:
	case typeInt8:
	case typeUInt8:
		// Same size for above
		return sizeof(uint8_t);
	case typeInt16:
	case typeUInt16:
		return sizeof(uint16_t);
	case typeInt32:
	case typeUInt32:
	case typeFloat32:
		return sizeof(uint32_t);
	case typeUndefined:
	case typeInvalid:
	default:
		// Should not happen. Return big size to cause failure
		return 1000;
	}
}


