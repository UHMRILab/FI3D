#include <FI/ModuleMenuDialog.h>

#include <FI/GlobalsFI.h>

#include <fi3d/logger/Logger.h>
#include <fi3d/server/message_keys/MessageKeys.h>
#include <fi3d/utilities/Icons.h>

#include "ui_ModuleMenuDialog.h"

#include <QKeyEvent>
#include <QPushButton>
#include <QToolButton>

using namespace fi;

ModuleMenuDialog::ModuleMenuDialog(QWidget* parent) :
    QDialog(parent),
    ModuleElement(),
    mGUI(new Ui::ModuleMenuDialog),
    mAvailableModulesSet(false)
{
    mGUI->setupUi(this);
    mGUI->disconnect_button->setDisabled(true);

    QObject::connect(
        mGUI->connect_button, &QPushButton::clicked,
        this, &ModuleMenuDialog::onConnectClick);
    QObject::connect(
        mGUI->disconnect_button, &QPushButton::clicked,
        this, &ModuleMenuDialog::onDisconnectClick);
}

ModuleMenuDialog::~ModuleMenuDialog() {
    delete mGUI;
}

void ModuleMenuDialog::setAvailableModules(const QList<QString>& availableModules) {
    if (mAvailableModulesSet) {
        return;
    }

    this->clearAvailableModuleButtons();

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(mGUI->availableModules_frame->layout());
    for (int i = availableModules.count() - 1; i >= 0; i--) {
        QString modu = availableModules[i];

        QPushButton* moduButton = new QPushButton(modu, mGUI->availableModules_frame);

        layout->insertWidget(0, moduButton);

        // This module shouldn't need to make other modules of this type.
        if (modu == this->getModuleName()) {
            moduButton->setDisabled(true);
            moduButton->setText(tr("%1 (This Module)").arg(modu));
        }

        QObject::connect(
            moduButton, &QPushButton::clicked,
            this, &ModuleMenuDialog::onAvailableModuleClick);
    }

    mAvailableModulesSet = true;
}

void ModuleMenuDialog::setActiveModules(const QList<QString>& activeModules) {
    this->clearActiveModuleButtons();

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(mGUI->activeModules_frame->layout());
    for (int i = activeModules.count() - 1; i >= 0; i--) {
        QString modu = activeModules[i];

        QPushButton* moduButton = new QPushButton(modu, mGUI->activeModules_frame);

        QHBoxLayout* moduLayout = new QHBoxLayout(mGUI->activeModules_frame);
        moduLayout->addWidget(moduButton);

        if (modu == this->getModuleID()) {
            // This module shouldn't be able to subscribe to itself.
            moduButton->setDisabled(true);
            moduButton->setText(tr("%1 (This Module)").arg(modu));
        } else {
            QToolButton* closeButton = new QToolButton(mGUI->activeModules_frame);
            closeButton->setIcon(fi3d::Icons::getIcon(fi3d::EIcon::SHUTDOWN));
            closeButton->setProperty(fi3d::MODULE_ID.toStdString().c_str(), modu);
            moduLayout->addWidget(closeButton);

            QObject::connect(
                closeButton, &QToolButton::clicked,
                this, &ModuleMenuDialog::onStopModuleClick);
        }
        
        layout->insertLayout(0, moduLayout);

        QObject::connect(
            moduButton, &QPushButton::clicked,
            this, &ModuleMenuDialog::onActivateModuleClick);
    }
}

void ModuleMenuDialog::clearAvailableModuleButtons() {
    while (mGUI->availableModules_frame->layout()->count() != 1) {
        QLayoutItem* child = mGUI->availableModules_frame->layout()->takeAt(0);
        delete child->widget();
        delete child;
    }
    mAvailableModulesSet = false;
}

