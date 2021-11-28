#include "fi3d/utilities/RequestMaker.h"

#include <QJsonDocument>
#include <QtEndian>

using namespace fi3d;

RequestMaker::RequestMaker()
	: QObject(),
	mIP("localhost"),
	mPort(8080),
	mConnection(this),
	mMessageSize(-1),
	mReceivingMessage("")
{
	QObject::connect(
		&mConnection, &QTcpSocket::connected,
		this, &RequestMaker::connected);
	QObject::connect(
		&mConnection, &QTcpSocket::disconnected,
		this, &RequestMaker::disconnected);
	QObject::connect(
		&mConnection, &QTcpSocket::readyRead,
		this, &RequestMaker::onPacket);
	QObject::connect(
		&mConnection, SIGNAL(error(QAbstractSocket::SocketError)), 
		this, SLOT(connectionError(QAbstractSocket::SocketError)));
}

RequestMaker::~RequestMaker() {
	this->closeConnection();
}

void RequestMaker::setConnectionIP(const QString& IP) {
	this->setConnectionParameters(IP, mPort);
}

void RequestMaker::setConnectionPort(const int& port) {
	this->setConnectionParameters(mIP, port);
}

void RequestMaker::attemptConnection() {
	this->connectToServer(mIP, mPort);
}

void RequestMaker::setConnectionParameters(const QString& IP, const int& port) {
	this->closeConnection();
	if (mIP != IP) {
		mIP = IP;
		emit changedIP(mIP);
	}

	if (mPort != port) {
		mPort = port;
		emit changedPort(mPort);
	}
}

void RequestMaker::connectToServer(const QString& IP, const int& port) {
	qDebug() << "Enter - Connecting to IP=" << IP << "Port=" << port;
	this->setConnectionParameters(IP, port);
	mConnection.connectToHost(mIP, mPort);
	qDebug() << "Exit";
}

void RequestMaker::closeConnection() {
	qDebug() << "Enter - Closing connection - IP=" << mIP << "Port=" << mPort;
	mConnection.disconnectFromHost();
	qDebug() << "Exit";
}

bool RequestMaker::isConnected() const {
	return mConnection.state() == QAbstractSocket::ConnectedState;
}

void RequestMaker::sendRequest(QByteArray& message, const bool& includeSize) {
	qDebug() << "Enter";
	if (includeSize) {
		qint32 responseSize = sizeof(qint32) + message.size();
		message.insert(0, (const char*)&responseSize, sizeof(qint32));
	}

	qDebug() << "Sending Request of size" << message.size() << "->"  << message;
	mConnection.write(message);
	qDebug() << "Exit";
}

void RequestMaker::onPacket() {
	qDebug() << "Enter";
	int packetSize = mConnection.bytesAvailable();
	while (packetSize != 0) {
		if (mMessageSize == -1) {
			//QString msgSizeString = QString::fromUtf8(mConnection.read(4));
			//mMessageSize = msgSizeString.toInt();

			mMessageSize = qFromLittleEndian<qint32>(mConnection.read(4));

			qDebug() << "Received new message of size:" << mMessageSize;
			mMessageSize -= 4;
			packetSize -= 4;
		}

		QByteArray receivedBytes = mConnection.read(mMessageSize);
		mMessageSize -= receivedBytes.count();
		packetSize -= receivedBytes.count();

		mReceivingMessage.append(receivedBytes);
		if (mMessageSize == 0) {
			emit newMessage(mReceivingMessage);

			mMessageSize = -1;
			mReceivingMessage.clear();
		}
	}
	qDebug() << "Exit";
}

void RequestMaker::sendRequest(QJsonObject& message, const bool& includeSize) {
	QJsonDocument doc(message);
	QByteArray msg = doc.toJson(QJsonDocument::JsonFormat::Compact);
	this->sendRequest(msg, includeSize);
}

QString RequestMaker::getIP() const {
	return mIP;
}

int RequestMaker::getPort() const {
	return mPort;
}
