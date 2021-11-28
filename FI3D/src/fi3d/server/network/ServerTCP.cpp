#include <fi3d/server/network/ServerTCP.h>

#include <fi3d/FI3D/FI3D.h>

#include <QFile>
#include <QJsonDocument>
#include <QNetworkInterface>
#include <QtEndian>

using namespace fi3d;

ServerTCP::ServerTCP(const bool& enableSSL)
	: QTcpServer(),
	ENABLE_SSL(enableSSL),
	mSslKey(),
	mSslCertificate()
{
	if (ENABLE_SSL) {
		QByteArray key;
		QByteArray cert;

		QFile file_key(tr("%1\\FI3D\\network\\server.key").arg(FI3D::ASSETS_DIRECTORY));
		if (file_key.open(QIODevice::ReadOnly))     {
			key = file_key.readAll();
			file_key.close();
		} else {
			qDebug() << file_key.errorString();
		}

		QFile file_cert(tr("%1\\FI3D\\network\\server.crt").arg(FI3D::ASSETS_DIRECTORY));
		if (file_cert.open(QIODevice::ReadOnly))     {
			cert = file_cert.readAll();
			file_cert.close();
		} else {
			qDebug() << file_cert.errorString();
		}

		mSslKey = QSslKey(key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "server");
		mSslCertificate = QSslCertificate(cert);
	}
}

ServerTCP::~ServerTCP() {}

bool ServerTCP::startServer(const int& port) {
	qDebug() << "Enter";
	if (this->isListening()) {
		if (port == this->serverPort()) {
			qDebug() << "Exit - Server already started";
			return true;
		} else {
			this->close();
		}
	}

	if (!this->listen(QHostAddress::Any, port)) {
		qCritical() << "Failed to start server: " << this->errorString();
		qDebug() << "Exit - Failed to start server";
		return false;
	}

	qDebug() << "Exit";
	return true;
}

bool ServerTCP::isSSLEnabled() const {
	return ENABLE_SSL;
}

QString ServerTCP::getIPAddress() const {
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	QString ip;
	
	// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address()) {
			ip = ipAddressesList.at(i).toString();
			break;
		}
	}

	// if we did not find one, use IPv4 localhost
	if (ip.isEmpty()) {
		ip = QHostAddress(QHostAddress::LocalHost).toString();
	}

	return ip;
}

void ServerTCP::incomingConnection(qintptr socketDescriptor) {
	qDebug() << "Enter";

	ClientTCP* client = this->makeClientTCPSocket();

	if (!client->setSocketDescriptor(socketDescriptor)) {
		delete client;
		return;
	}

	if (ENABLE_SSL) {
		client->setPrivateKey(mSslKey);
		client->setLocalCertificate(mSslCertificate);
		client->setProtocol(QSsl::AnyProtocol);
		client->setPeerVerifyMode(QSslSocket::VerifyNone);
		client->startServerEncryption();

		connect(
			client, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
			[=](const QList<QSslError>& errors) 
		{ 			
			for (QSslError error : errors) {
				qWarning() << "SSL Error: " << error.errorString();
			}
			qWarning() << "These errors will be ignored";
			client->ignoreSslErrors();
		});
	}

	this->addPendingConnection(client);

	qDebug() << "Exit";
}

ClientTCP* ServerTCP::makeClientTCPSocket() const {
	return new ClientTCP();
}
