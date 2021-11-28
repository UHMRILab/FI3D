#include <fi3d/server/Server.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/FI3D/FI3DController.h>

#include <fi3d/server/ServerDialog.h>
#include <fi3d/server/message_keys/MessageKeys.h>
#include <fi3d/server/message_keys/EResponseStatus.h>
#include <fi3d/server/message_keys/EMessage.h>

#include <QAbstractSocket>
#include <QByteArray>
#include <QJsonDocument>
#include <QList>
#include <QNetworkInterface>

using namespace fi3d;

/************************ FI3D Componenet Members ************************/
REGISTER_FI3D_COMPONENT_WITH_GUI(Server, "Server", "Server")

void Server::showDialog() {
	if (!INSTANCE->mDialog->isVisible()) {
		INSTANCE->mDialog->show();
		INSTANCE->mDialog->updateServerIP(Server::GetIPAddress());
		INSTANCE->mDialog->updateServerPort(Server::GetPortNumber());
		INSTANCE->mDialog->updatePassword(Server::getPassword());
		INSTANCE->mDialog->updateConnectedDevices(Server::GetHMDCount());
		INSTANCE->mDialog->updateUnidentifiedConnections(Server::getUnidentifiedCount());
	}
}

/************************ Static Members ************************/
void Server::sendMessage(MessagePtr message, const QString& clientID) {
	qDebug() << "Enter";
	if (message.isNull()) {
		qWarning() << "Failed to send message to" << clientID << "because the given message is null.";
		return;
	}
	
	FrameworkInterface* client = INSTANCE->mAuthenticatedClients.value(clientID, Q_NULLPTR);
	if (client == Q_NULLPTR) {
		qWarning() << "Failed to deliver response. Client not found.";
		return;
	}

	QJsonDocument doc(*message->getInfo().data());
	QByteArray info = doc.toJson(QJsonDocument::JsonFormat::Compact);
	QByteArray header = Server::createHeader(info.count(), message->hasPayload(), message->getPayload()->count());

	qDebug() << "Sending Message to" << clientID << "with Info=\n" << info;

	client->write(header);
	client->write(info);
	if (message->hasPayload()) {
		client->write(*message->getPayload().data());
	}
	qDebug() << "Exit";
}

void Server::sendMessage(const QJsonObject& message, const QString& clientID) {
	qDebug() << "Enter";
	FrameworkInterface* client = INSTANCE->mAuthenticatedClients.value(clientID, Q_NULLPTR);
	if (client == Q_NULLPTR) {
		qWarning() << "Failed to deliver response. Client not found.";
		return;
	}

	QJsonDocument doc(message);
	QByteArray info = doc.toJson(QJsonDocument::JsonFormat::Compact);
	QByteArray header = Server::createHeader(info.count());

	qDebug() << "Sending Message to" << clientID << "with Info=\n" << info;

	client->write(header);
	client->write(info);
	qDebug() << "Exit";
}

void Server::sendSelectMessage(MessagePtr message, const QVector<QString>& clientIDs) {
	qDebug() << "Enter";
	if (clientIDs.size() == 0) {
		return;
	}

	if (message.isNull()) {
		qWarning() << "Failed to send message to" << clientIDs.count() << "clients because the given message is null.";
		return;
	}

	QJsonDocument doc(*message->getInfo().data());
	QByteArray info = doc.toJson(QJsonDocument::JsonFormat::Compact);
	QByteArray header = Server::createHeader(info.count(), message->hasPayload(), message->getPayload()->count());

	qDebug() << "Sending Message to " << clientIDs.count() << " clients with Info=\n" << info;

	for (int i = 0; i < clientIDs.size(); i++) {
		FrameworkInterface* client = INSTANCE->mAuthenticatedClients.value(clientIDs.at(i), Q_NULLPTR);
		if (client != Q_NULLPTR) {
			client->write(header);
			client->write(info);
			if (message->hasPayload()) {
				client->write(*message->getPayload().data());
			}
		} else {
			qWarning() << "Failed to send message to" << clientIDs.at(i) << "because the client was not found.";
		}
	}
	qDebug() << "Exit";
}

