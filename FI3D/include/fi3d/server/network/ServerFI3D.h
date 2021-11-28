#pragma once
/*!
* @author	VelazcoJD
* @file		ServerFI3D.h
* @class	fi3d::ServerFI3D
* @brief	Functions as a TCP server with SSL/TLS support and adheres to the
*			FI3D communication protocol.
* 
* The SSL feature can be set at creation of the instance. Once the server
* has been designated as SSL enabled or not, this property cannot be changed.
* 
* The client connections managed have functionalities to parse incoming data
* using the FI3D standard (see ClientFI3D for more information). If simple
* TCP transfer of data is needed, use the ServerTCP/ClientTCP classes instead.
*/

#include <fi3d/server/network/ServerTCP.h>


namespace fi3d {
class ServerFI3D : public ServerTCP {

	Q_OBJECT

public:
	/// @brief Constructor.
	ServerFI3D(const bool& enableSSL = false);

	/// @brief Destructor.
	~ServerFI3D();

protected:
	/// @brief Creates a ClientFI3D to be used instead of ClientTCP.
	virtual ClientTCP* makeClientTCPSocket() const override;
};
}