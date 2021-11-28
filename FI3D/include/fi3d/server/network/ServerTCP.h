#pragma once
/*!
* @author	VelazcoJD
* @file		ServerTCP.h
* @class	fi3d::ServerTCP
* @brief	Functions as a TCP server with SSL/TLS support.
* 
* The SSL feature can be set at creation of the instance. Once the server
* has been designated as SSL enabled or not, this property cannot be changed.
* 
* Derived classes who wish to use derived types of ClientTCP for the socket
* connections should overrie the createClientTCPSocket function and return
* the derived instance instead. This function is used by nextPendingConnection
* to create the socket object that will be used for communication.
* 
* It is also possible to override nextPendingConnection to alter logic of
* new connections. See the QTcpServer documentation for more information.
*/

#include <fi3d/server/network/ClientTCP.h>

#include <QSslCertificate>
#include <QSslKey>
#include <QTcpServer>

namespace fi3d {
class ServerTCP : public QTcpServer{

	Q_OBJECT

private:
	/// @brief Whether SSL is enabled with the server.
	const bool ENABLE_SSL;

	///  @brief The SSL key when SSL is enabled.
	QSslKey mSslKey;

	///  @brief The SSL certificate when SSL is enabled.
	QSslCertificate mSslCertificate;

public:
	/// @brief Constructor.
	ServerTCP(const bool& enableSSL = false);

	/// @brief Destructor.
	~ServerTCP();

public slots:
	/* 
	 * @brief Starts the server.
	 * 
	 * This function wraps the QTcpServer listen function to add extra
	 * functionality, such as quickly change to a different port.
	 * 
	 * Calling this function when the server is running with the same port 
	 * does nothing. Calling this function with a different port while the
	 * server is running will stop the server and start again with the new 
	 * port.
	 * 
	 * @param port The port the server should listen to, 0 for any port.
	 */
	bool startServer(const int& port);

public:
	/// @brief Whether the server has SSL enabled.
	bool isSSLEnabled() const;

	/// @brief Get the current IP of the connection.
	QString getIPAddress() const;

protected:
	/*!
	 * @brief Handles the creation of the socket for an incoming connection.
	 *
	 * The function handles a new connection with the given descriptor. It
	 * ovverrides the native QTcpServer functionality to create SSL socket
	 * for the connection when SSL is enabled.
	 * 
	 * This function may be overriden to change new connection functionality.
	 * If overriden, make sure to call addPendingConnection with the created
	 * socket to preserv base functionality.
	 * 
	 * Note: To use custom made Tcp Classes, override the makeClientTCPSocket
	 * function. The socket is used here is obtained from it.
	 */
	virtual void incomingConnection(qintptr socketDescriptor) override;

	/*!
	 * @brief Creates the socket used for incoming connections.
	 * 
	 * Override this function to use a derived class of ClientTCP instead.
	 */
	virtual ClientTCP* makeClientTCPSocket() const;
};
}