#pragma once
/*!
* @author	VelazcoJD
* @file		Message.h
* @class	fi3d::Message
* @brief	Contains information about a message (Request/Response).
* 
* Each Message has two pieces: info and payload. The info is encoded in JSON 
* whereas the payload itself is a simple byte array. The payload serves so that
* any piece of information can be transmitted quickly. What the payload 
* represents and how it is structured can be included as part of the info.
* Some messages have no payload and are constituted by simply the info. 
* To determite whether a message has a payload, simply use the hasPayload 
* function. 
* 
* The info and payload are never null. At minimum, they are  empty objects.
* That is, the info is a new QJsonObject and payload is a new QByteArray. To 
* determine if the data in the Message is good, check if it's valid with the
* isMessageValid function.
* 
* To read on the format of the message, see the FI3DMessageProtocol document.
*/

#include <QJsonObject>
#include <QSharedPointer>
#include <QByteArray>

namespace fi3d {
class Message {
private:
	/// @brief The info part of the message encoded in JSON.
	QSharedPointer<QJsonObject> mInfo;

	/// @brief The payload as a byte array.
	QSharedPointer<QByteArray> mPayload;

	/// @brief Used to tell whether this object contains an actual message, or
	/// it's an empty constructed object.
	bool mIsMessageEmpty;

	/// @brief Whether the message contains a payload, defaults to false.
	bool mHasPayload;

public:
	/// @brief Constructs a Message with a payload.
	Message(QSharedPointer<QJsonObject> info, QSharedPointer<QByteArray> payload);

	/// @brief Constructs a Message with no payload.
	Message(QSharedPointer<QJsonObject> info);

	/// @brief Constructs an empty Message.
	Message();

	/// @brief Destructor.
	~Message();

	/// @brief Sets the info and the payload of the Message.
	void setInfoAndPayload(QSharedPointer<QJsonObject> info, QSharedPointer<QByteArray> payload);
	
	/// @brief Sets the info part and keeps the current payload. If there was
	/// no payload, this function does the same as setInfo.
	void setInfoAndKeepPayload(QSharedPointer<QJsonObject> info);

	/// @brief Sets the payload part and keeps the current info. If the info
	/// part was not previously set, the message remains invalid.
	void setPayloadAndKeepInfo(QSharedPointer<QByteArray> payload);

	/// @brief Sets the info of the Message and assigns no payload.
	void setInfo(QSharedPointer<QJsonObject> info);

	/*
	 * @brief Invalidates the Message (as if it was empty constructed).
	 * 
	 * Useful for when the payload has changed and the info does not reflect
	 * that.
	 */
	void invalidate();

	/// @brief Whether the Message has valid info & payload pair.
	bool isMessageValid();

	/// @brief Whether the Message has a payload or not.
	bool hasPayload();

	/// @brief Gets the info.
	QSharedPointer<QJsonObject> getInfo();

	/// @brief Gets the payload.
	QSharedPointer<QByteArray> getPayload();
};

/// @brief Alias for a smart pointer of this class.
using MessagePtr = QSharedPointer<Message>;

}