#pragma once
/*!
* @author	VelazcoJD
* @file		ClientTCP.h
* @class	fi3d::ClientTCP
* @brief	Functions as a TCP client with SSL/TLS support.
*/

#include <QSslSocket>

#include <QSharedPointer>

namespace fi3d {
class ClientTCP : public QSslSocket {

	Q_OBJECT

public:
	/// @brief Constructor.
	ClientTCP();

	/// @brief Destructor.
	~ClientTCP();
};

/// @brief Alias for a smart pointer of this class.
using ClientTCPPtr = QSharedPointer<ClientTCP>;

}