void ModuleMenuDialog::clearActiveModuleButtons() {
    while (mGUI->activeModules_frame->layout()->count() != 1) {
        QLayout* moduleLayout = mGUI->activeModules_frame->layout()->takeAt(0)->layout();
        
        while (moduleLayout->count() != 0) {
            QLayoutItem* item = moduleLayout->takeAt(0);
            delete item->widget();
            delete item;
        }

        delete moduleLayout;
    }
}

void ModuleMenuDialog::onAvailableModuleClick() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    auto inte = this->getStringModuleInteraction(MI_ACTIVATE_MODULE);
    inte->setValue(btn->text());

    inte->blockSignals(true);
    inte->setValue("");
    inte->blockSignals(false);
}

void ModuleMenuDialog::onActivateModuleClick() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    auto inte = this->getStringModuleInteraction(MI_SUBSCRIBE_TO_MODULE);
    inte->setValue(btn->text());
}

void ModuleMenuDialog::onStopModuleClick() {
    QObject* closeButton = sender();
    
    QString moduleID = closeButton->property(fi3d::MODULE_ID.toStdString().c_str()).toString();

    auto inte = this->getStringModuleInteraction(MI_STOP_MODULE);
    inte->setValue(moduleID);

    inte->blockSignals(true);
    inte->setValue("");
    inte->blockSignals(false);
}

void ModuleMenuDialog::onConnectClick() {
    auto ipInte = this->getStringModuleInteraction(MI_IP);
    ipInte->setValue(mGUI->IP_text->text());

    auto portInte = this->getIntegerModuleInteraction(MI_PORT);
    portInte->setValue(mGUI->port_text->text().toInt());

    auto passInte = this->getStringModuleInteraction(MI_PASSWORD);
    passInte->setValue(mGUI->password_text->text());
    
    auto connectInte = this->getValuelessModuleInteraction(MI_CONNECT);
    connectInte->triggerInteraction();
}

void ModuleMenuDialog::onDisconnectClick() {
    auto disconnectInte = this->getValuelessModuleInteraction(MI_DISCONNECT);
    disconnectInte->triggerInteraction();
}

void ModuleMenuDialog::closeEvent(QCloseEvent* event) {
    emit dialogClosing();
    QDialog::closeEvent(event);
}

void ModuleMenuDialog::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key::Key_Enter) {
        if (mGUI->connect_button->isEnabled()) {
            this->onConnectClick();
        } else {
            QDialog::keyPressEvent(event);
        }
    } else {
        QDialog::keyPressEvent(event);
    }
}

void ModuleMenuDialog::setConnectionFeedback(const QString& feedback) {
    mGUI->status_label->setText(feedback);
}

void ModuleMenuDialog::updateIP(const QString& IP) {
    mGUI->IP_text->blockSignals(true);
    mGUI->IP_text->setText(IP);
    mGUI->IP_text->blockSignals(false);
}

void ModuleMenuDialog::updatePort(const int& port) {
    mGUI->port_text->blockSignals(true);
    mGUI->port_text->setText(tr("%1").arg(port));
    mGUI->port_text->blockSignals(false);
}

void ModuleMenuDialog::updatePassword(const QString& password) {
    mGUI->password_text->blockSignals(true);
    mGUI->password_text->setText(password);
    mGUI->password_text->blockSignals(false);
}

void ModuleMenuDialog::updateConnected() {
    mGUI->IP_text->setDisabled(true);
    mGUI->port_text->setDisabled(true);
    mGUI->password_text->setDisabled(true);
    mGUI->connect_button->setDisabled(true);
    mGUI->disconnect_button->setDisabled(false);
}

void ModuleMenuDialog::updateDisconnected() {
    mGUI->IP_text->setDisabled(false);
    mGUI->port_text->setDisabled(false);
    mGUI->password_text->setDisabled(false);
    mGUI->connect_button->setDisabled(false);
    mGUI->disconnect_button->setDisabled(true);

    this->clearAvailableModuleButtons();
    this->clearActiveModuleButtons();
}
