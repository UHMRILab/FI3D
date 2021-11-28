#include <fi3d/server/ServerDialog.h>

#include <QInputDialog>

using namespace fi3d;

ServerDialog::ServerDialog(const QString& ip, const int& port, 
	const QString& password, const int& connections, 
	const int& unidentifiedConnections)
	: QDialog()
{
	ui.setupUi(this);
	ui.serverAddress_label->setText(ip);
	ui.serverPort_label->setText(tr("%1").arg(port));
	ui.password_label->setText(password);
	ui.connectedDevices_label->setText(tr("%1").arg(connections));
	ui.unidentifiedDevices_label->setText(tr("%1").arg(unidentifiedConnections));

	this->setWindowTitle("Server Information");
	QObject::connect(
		ui.changePassword_button, &QPushButton::clicked, 
		this, &ServerDialog::onChangePasswordClick); //
}

ServerDialog::~ServerDialog() {}

void ServerDialog::onChangePasswordClick() {
	bool ok = false;
	QString newPassword = QInputDialog::getText(this, tr("Enter password"), 
		tr("Enter new password: "),	QLineEdit::Normal, 
		ui.password_label->text(), &ok);

	if (ok && !newPassword.isEmpty()) {
		emit changePassword(newPassword);
	}
}

void ServerDialog::updateServerIP(const QString& ip) {
	ui.serverAddress_label->setText(ip);
}

void ServerDialog::updateServerPort(const int& port) {
	ui.serverPort_label->setText(tr("%1").arg(port));
}

void ServerDialog::updatePassword(const QString& password) {
	ui.password_label->setText(password);
}

void ServerDialog::updateConnectedDevices(const int& count) {
	ui.connectedDevices_label->setText(tr("%1").arg(count));
}

void ServerDialog::updateUnidentifiedConnections(const int& count) {
	ui.unidentifiedDevices_label->setText(tr("%1").arg(count));
}
