#include <fi3d/utilities/Icons.h>

#include <fi3d/FI3D/FI3D.h>

using namespace fi3d;

Icons::Icons() {}

Icons::~Icons() {}

QIcon Icons::getIcon(const EIcon& iconType) {
	if (iconType == EIcon::UNKNOWN) {
		return QIcon();
	}

	QString iconPath = QObject::tr("%1/FI3D/icons/%2.png").arg(FI3D::ASSETS_DIRECTORY).arg(iconType.getName());
	QIcon icon(iconPath);
	return icon;
}