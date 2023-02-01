#include <TEMPLATE/ModuleWindowTEMPLATE.h>

#include "ui_ModuleWindowTEMPLATE.h"

using namespace template;

ModuleWindowTEMPLATE::ModuleWindowTEMPLATE()
	: ModuleWindow(),
	mGUI(new Ui::ModuleWindowTEMPLATE)
{
	mGUI->setupUi(this);
}

ModuleWindowTEMPLATE::~ModuleWindowTEMPLATE() {}

QVTKOpenGLWidget* ModuleWindowTEMPLATE::getSceneWidget() {
	return mGUI->Frame3D;
}