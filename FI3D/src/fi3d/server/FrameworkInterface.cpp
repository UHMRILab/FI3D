#include <fi3d/server/FrameworkInterface.h>

#include <fi3d/logger/Logger.h>

#include <QtEndian>
#include <QByteArray>
#include <QJsonDocument>
#include <QTcpSocket>

using namespace fi3d;

FrameworkInterface::FrameworkInterface(const QString& FIID)
	: ClientFI3D(),
	mFIID(FIID),
	mIsAuthenticated(false)
{
}

FrameworkInterface::~FrameworkInterface() {}

void FrameworkInterface::setAuthenticated(const bool& isAuthenticated) {
	qDebug() << "Enter - Authenticated:" << isAuthenticated;
	mIsAuthenticated = isAuthenticated;
	qDebug() << "Exit";
}

bool FrameworkInterface::isAuthenticated() const {
	return mIsAuthenticated;
}

void FrameworkInterface::setFIID(const QString& FIID) {
	mFIID = FIID;
}

QString FrameworkInterface::getFIID() const {
	return mFIID;
}
