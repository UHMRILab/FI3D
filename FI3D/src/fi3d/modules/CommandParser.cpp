#include <fi3d/modules/CommandParser.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/rendering/scenes/Scene.h>

using namespace fi3d;

CommandParser::CommandParser()
	: QObject(Q_NULLPTR),
	ModuleElement()
{}

CommandParser::~CommandParser() {}

void CommandParser::parseCommand(const QString& command) {
	qDebug() << "Enter - Command:" << command;
	QStringList args = command.split(" ");
	
	if (args.count() == 0) {
		qDebug() << "Exit - No arguments were given";
		return;
	}

	
	QString commandID = args[0].toLower();
	qDebug() << "CommandID:" << commandID;
	if (commandID == "rotatex") {
		if (!this->getMainScene().isNull()) {
			this->getMainScene()->rotateXVisual(args[2].toDouble(), args[1]);
		}
	} else if (commandID == "rotatey") {
		if (!this->getMainScene().isNull()) {
			this->getMainScene()->rotateYVisual(args[2].toDouble(), args[1]);
		}
	} else if (commandID == "rotatez") {
		if (!this->getMainScene().isNull()) {
			this->getMainScene()->rotateZVisual(args[2].toDouble(), args[1]);
		}
	} else if (commandID == "translatex") {
		if (!this->getMainScene().isNull()) {
			this->getMainScene()->translateVisual(args[2].toDouble(), 0.0, 0.0, args[1]);
		}
	} else if (commandID == "translatey") {
		if (!this->getMainScene().isNull()) {
			this->getMainScene()->translateVisual(0.0, args[2].toDouble(), 0.0, args[1]);
		}
	} else if (commandID == "translatez") {
		if (!this->getMainScene().isNull()) {
			this->getMainScene()->translateVisual(0.0, 0.0, args[2].toDouble(), args[1]);
		}
	} else {
		handleCommand(command);
		qDebug() << "Exit - Custom command was given";
	}

	qDebug() << "Exit";
}

void CommandParser::handleCommand(const QString& command) {}