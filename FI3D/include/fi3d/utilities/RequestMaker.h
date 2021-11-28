#pragma once
/*!
* @author	VelazcoJD
* @file		RequestMaker.h
* @class	fi3d::RequestMaker
* @brief	Basic functionality for a TCP client.
*/

#include <QObject>
#include <QTcpSocket>

namespace fi3d {
class RequestMaker : public QObject {

	Q_OBJECT

signals:
	/// @brief Emitted when the IP changes.
	void changedIP(const QString& IP);

	/// @brief Emitted when the Port changes.
	void changedPort(const int& port);

	/// @brief Emitted when the connection has been established.
	void connected();

	/// @brief Emitted when the connection ended.
	void disconnected();

	/// @brief Emitted when a connection error occurs.
	void connectionError(const QAbstractSocket::SocketError& error);

	/// @brief Emitted when a message is received.
	void newMessage(const QByteArray& message);

private:
	/// @brief The IP that this client is connected to.
	QString mIP;

	/// @brief The port that this client is connected to.
	int mPort;

	/// @brief The connection socket.
	QTcpSocket mConnection;

	/// @brief The size of the message that is being received.
	int mMessageSize;

	/// @brief The message being received.
	QByteArray mReceivingMessage;

public:
	/// @brief Constructor.
	RequestMaker();

	/// @brief Destructor.
	~RequestMaker();

public slots:
	/// @brief Connects to the server based on stored IP and port.
	void attemptConnection();

	/*!
	 * @brief Sets IP but waits to connect for attemptConnection call.
	 *
	 * If there is currently an active connection, it is closed first.
	 */
	void setConnectionIP(const QString& IP);

	/*!
	 * @brief Sets Port but waits to connect for attemptConnection call.
	 *
	 * If there is currently an active connection, it is closed first.
	 */
	void setConnectionPort(const int& port);

	/*!
	 * @brief Sets IP and Port but waits to connect for attemptConnection call.
	 *
	 * If there is currently an active connection, it is closed first.
	 */
	void setConnectionParameters(const QString& IP, const int& port);

	/*!  
	 * @brief Connects to the given IP and port. 
	 *
	 * If it is already connected, disconnects first. 
	 */
	void connectToServer(const QString& IP, const int& port);

	/// @brief Disconnects from the current connection.
	void closeConnection();

	/// @brief Whether there is a running connection.
	bool isConnected() const;

public:
	/*!
	 * @brief Sends the given bytearray as a message.
	 *
	 * If includeSize is true, the message size is prepended to the message as
	 * a 4-byte integer.
	 *
	 * @param message The message to send.
	 * @param includeSize Whether the message should include the header.
	 */
	void sendRequest(QByteArray& message, const bool& includeSize = false);

	/*!
	 * @brief Sends the given QJsonObject as a message.
	 *
	 * If includeSize is true, the message size is prepended to the message as
	 * a 4-byte integer.
	 *
	 * @param message The message to send.
	 * @param includeSize Whether the message should include the header.
	 */
	void sendRequest(QJsonObject& message, const bool& includeSize = false);

private slots:
	/// @brief When a new message is received.
	void onPacket();

public:
	/// @brief Get the current IP of the connection.
	QString getIP() const;

	/// @brief Get the current port of the connection.
	int getPort() const;
};
}