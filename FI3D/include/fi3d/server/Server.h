#pragma once
/*!
* @author	VelazcoJD
* @file		Server.h
* @class	fi3d::Server
* @brief	The server is the FI3D componenet used to communicate with
*			the framework interfaces (FIs).
*/

#include <fi3d/FI3D/FI3DComponentRegistration.h>

#include <fi3d/server/network/ServerFI3D.h>
#include <fi3d/server/network/Message.h>

#include <fi3d/server/FrameworkInterface.h>

#include <QJsonObject>

#include <QVector>

class QAbstractSocket;

namespace fi3d {
class ServerDialog;

class Server : public ServerFI3D {
	
	Q_OBJECT
	FI3D_COMPONENT_WITH_GUI(Server)

	/****************** Static members for the singleton ******************/
public:
	/*! 
	 * @brief Sends the given message to the client with the given ID. 
	 *
	 * The receiving Client must be authorized. 
	 */
	static void sendMessage(MessagePtr message, const QString& clientID);

	/*!
	 * @brief Sends the given message (info only) to the client with the given 
	 * ID.
	 *
	 * The receiving Client must be authorized.
	 */
	static void sendMessage(const QJsonObject& info, const QString& clientID);

	/*! 
	 * @brief Sends the given message to the given list of clients. 
	 *
	 * The receiving clients must be authorized. 
	 */
	static void sendSelectMessage(MessagePtr message, const QVector<QString>& clientIDs);

	/*!
	 * @brief Sends the given message (info only) to the given list of clients.
	 *
	 * The receiving clients must be authorized.
	 */
	static void sendSelectMessage(const QJsonObject& info, const QVector<QString>& clientIDs);

	/// @brief Sends the given message to all authorized clients.
	static void sendGlobalMessage(MessagePtr message);

	/// @brief Sends the given message (info only) to all authorized clients.
	static void sendGlobalMessage(const QJsonObject& info);

private:
	/// @brief Gets the password for the server. 
	static QString getPassword();

	/// @breif Sets the password for the server.
	static void setPassword(const QString& password);

public:
	/// @brief Gets the IP address of the server.
	static QString GetIPAddress();

	/// @brief Gets the port number of the server.
	static int GetPortNumber();

	/// @brief Gets a count of the connected devices.
	static int GetHMDCount();

	/// @brief Gets the count of unidentified connections.
	static int getUnidentifiedCount();

	/// @brief Gets a pointer to the running server.
	static const Server* getInstance();

	/****************** Singleton members ******************/
signals:
	/// @brief Emitted when a new client has connected.
	void changedClientConnected(const QString& connectionName) const;

	/// @brief Emitted when a client has disconnected.
	void changedClientDisconnected(const QString& connectionName) const;

	/// @brief Emitted when a client has been authenticated successfully.
	void changedClientIdentified(const QString& connectionName) const;

	/// @brief Emitted when a module type request has been received.
	void changedModuleRequest(QJsonObject& moduleParams, const QString& clientID, const QString& message) const;

	/// @brief Emitted when an application type request has been received.
	void changedApplicationRequest(QJsonObject& applicationParams, 
		const QString& clientID, const QString& message) const;

	/// @brief Emitted when a data type request has been received. 
	void changedDataRequest(QJsonObject& dataParams, const QString& clientID, const QString& message) const;

private:
	/// @brief The dialog which is used by the user to see server information.
	QSharedPointer<ServerDialog> mDialog;

	/// @brief The IP address that a client can use to connect to.
	QString mIPAddress;

	/// @brief The port that a client can use to connect to.
	int mPort;

	/// @brief Password used to identify connections.
	QString mPassword;

	/// @brief List of authenticated clients.
	QHash<QString, FrameworkInterface*> mAuthenticatedClients;

	/// @brief List of unauthenticated clients.
	QHash<QString, FrameworkInterface*> mUnauthenticatedClients;

	/// @brief Keeps a running count of connections, used to ID new connections.
	unsigned long long mDeviceCount;

private:
	/// @brief Private constructor. Only one server can exist.
	Server();

public:
	/// @brief Destructor.
	~Server();

private:
	/// @brief Stops the server.
	void stopServer();

	/// @brief Attempts to authenticate the connection with the given clientID.
	void authenticateConnection(const QString& clientID, const QString& requestObject);

private slots:
	/// @brief Gets called when a new connection is made.
	void onNewConnection();

	/// @brief Gets called when a new connection failed to open.
	void onNewConnectionError(QAbstractSocket::SocketError error);

	/// @brief Gets called when a connection is closed.
	void onCloseConnection();

	/// @brief Gets called once a message is received from a client.
	void onMessage(MessagePtr request);

protected:
	/// @brief Creates a FrameworkInterface to be used instead of ClientFI3D.
	virtual ClientTCP* makeClientTCPSocket() const override;

private:
	/// @brief Used to calculate the length of the Message header with and 
	/// without payload
	static int sHeaderWithoutPayloadLength, sHeaderWithPayloadLength;

public:
	/// @brief Creates the Message header based on the given Message.
	static QByteArray createHeader(MessagePtr message);

	/// @brief Creates the Message header based on given information.
	static QByteArray createHeader(const qint32& infoLenth, 
		const bool& hasPayload = false, const qint32& payloadLength = 0);
};
}