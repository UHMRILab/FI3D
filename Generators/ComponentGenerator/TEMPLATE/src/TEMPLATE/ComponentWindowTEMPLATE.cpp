#include <TEMPLATE/ComponentWindowTEMPLATE.h>

#include "ui_ComponentWindowTEMPLATE.h"

using namespace template;

// @todo: Needing more fixes for this omnicon file.

ComponentWindowTEMPLATE::ComponentWindowTEMPLATE()
	: ComponentWindow(),
	mGUI(new Ui::ComponentWindowTEMPLATE)
{
	mGUI->setupUi(this);
}

ComponentWindowTEMPLATE::~ComponentWindowTEMPLATE() {}

QVTKOpenGLWidget* ComponentWindowTEMPLATE::getSceneWidget() {
	return mGUI->Frame3D;
}