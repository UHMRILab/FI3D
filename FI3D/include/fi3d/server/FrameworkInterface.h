#pragma once
/*!
* @author	VelazcoJD
* @file		FrameworkInterface.h
* @class	fi3d::FrameworkInterface
* @brief	Information about a framework interface (FI).
*/

#include <fi3d/server/network/ClientFI3D.h>

#include <fi3d/server/network/Message.h>

class QByteArray;
class QTcpSocket;

namespace fi3d {
class FrameworkInterface : public ClientFI3D {

	Q_OBJECT

private:
	/// @brief The ID of the FI.
	QString mFIID;

	/// @brief Whether this client is authenticated, set by the server.
	bool mIsAuthenticated;

public:
	/// @brief Constructor.
	FrameworkInterface(const QString& FIID = "");

	/// @brief Destructor.
	~FrameworkInterface();

	/// @brief Change the authentication flag.
	void setAuthenticated(const bool& isAuthenticated);

	/// @brief Checks whether this client is authenticated.
	bool isAuthenticated() const;
	
	/// @brief Sets the ID of the FI.
	void setFIID(const QString& FIID);

	/// @brief Gets the ID of the FI.
	QString getFIID() const;
};

/// @brief Alias for a smart pointer of this class.
using FrameworkInterfacePtr = QSharedPointer<FrameworkInterface>;

}