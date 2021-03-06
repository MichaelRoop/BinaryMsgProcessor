/*
 Name:		BinaryMsgProcessor.h
 Created:	10/11/2021 10:47:42 AM
 Author:	Michael
 Editor:	http://www.visualmicro.com
*/

#ifndef _BinaryMsgProcessor_h
#define _BinaryMsgProcessor_h

#include "BinaryMsgDefines.h"
#include "BinaryMsgEnumerations.h"

#define MAX_IN_ID_REG 25


#ifdef BINARY_MSG_DEBUG

// Error types
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

// Error message object
struct ErrMsg {
public:
	uint8_t SOH = 0;
	uint8_t STX = 0;
	uint16_t Size = 0;
	MsgDataType DataType = typeUndefined;
	byte PayloadSize = 0;
	uint16_t RequiredSize = 0;
	uint8_t Id = 0;
	MsgError Error = err_NoErr;
	void Init();
	ErrMsg();
};
#endif // BINARY_MSG_DEBUG


class BinaryMsgProcessor {
	// Typedefs for execute methods
	typedef void (*msgFuncPtrBool)(uint8_t id, bool value);
	typedef void (*msgFuncPtrInt8)(uint8_t id, int8_t value);
	typedef void (*msgFuncPtrInt16)(uint8_t id, int16_t value);
	typedef void (*msgFuncPtrInt32)(uint8_t id, int32_t value);
	typedef void (*msgFuncPtrUInt8)(uint8_t id, uint8_t value);
	typedef void (*msgFuncPtrUInt16)(uint8_t id, uint16_t value);
	typedef void (*msgFuncPtrUInt32)(uint8_t id, uint32_t value);
	typedef void (*msgFuncPtrFloat32)(uint8_t id, float value);
#ifdef BINARY_MSG_DEBUG
	typedef void (*errEventPtr)(ErrMsg* err);
#endif // BINARY_MSG_DEBUG

public:

	// Register in message ids and expected data type for validation
	static bool RegisterInMsgId(uint8_t id, MsgDataType dataType);

	// Validate only the header fragment on read
	static bool ValidateHeader(uint8_t* buff, uint8_t size);

	// Read the size field from the header. Call ValidateHeader first
	static uint16_t GetSizeFromHeader(uint8_t* buff);

	// Read data type field from header. Call ValidateHeader first
	static MsgDataType GetDataTypeFromHeader(uint8_t* buff);

	// Validate the entire message after it is read
	static bool ValidateMessage(uint8_t* buff, int length);

	static void RegisterInMsgHandler_Bool(msgFuncPtrBool ptr);
	static void RegisterInMsgHandler_Int8(msgFuncPtrInt8 ptr);
	static void RegisterInMsgHandler_Int16(msgFuncPtrInt16 ptr);
	static void RegisterInMsgHandler_Int32(msgFuncPtrInt32 ptr);
	static void RegisterInMsgHandler_UInt8(msgFuncPtrUInt8 ptr);
	static void RegisterInMsgHandler_UInt16(msgFuncPtrUInt16 ptr);
	static void RegisterInMsgHandler_UInt32(msgFuncPtrUInt32 ptr);
	static void RegisterInMsgHandler_Float32(msgFuncPtrFloat32 ptr);
#ifdef BINARY_MSG_DEBUG
	static void RegisterErrHandler(errEventPtr ptr);
#endif // BINARY_MSG_DEBUG

private:
	static msgFuncPtrBool ptrBool;
	static msgFuncPtrInt8 ptrInt8;
	static msgFuncPtrInt16 ptrInt16;
	static msgFuncPtrInt32 ptrInt32;
	static msgFuncPtrUInt8 ptrUInt8;
	static msgFuncPtrUInt16 ptrUInt16;
	static msgFuncPtrUInt32 ptrUInt32;
	static msgFuncPtrFloat32 ptrFloat32;
#ifdef BINARY_MSG_DEBUG
	static errEventPtr errCallback;
#endif // BINARY_MSG_DEBUG

	// Each entry has id,data type. Used for validation
	static uint8_t inMsgIds[MAX_IN_ID_REG][2];
	static uint8_t currentIdListNextPos;
#ifdef BINARY_MSG_DEBUG
	static ErrMsg errMsg;
#endif // BINARY_MSG_DEBUG

	BinaryMsgProcessor();
	static uint8_t GetIdFromHeader(uint8_t* buff);
	static byte GetPayloadSize(MsgDataType dt);
#ifdef BINARY_MSG_DEBUG
	static bool RaiseError(ErrMsg* msg);
#endif // BINARY_MSG_DEBUG
	static bool RaiseRegisteredEvents(uint8_t* buff);
	static bool RaiseBool(uint8_t id, uint8_t* buff, uint8_t offset);
	static bool RaiseInt8(uint8_t id, uint8_t* buff, uint8_t offset);
	static bool RaiseInt16(uint8_t id, uint8_t* buff, uint8_t offset);
	static bool RaiseInt32(uint8_t id, uint8_t* buff, uint8_t offset);
	static bool RaiseUInt8(uint8_t id, uint8_t* buff, uint8_t offset);
	static bool RaiseUInt16(uint8_t id, uint8_t* buff, uint8_t offset);
	static bool RaiseUInt32(uint8_t id, uint8_t* buff, uint8_t offset);
	static bool RaiseFloat32(uint8_t id, uint8_t* buff, uint8_t offset);

};


#endif

