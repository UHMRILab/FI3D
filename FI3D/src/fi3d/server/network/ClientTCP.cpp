#include "fi3d/server/network/ClientTCP.h"

#include <QJsonDocument>
#include <QtEndian>

using namespace fi3d;

ClientTCP::ClientTCP()
	: QSslSocket()
{
}

ClientTCP::~ClientTCP() {}