void Server::sendSelectMessage(const QJsonObject& message, const QVector<QString>& clientIDs) {
	qDebug() << "Enter";
	if (clientIDs.size() == 0) {
		return;
	}

	QJsonDocument doc(message);
	QByteArray info = doc.toJson(QJsonDocument::JsonFormat::Compact);
	QByteArray header = Server::createHeader(info.count());

	qDebug() << "Sending Message to " << clientIDs.count() << " clients with Info=\n" << info;

	for (int i = 0; i < clientIDs.size(); i++) {
		FrameworkInterface* client = INSTANCE->mAuthenticatedClients.value(clientIDs.at(i), Q_NULLPTR);
		if (client != Q_NULLPTR) {
			client->write(header);
			client->write(info);
		} else {
			qWarning() << "Failed to send message to" << clientIDs.at(i) << "because the client was not found.";
		}
	}
	qDebug() << "Exit";
}

void Server::sendGlobalMessage(MessagePtr message) {
	qDebug() << "Enter";
	if (INSTANCE->mAuthenticatedClients.count() == 0) {
		return;
	}

	if (message.isNull()) {
		qWarning() << "Failed to send message global message because the given message is null.";
		return;
	}

	QJsonDocument doc(*message->getInfo().data());
	QByteArray info = doc.toJson(QJsonDocument::JsonFormat::Compact);
	QByteArray header = Server::createHeader(info.count(), message->hasPayload(), message->getPayload()->count());

	qDebug() << "Sending Message to all clients with Info=\n" << info;

	QHash<QString, FrameworkInterface*>::iterator it = INSTANCE->mAuthenticatedClients.begin();
	for (; it != INSTANCE->mAuthenticatedClients.end(); it++) {
		it.value()->write(header);
		it.value()->write(info);
		if (message->hasPayload()) {
			it.value()->write(*message->getPayload().data());
		}
	}
	qDebug() << "Exit";
}

void Server::sendGlobalMessage(const QJsonObject& message) {
	qDebug() << "Enter";
	if (INSTANCE->mAuthenticatedClients.count() == 0) {
		return;
	}

	QJsonDocument doc(message);
	QByteArray info = doc.toJson(QJsonDocument::JsonFormat::Compact);
	QByteArray header = Server::createHeader(info.count());

	qDebug() << "Sending Message to all clients with Info=\n" << info;

	QHash<QString, FrameworkInterface*>::iterator it = INSTANCE->mAuthenticatedClients.begin();
	for (; it != INSTANCE->mAuthenticatedClients.end(); it++) {
		it.value()->write(header);
		it.value()->write(info);
	}
	qDebug() << "Exit";
}

QString Server::getPassword() {
	qDebug() << "Enter";
	return INSTANCE->mPassword;
	qDebug() << "Exit";
}

void Server::setPassword(const QString& password) {
	qDebug() << "Enter - new password:" << password;
	INSTANCE->mPassword = password;
	INSTANCE->mDialog->updatePassword(INSTANCE->mPassword);
	qDebug() << "Exit";
}

QString Server::GetIPAddress() {
	return INSTANCE->mIPAddress;
}

int Server::GetPortNumber() {
	return INSTANCE->mPort;
}

int Server::GetHMDCount() {
	return INSTANCE->mAuthenticatedClients.size();
}

int Server::getUnidentifiedCount() {
	return INSTANCE->mUnauthenticatedClients.size();
}

const Server* Server::getInstance() {
	return INSTANCE.data();
}

/************************ Singleton Members ************************/
Server::Server()
	: mIPAddress("Server Down"),
	mPort(9000),
	mPassword("admin"),
	mAuthenticatedClients(),
	mUnauthenticatedClients(),
	mDeviceCount(0)
{
	mDialog.reset(new ServerDialog(mIPAddress, mPort, mPassword, 
		mAuthenticatedClients.count(), mUnauthenticatedClients.count()));
	mDialog->setModal(true);
	QObject::connect(
		mDialog.data(), &ServerDialog::changePassword, 
		this, &Server::setPassword);

	this->startServer(mPort);

	mIPAddress = this->getIPAddress();
	mDialog->updateServerIP(mIPAddress);
	mDialog->updateServerPort(mPort);
	mDialog->updatePassword(mPassword);
	mDialog->updateConnectedDevices(mAuthenticatedClients.count());
	mDialog->updateUnidentifiedConnections(mUnauthenticatedClients.count());

	QObject::connect(
		this, &QTcpServer::newConnection,
		this, &Server::onNewConnection);
}

Server::~Server() {}

