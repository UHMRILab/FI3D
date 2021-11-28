#include <fi3d/server/network/ServerFI3D.h>

#include <fi3d/server/network/ClientFI3D.h>

using namespace fi3d;

ServerFI3D::ServerFI3D(const bool& enableSSL)
	: ServerTCP(enableSSL)
{
}

ServerFI3D::~ServerFI3D() {}

ClientTCP* ServerFI3D::makeClientTCPSocket() const {
	return new ClientFI3D();
}
