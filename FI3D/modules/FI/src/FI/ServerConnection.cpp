#include <FI/ServerConnection.h>

#include <fi3d/modules/ModuleMessageEncoder.h>

#include <fi3d/server/message_keys/MessageKeys.h>

#include <QJsonParseError>
#include <QJsonObject>

using namespace fi3d;
using namespace fi;

inline void insertRequestHeader(QSharedPointer<QJsonObject> request, 
	const EMessage& messageType, const QString& clientID,
	const QString& message = "") 
{
	request->insert(CLIENT_ID, clientID);
	request->insert(MESSAGE_TYPE, messageType.toInt());
	request->insert(MESSAGE, message);
}

ServerConnection::ServerConnection() 
	: ClientFI3D(),
	mFI3DPassword("admin"),
	mIsAuthenticated(false)
{
	QObject::connect(
		this, &QTcpSocket::disconnected,
		this, &ServerConnection::onDisconnected);
	QObject::connect(
		this, &ClientFI3D::messageReceived,
		this, &ServerConnection::onMessage);
}

ServerConnection::~ServerConnection() {
	// Disconnect the signal because it calls this function after the object
	// has been deleted (causing an excepting).
	QObject::disconnect(
		this, &QTcpSocket::disconnected,
		this, &ServerConnection::onDisconnected);
}

void ServerConnection::onDisconnected() {
	qDebug() << "Enter";
	if (!mClientID.isEmpty()) {
		mClientID = "";
		emit changedClientID(mClientID);
	}

	if (mIsAuthenticated) {
		mIsAuthenticated = false;
		emit changedAuthenticated(mIsAuthenticated);
	}

	emit changedConnectionFeedback("Disconnected");
	qDebug() << "Exit";
}

void ServerConnection::onMessage(MessagePtr message) {
	qDebug() << "Enter";
	
	QSharedPointer<QJsonObject> response = message->getInfo();

	int messageType = response->value(MESSAGE_TYPE).toInt();
	
	qDebug() << "Received message of type:" << EMessage(messageType).getName();
	switch (messageType) {
		case EMessage::AUTHENTICATION:
			this->onServerMessage(message);
			break;
		case EMessage::APPLICATION:
			emit newApplicationResponse(message);
			break;
		case EMessage::DATA:
			emit newDataResponse(message);
			break;
		case EMessage::MODULE:
			emit newModuleResponse(message);
			break;
		default:
			break;
	}
	qDebug() << "Exit";
}

void ServerConnection::onServerMessage(MessagePtr message) {
	qDebug() << "Enter";
	QSharedPointer<QJsonObject> info = message->getInfo();

	int responseStatus = info->value(RESPONSE_STATUS).toInt();
	switch (responseStatus) {
		case EResponseStatus::INFO_REQUIRED:
			mClientID = info->value(CLIENT_ID).toString();
			{
				QSharedPointer<QJsonObject> request(new QJsonObject());
				insertRequestHeader(request, EMessage::AUTHENTICATION, mClientID);
				request->insert(PASSWORD, mFI3DPassword);
				this->sendRequest(request);
			}
			emit changedClientID(mClientID);
			emit changedConnectionFeedback("Connection Established. Authenticating...");
			break;
		case EResponseStatus::ERROR_RESPONSE:
			// TODO: Provide error feedback.
			break;
		case EResponseStatus::SUCCESS:
			mIsAuthenticated = true;
			qInfo() << "FI3D connection authenticated";
			emit changedAuthenticated(mIsAuthenticated);
			emit changedConnectionFeedback("Connected and Authenticated");
			break;
		default:
			break;
	}
	qDebug() << "Exit";
}

void ServerConnection::sendApplicationRequest(QJsonObject& appParams, const QString& message) {
	QSharedPointer<QJsonObject> request(new QJsonObject());
	request->insert(APPLICATION_PARAMS, appParams);
	insertRequestHeader(request, EMessage::APPLICATION, mClientID, message);
	this->sendRequest(request);
}

void ServerConnection::sendModuleRequest(QJsonObject& moduParams, const QString& message) {
	QSharedPointer<QJsonObject> request(new QJsonObject());
	request->insert(MODULE_PARAMS, moduParams);
	insertRequestHeader(request, EMessage::MODULE, mClientID, message);
	this->sendRequest(request);
}

void ServerConnection::sendDataRequest(QJsonObject& dataParams, const QString& message) {
	QSharedPointer<QJsonObject> request(new QJsonObject());
	request->insert(DATA_PARAMS, dataParams);
	insertRequestHeader(request, EMessage::DATA, mClientID, message);
	this->sendRequest(request);
}

void ServerConnection::setConnectionPassword(const QString& password) {
	this->close();
	mFI3DPassword = password;
	emit changedPassword(password);
}

void ServerConnection::sendRequest(QSharedPointer<QJsonObject> request) {
	MessagePtr message(new Message(request));
	this->sendMessage(message);
}

QString ServerConnection::getConnectionPassword() const {
	return mFI3DPassword;
}

QString ServerConnection::getClientID() const {
	return mClientID;
}

bool ServerConnection::isAuthenticated() const {
	return mIsAuthenticated;
}