void Server::stopServer() {
	qDebug() << "Enter";
	QHash<QString, FrameworkInterface*>::iterator it = mAuthenticatedClients.begin();
	for (; it != mAuthenticatedClients.end(); it++){
		it.value()->close();
	}

	it = mUnauthenticatedClients.begin();
	for (; it != mUnauthenticatedClients.end(); it++) {
		it.value()->close();
	}

	mAuthenticatedClients.clear();
	mUnauthenticatedClients.clear();
	
	this->close();

	mDialog->updateServerIP("Server Down");
	mDialog->updateServerPort(mPort);
	mDialog->updatePassword(mPassword);
	mDialog->updateConnectedDevices(mAuthenticatedClients.count());
	mDialog->updateUnidentifiedConnections(mUnauthenticatedClients.count());
	qDebug() << "Exit";
}

void Server::authenticateConnection(const QString& clientID, const QString& password) {
	qDebug() << "Enter";
	if (mAuthenticatedClients.contains(clientID)) {
		qInfo() << "Client" << clientID << "is trying to authenticate again.";
		QJsonObject authResponse{
			{RESPONSE_STATUS, EResponseStatus::SUCCESS},
			{MESSAGE_TYPE, EMessage::AUTHENTICATION},
			{MESSAGE, "Authentication Successful"}
		};
		this->sendMessage(authResponse, clientID);

		qDebug() << "Exit - Already authenticated";
		return;
	}

	FrameworkInterface* authClient = mUnauthenticatedClients.value(clientID, Q_NULLPTR);
	if (!authClient) {
		qCritical() << "FI with ID:" << clientID << "tried to authenticate, but it was not found";
		qDebug() << "Exit - FI not found.";
		return;
	}
	
	if (mPassword == password) {
		mAuthenticatedClients.insert(clientID, authClient);
		mUnauthenticatedClients.remove(clientID);
		authClient->setAuthenticated(true);

		mDialog->updateConnectedDevices(mAuthenticatedClients.count());
		mDialog->updateUnidentifiedConnections(mUnauthenticatedClients.count());

		QJsonObject authResponse{
			{RESPONSE_STATUS, EResponseStatus::SUCCESS},
			{MESSAGE_TYPE, EMessage::AUTHENTICATION},
			{MESSAGE, "Authentication Successful"}
		};
		this->sendMessage(authResponse, clientID);
		
		emit changedClientIdentified(clientID);
		qInfo() << "FI with ID:" << clientID << "authenticated successfully";
	} else {
		QJsonObject authResponse{
			{RESPONSE_STATUS, EResponseStatus::ERROR_RESPONSE},
			{MESSAGE_TYPE, EMessage::AUTHENTICATION},
			{MESSAGE, "Incorrect password"}
		};
		this->sendMessage(authResponse, clientID);
		
		qWarning() << "FI with ID" << clientID << "tried to authenticate with incorrect password";
	}
	qDebug() << "Exit";
}

void Server::onNewConnection() {
	qDebug() << "Enter";
	FrameworkInterface* fiSocket = qobject_cast<FrameworkInterface*>(this->nextPendingConnection());

	QString FIID = tr("HMD-%1").arg(mDeviceCount++);
	fiSocket->setFIID(FIID);

	QObject::connect(
		fiSocket, &FrameworkInterface::disconnected,
		this, &Server::onCloseConnection);
	QObject::connect(
		fiSocket, &FrameworkInterface::messageReceived, 
		this, &Server::onMessage);

	mUnauthenticatedClients.insert(FIID, fiSocket);
	mDialog->updateUnidentifiedConnections(mUnauthenticatedClients.count());

	QJsonObject authResponse{
		{RESPONSE_STATUS, EResponseStatus::INFO_REQUIRED},
		{MESSAGE_TYPE, EMessage::AUTHENTICATION},
		{CLIENT_ID, FIID},
		{MESSAGE, ""}
	};
	QJsonDocument doc(authResponse);
	QByteArray info = doc.toJson(QJsonDocument::JsonFormat::Compact);
	QByteArray header = Server::createHeader(info.count());
	fiSocket->write(header);
	fiSocket->write(info);
	
	//TODO: Connect to a timer that deletes this connection if they don't 
	//provide password after some time
	//TODO: For debugging purposes, every connection will be automatically 
	//authenticated.
	this->authenticateConnection(FIID, mPassword);

	emit changedClientConnected(FIID);

	qDebug() << "Exit - Connection added: " << FIID;
}

void Server::onNewConnectionError(QAbstractSocket::SocketError error) {
	// TODO: This function has not been tested.
	qDebug() << "Error opening new connection:" << error;
}

