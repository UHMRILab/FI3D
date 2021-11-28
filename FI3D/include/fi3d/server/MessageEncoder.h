#pragma once
/*!
* @author	VelazcoJD
* @file		MessageEncoder.h
* @class	fi3d::MessageEncoder
* @brief	Abstract class that defines structure of a message encoder.
*
* Derived classes must implement what to do w/ parsed request.
*/

#include <fi3d/server/message_keys/EMessage.h>
#include <fi3d/server/network/Message.h>

#include <QSharedPointer>
#include <QJsonObject>

namespace fi3d {
class MessageEncoder : public QObject {

	Q_OBJECT

protected:
	/// @brief The type of message encoder, identified by EMessage.
	EMessage mMessageEncoderType;

protected:
	/// @brief Constructor, can only be inherited.
	MessageEncoder(const EMessage& messageEncoderType);

public:
	/// @brief Destructor.
	~MessageEncoder();

public slots:
	/*! 
	 * @brief Prases and executes the request. 
	 *
	 * Derived classes must implement this to execute their own logic. 
	 */
	virtual void parseRequest(const QJsonObject& request, const QString& clientID, const QString& message) = 0;

	/*! 
	 * @brief Called when a new client is connected. 
	 *
	 * Does nothing and can be overriden to apply other logic.
	 */
	virtual void onConnectedClient(const QString& clientID);

	/*!
	 * @brief Called when a client is disconnected. 
	 *
	 * Does nothing and can be overriden to apply other logic.
	 */
	virtual void onDisconnectedClient(const QString& clientID);

protected:
	/// @brief Sends a message to the selected client. 
	virtual void sendMessage(MessagePtr message, const QString& clientID);

	/// @brief Sends a message (info only) to the selected client. 
	virtual void sendMessage(const QJsonObject& message, const QString& clientID);

	/// @brief Sends a message to a select list of clients.
	virtual void sendSelectMessage(MessagePtr message, const QVector<QString>& clientIDs);

	/// @brief Sends a message (info only) to a select list of clients.
	virtual void sendSelectMessage(const QJsonObject& message, const QVector<QString>& clientIDs);

	/// @brief Sends a message to all authenticated clients.
	virtual void sendGlobalMessage(MessagePtr message);

	/// @brief Sends a message (info only) to all authenticated clients.
	virtual void sendGlobalMessage(const QJsonObject& message);

	/// @brief Sends and error message to the selected client. 
	virtual void sendErrorMessage(const QString& errorResponse, const QString& clientID);

	/// @brief Sends an error message to a select list of clients.
	virtual void sendSelectErrorMessage(const QString& errorResponse, const QVector<QString>& clientIDs);

	/// @brief Sends an error message to all authenticated clients.
	virtual void sendGlobalErrorMessage(const QString& errorResponse); 
};

/// @brief Alias for a smart pointer of this class.
using MessageEncoderPtr = QSharedPointer<MessageEncoder>;

}