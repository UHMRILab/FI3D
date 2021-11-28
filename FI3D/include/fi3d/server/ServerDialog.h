#pragma once
/*!
* @author	VelazcoJD
* @file		ServerDialog.h
* @class	fi3d::ServerDialog
* @brief	Displays information about the server and its clients in a dialog.
*/

#include "ui_ServerDialog.h"

#include <QDialog>

namespace fi3d {
class ServerDialog : public QDialog {
	Q_OBJECT

signals:
	/// @brief Emitted when the user wants to change the password. 
	void changePassword(const QString& newPassword) const;

private:
	/// @brief The UI elements.
	Ui::ServerDialog ui;

public:
	/// @brief Constructor.
	ServerDialog(const QString& ip, const int& port, const QString& password, 
		const int& connections, const int& unidentifiedConnections);

	/// @brief Destructor.
	~ServerDialog();

private slots:
	/// @brief Handles the user clicking on the change password button.
	void onChangePasswordClick();

public slots:
	/// @brief Updates the server's IP.
	void updateServerIP(const QString& ip);

	/// @brief Updates the server's port.
	void updateServerPort(const int& port);

	/// @brief Updates the server's password.
	void updatePassword(const QString& password);

	/// @brief Updates the number of connected devices.
	void updateConnectedDevices(const int& count);

	/// @brief Updates the number of unidentified connected devices.
	void updateUnidentifiedConnections(const int& count);
};

/// @brief Alias for a smart pointer of this class.
using ServerDialogPtr = QSharedPointer<ServerDialog>;

}