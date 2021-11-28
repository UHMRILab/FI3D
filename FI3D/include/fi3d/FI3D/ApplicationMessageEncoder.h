#pragma once
/*!
* @author	VelazcoJD
* @file		ApplicationMessageEncoder.h
* @class	fi3d::ApplicationMessageEncoder
* @brief	Encodes messages for the overall application.
*
* Sends overall application information to every connected client. It also 
* handles messages indicating to start/stop a module.
*/

#include <fi3d/server/MessageEncoder.h>

#include <QHash>

namespace fi3d {
class ApplicationMessageEncoder : public fi3d::MessageEncoder {

	Q_OBJECT

protected:
	/// @brief List of modules that are active.
	QHash<QString, QString> mActiveModules;

signals:
	/// @brief Emitted when a request to start a module has been received.
	void changeStartModule(const QString& moduleName, const QString& moduleID);

	/// @brief Emitted when a request to stop a module has been received.
	void changeStopModule(const QString& moduleID);

public:
	/// @brief Constructor.
	ApplicationMessageEncoder();

	/// @brief Destructor.
	~ApplicationMessageEncoder();

public slots:
	/*!
	 * @brief Adds active module to list of active modules.
	 *
	 * This function results in a message sent to all clients informing them 
	 * of the new active module.
	 */
	virtual void addActiveModule(const QString& moduleID, const QString& moduleName);

	/*!
	 * @brief Removes active module from list of active modules.
	 *
	 * This function results in a message sent to all clients informing them of
	 * the removed active module.
	 */
	virtual void removeActiveModule(const QString& moduleID);

public:
	/// @brief Parses client requests.
	virtual void parseRequest(const QJsonObject& applicationParams, 
		const QString& clientID, const QString& message) override;

protected:
	/// @brief Parses a start module request.
	void parseStartModuleRequest(const QJsonObject& applicationParams, const QString& clientID);

	/// @brief Parses a stop module request.
	void parseStopModuleRequest(const QJsonObject& applicationParams, const QString& clientID);

	/// @brief When a new client is connected, send them module information.
	void onConnectedClient(const QString& ClientID) override;

	/// @brief Creates a response with application information.
	QJsonObject prepareApplicationResponse();
};

/// @brief Alias for a smart pointer of this class.
using ApplicationMessageEncoderPtr = QSharedPointer<fi3d::ApplicationMessageEncoder>;

}