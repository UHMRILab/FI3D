#include <DEMO/ModuleWindowDEMO.h>

#include <DEMO/GlobalsDEMO.h>

#include "ui_ModuleWindowDEMO.h"

using namespace demo;

ModuleWindowDEMO::ModuleWindowDEMO()
	: ModuleWindow(),
	mGUI(new Ui::ModuleWindowDEMO)
{
	mGUI->setupUi(this);

	// GUI Callbacks
	QObject::connect(
		mGUI->slice_slider, &QSlider::valueChanged,
		this, &ModuleWindowDEMO::onSlice);
	QObject::connect(
		mGUI->phase_slider, &QSlider::valueChanged,
		this, &ModuleWindowDEMO::onPhase);
	QObject::connect(
		mGUI->surface_checkBox, &QCheckBox::stateChanged,
		this, &ModuleWindowDEMO::onSurfaceEnable);
	QObject::connect(
		mGUI->animate_checkBox, &QCheckBox::stateChanged,
		this, &ModuleWindowDEMO::onAnimateEnable);
}

ModuleWindowDEMO::~ModuleWindowDEMO() {}

QVTKOpenGLWidget* ModuleWindowDEMO::getSceneWidget() {
	return mGUI->Frame3D;
}

void ModuleWindowDEMO::onSlice(const int& index) {
	this->setModuleInteractionValue(MI_SLICE, index);
}

void ModuleWindowDEMO::onPhase(const int& index) {
	this->setModuleInteractionValue(MI_PHASE, index);
}

void ModuleWindowDEMO::onSurfaceEnable() {
	bool isEnable = false;
	if (mGUI->surface_checkBox->isChecked()) {
		isEnable = true;
	}

	this->setModuleInteractionValue(MI_ENABLE_SURFACE, isEnable);
}

void ModuleWindowDEMO::onAnimateEnable() {
	bool isEnable = false;
	if (mGUI->animate_checkBox->isChecked()) {
		isEnable = true;
	}

	this->setModuleInteractionValue(MI_ANIMATE, isEnable);
}

void ModuleWindowDEMO::updateSlice(const int& index) {
	mGUI->slice_slider->blockSignals(true);
	mGUI->slice_slider->setValue(index);
	mGUI->slice_slider->blockSignals(false);
}

void ModuleWindowDEMO::updatePhase(const int& index) {
	mGUI->phase_slider->blockSignals(true);
	mGUI->phase_slider->setValue(index);
	mGUI->phase_slider->blockSignals(false);
}

void ModuleWindowDEMO::updateSurfaceEnable(const bool& isEnabled) {
	mGUI->surface_checkBox->blockSignals(true);
	mGUI->surface_checkBox->setChecked(isEnabled);
	mGUI->surface_checkBox->blockSignals(false);
}

void ModuleWindowDEMO::updateAnimateEnable(const bool& isEnabled) {
	mGUI->animate_checkBox->blockSignals(true);
	mGUI->animate_checkBox->setChecked(isEnabled);
	mGUI->animate_checkBox->blockSignals(false);
}

void ModuleWindowDEMO::updateIP(const QString& IP) {
	mGUI->IP_label->setText(IP);
}

void ModuleWindowDEMO::updatePort(const int& port) {
	mGUI->port_label->setText(tr("%1").arg(port));
}

void ModuleWindowDEMO::updateNumberOfConnectedFI(const int& connectionCount) {
	mGUI->connectedFI_label->setText(tr("%1").arg(connectionCount));
}