void Server::onCloseConnection() {
	qDebug() << "Enter";
	
	FrameworkInterface* closingFI = qobject_cast<FrameworkInterface*>(sender());
	if (closingFI == Q_NULLPTR) {
		qCritical() << "A close connection request was made from an unknown object.";
		return;
	}
	QString FIID = closingFI->getFIID();


	mAuthenticatedClients.remove(FIID);
	mUnauthenticatedClients.remove(FIID);
	
	mDialog->updateConnectedDevices(mAuthenticatedClients.count());
	mDialog->updateUnidentifiedConnections(mUnauthenticatedClients.count());

	emit changedClientDisconnected(FIID);
	qInfo() << "Closing Connection:" << FIID;

	qDebug() << "Exit";
}

void Server::onMessage(MessagePtr message) {
	qDebug() << "Enter";
	// TODO: All requests so far do not include a payload. This function
	// Assumes all received requests have no payload. This needs to be updated
	// so that payloads are taken into account for when they may be needed
	// in the future.

	FrameworkInterface* fi = qobject_cast<FrameworkInterface*>(sender());
	if (!fi) {
		qCritical() << "Message received from an unknown object.";
		qDebug() << "Exit - Unknown sender";
		return;
	}

	if (message.isNull()) {
		qCritical() << "Received a request from" << fi->getFIID() << "that is null.";
		qDebug() << "Exit - Received message is null";
		return;
	}

	QSharedPointer<QJsonObject> info = message->getInfo();

	qDebug() << "New request from: " << fi->getFIID() << "\n" << *info.data();

	QString clientID = info->value("ClientID").toString("");
	if (clientID != fi->getFIID()) {
		qWarning() << "Request's FI ID does not match the FI's ID";
		qDebug() << "Exit - Non-matching FI IDs";
		return;
	}

	int requestType = info->value(MESSAGE_TYPE).toInt(0);
	switch (requestType) {
		case EMessage::AUTHENTICATION: {
			if (!fi->isAuthenticated()) {
				this->authenticateConnection(clientID, info->value(PASSWORD).toString(""));
			}
			break;
		} case EMessage::APPLICATION: {
			if (fi->isAuthenticated()) {
				qDebug() << "Received a main application request";
				QJsonObject applicationParams = info->value(APPLICATION_PARAMS).toObject();
				emit changedApplicationRequest(applicationParams, clientID, info->value(MESSAGE).toString(""));
			}
			break;
		} case EMessage::MODULE: {
			if (fi->isAuthenticated()) {
				qDebug() << "Received a module request";
				QJsonObject moduleParams = info->value(MODULE_PARAMS).toObject();
				emit changedModuleRequest(moduleParams, clientID, info->value(MESSAGE).toString(""));
				break;
			}
		} case EMessage::DATA: {
			if (fi->isAuthenticated()) {
				qDebug() << "Received a data request";
				QJsonObject dataParams = info->value(DATA_PARAMS).toObject();
				emit changedDataRequest(dataParams, clientID, info->value(MESSAGE).toString(""));
				break;
			}
		} default:
			qWarning() << "Received message of unknown type FI: " << clientID;
			break;
	}
	qDebug() << "Exit";
}

ClientTCP* Server::makeClientTCPSocket() const {
	return new FrameworkInterface();
}

int Server::sHeaderWithoutPayloadLength = sizeof(quint32) + sizeof(bool);
int Server::sHeaderWithPayloadLength = 2 * sizeof(quint32) + sizeof(bool);

QByteArray Server::createHeader(MessagePtr message) {
	if (message.isNull()) {
		qWarning() << "Failed to create Message header because Message is null.";
		return QByteArray();
	}

	QJsonDocument doc(*message->getInfo().data());
	QByteArray info = doc.toJson(QJsonDocument::JsonFormat::Compact);

	qint32 infoLength = info.count();
	if (message->hasPayload()) {
		quint32 payloadLength = message->getPayload()->count();
		return Server::createHeader(infoLength, message->hasPayload(), payloadLength);
	} else {
		return Server::createHeader(infoLength);
	}
}

QByteArray Server::createHeader(const qint32& infoLenth, const bool& hasPayload, const qint32& payloadLength) {
	qDebug() << "Enter"; 
	
	QByteArray header;
	if (hasPayload) {
		header.reserve(sHeaderWithPayloadLength);
	} else {
		header.reserve(sHeaderWithoutPayloadLength);
	}

	header.append(hasPayload);
	header.append((const char*)&infoLenth, sizeof(qint32));

	if (hasPayload) {
		header.append((const char*)&payloadLength, sizeof(qint32));
	}

	qDebug() << "Exit - Created header for Message - P=" << hasPayload << "Il=" << infoLenth << "Pl" << payloadLength;
	return header;
}
