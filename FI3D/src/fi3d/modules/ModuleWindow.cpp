#include <fi3d/modules/ModuleWindow.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/modules/IFeedbackEmitter.h>

#include <fi3d/modules/ModuleHandler.h>

#include <QStatusBar>

using namespace fi3d;

ModuleWindow::ModuleWindow() 
	: QMainWindow(),
	ModuleElement()
{
}

ModuleWindow::~ModuleWindow() {}

void ModuleWindow::connectFeedbackEmitter(IFeedbackEmitter* emitter) {
	QObject* emi = dynamic_cast<QObject*>(emitter);

	QObject::connect(
		emi, SIGNAL(feedback(const QString&)),
		this, SLOT(feedback(const QString&)),
		Qt::UniqueConnection);
	QObject::connect(
		emi, SIGNAL(feedbackWarning(const QString&)),
		this, SLOT(feedbackWarning(const QString&)),
		Qt::UniqueConnection);
	QObject::connect(
		emi, SIGNAL(feedbackError(const QString&)),
		this, SLOT(feedbackError(const QString&)),
		Qt::UniqueConnection);
	QObject::connect(
		emi, SIGNAL(feedbackColor(const QString&, const Qt::GlobalColor&)),
		this, SLOT(feedbackColor(const QString&, const Qt::GlobalColor&)),
		Qt::UniqueConnection);
}

void ModuleWindow::disconnectFeedbackEmitter(IFeedbackEmitter* emitter) {
	QObject* emi = dynamic_cast<QObject*>(emitter);

	QObject::disconnect(
		emi, SIGNAL(feedback(const QString&)),
		this, SLOT(feedback(const QString&)));
	QObject::disconnect(
		emi, SIGNAL(feedbackWarning(const QString&)),
		this, SLOT(feedbackWarning(const QString&)));
	QObject::disconnect(
		emi, SIGNAL(feedbackError(const QString&)),
		this, SLOT(feedbackError(const QString&)));
	QObject::disconnect(
		emi, SIGNAL(feedbackColor(const QString&, const Qt::GlobalColor&)),
		this, SLOT(feedbackColor(const QString&, const Qt::GlobalColor&)));
}

void ModuleWindow::handleFeedback(const QString& message, const QColor& color) {
	qInfo() << this->getModuleID() << "Feedback:" << message;
	if (this->statusBar()) {
		this->statusBar()->setStyleSheet(tr("color: %1").arg(color.name()));
		this->statusBar()->showMessage(message);
	}
}

void ModuleWindow::feedback(const QString& message) {
	this->handleFeedback(message, QColor("#fff"));
}

void ModuleWindow::feedbackError(const QString& message) {
	this->handleFeedback(message, QColor("#ff2727"));
}

void ModuleWindow::feedbackWarning(const QString& message) {
	this->handleFeedback(message, QColor("#ffb92c"));
}

void ModuleWindow::feedbackColor(const QString& message, const Qt::GlobalColor& color) {
	this->handleFeedback(message, color);
}

void ModuleWindow::closeEvent(QCloseEvent* event) {
	this->getModuleHandler()->closeModule();
	QMainWindow::closeEvent(event);
}
