#include <fi3d/server/MessageEncoder.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/server/Server.h>
#include <fi3d/server/message_keys/MessageKeys.h>
#include <fi3d/server/message_keys/EResponseStatus.h>

using namespace fi3d;

MessageEncoder::MessageEncoder(const EMessage& messageEncoderType)
	: QObject(Q_NULLPTR),
	mMessageEncoderType(messageEncoderType)
{
	/// Listen to the type of request based on the message type.
	if (messageEncoderType == EMessage::APPLICATION) {
		QObject::connect(
			Server::getInstance(), &Server::changedApplicationRequest,
			this, &MessageEncoder::parseRequest);
	} else if (messageEncoderType == EMessage::MODULE) {
		QObject::connect(
			Server::getInstance(), &Server::changedModuleRequest,
			this, &MessageEncoder::parseRequest);
	} else if (messageEncoderType == EMessage::DATA) {
		QObject::connect(
			Server::getInstance(), &Server::changedDataRequest,
			this, &MessageEncoder::parseRequest);
	}

	/// Listen for new connections.
	QObject::connect(
		Server::getInstance(), &Server::changedClientIdentified,
		this, &MessageEncoder::onConnectedClient);

	/// Listen for lost connections.
	QObject::connect(
		Server::getInstance(), &Server::changedClientDisconnected,
		this, &MessageEncoder::onDisconnectedClient);
}

MessageEncoder::~MessageEncoder() {}

void MessageEncoder::onConnectedClient(const QString& clientID) {}

void MessageEncoder::onDisconnectedClient(const QString& clientID) {}

void MessageEncoder::sendMessage(MessagePtr message, const QString& clientID) {
	qDebug() << "Enter";
	Server::sendMessage(message, clientID);
	qDebug() << "Exit";
}

void MessageEncoder::sendMessage(const QJsonObject& message, const QString& clientID) {
	qDebug() << "Enter";
	Server::sendMessage(message, clientID);
	qDebug() << "Exit";
}

void MessageEncoder::sendSelectMessage(MessagePtr message, const QVector<QString>& clientIDs) {
	qDebug() << "Enter";
	Server::sendSelectMessage(message, clientIDs);
	qDebug() << "Exit";
}

void MessageEncoder::sendSelectMessage(const QJsonObject& message, const QVector<QString>& clientIDs) {
	qDebug() << "Enter";
	Server::sendSelectMessage(message, clientIDs);
	qDebug() << "Exit";
}

void MessageEncoder::sendGlobalMessage(MessagePtr message) {
	qDebug() << "Enter";
	Server::sendGlobalMessage(message);
	qDebug() << "Exit";
}

void MessageEncoder::sendGlobalMessage(const QJsonObject& message) {
	qDebug() << "Enter";
	Server::sendGlobalMessage(message);
	qDebug() << "Exit";
}

void MessageEncoder::sendErrorMessage(const QString& errorMessage, const QString& clientID) {
	qDebug() << "Enter";
	QJsonObject message;
	message.insert(RESPONSE_STATUS, EResponseStatus::ERROR_RESPONSE);
	message.insert(MESSAGE, errorMessage);
	Server::sendMessage(message, clientID);
	qDebug() << "Exit";
}

void MessageEncoder::sendSelectErrorMessage(const QString& errorMessage, const QVector<QString>& clientIDs) {
	qDebug() << "Enter";
	QJsonObject message;
	message.insert(RESPONSE_STATUS, EResponseStatus::ERROR_RESPONSE);
	message.insert(MESSAGE, errorMessage);
	Server::sendSelectMessage(message, clientIDs);
	qDebug() << "Exit";
}

void MessageEncoder::sendGlobalErrorMessage(const QString& errorMessage) {
	qDebug() << "Enter";
	QJsonObject message;
	message.insert(RESPONSE_STATUS, EResponseStatus::ERROR_RESPONSE);
	message.insert(MESSAGE, errorMessage);
	Server::sendGlobalMessage(message);
	qDebug() << "Exit";
}