#include <fi3d/FI3D/FI3DControllerWindow.h>

#include <fi3d/logger/Logger.h>

#include "ui_FI3DControllerWindow.h"

using namespace fi3d;

FI3DControllerWindow::FI3DControllerWindow(QWidget *parent) :
    QMainWindow(parent),
	mGUI(new Ui::FI3DControllerWindow)
{
	mGUI->setupUi(this);

	mComponentButton = mGUI->componentSample_button;
	mModuleButton = mGUI->moduleSample_button;

	mComponentButton->setVisible(false);
	mModuleButton->setVisible(false);
}

FI3DControllerWindow::~FI3DControllerWindow() {
    delete mGUI;
}

void FI3DControllerWindow::setComponents(QList<QString> componentNames) {
	componentNames.sort();
	QVBoxLayout* components = qobject_cast<QVBoxLayout*>(mGUI->componentList_layout->layout());

	for (int i = componentNames.count() - 1; i >= 0; i--) {
		QPushButton* component = new QPushButton(componentNames[i], mGUI->componentList_layout);
		component->setFont(mComponentButton->font());
		
		components->insertWidget(0, component);

		QObject::connect(
			component, &QPushButton::clicked, 
			this, &FI3DControllerWindow::onComponentClicked);
	}
}

void FI3DControllerWindow::setModules(QList<QString> moduleNames) {
	moduleNames.sort();
	QVBoxLayout* modules = qobject_cast<QVBoxLayout*>(mGUI->moduleList_layout->layout());

	for (int i = moduleNames.count() - 1; i >= 0; i--) {
		QPushButton* module = new QPushButton(moduleNames[i], mGUI->moduleList_layout);
		module->setFont(mModuleButton->font());

		modules->insertWidget(0, module);

		QObject::connect(
			module, &QPushButton::clicked,
			this, &FI3DControllerWindow::onModuleClicked);
	}
}

void FI3DControllerWindow::onComponentClicked() {
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	if (btn == Q_NULLPTR) {
		qWarning() << "Component event triggered on a non-button object.";
		return;
	}

	emit componentButtonClicked(btn->text());
}

void FI3DControllerWindow::onModuleClicked() {
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	if (btn == Q_NULLPTR) {
		qWarning() << "Component event triggered on a non-button object.";
		return;
	}

	emit moduleButtonClicked(btn->text(), btn->text());
}
