#include <fi3d/logger/Logger.h>

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QObject>

#include <vtkOutputWindow.h>
#include <vtkFileOutputWindow.h>

using namespace fi3d;

QFile* Logger::logFile = Q_NULLPTR;
bool Logger::isInit = false;
QHash<QtMsgType, QString> Logger::contextNames = {
    {QtMsgType::QtDebugMsg,		" Debug  "},
	{QtMsgType::QtInfoMsg,		"  Info  "},
	{QtMsgType::QtWarningMsg,	"Warning "},
	{QtMsgType::QtCriticalMsg,	"Critical"},
    {QtMsgType::QtFatalMsg,		" Fatal  "}
};

bool Logger::init() {
	if (isInit) {
		return true;
	}

	// Create the log file either in the FI3D data directory (if it exists),
	// or in the current working directory.
	logFile = new QFile;
	QString dir = QObject::tr("%1/FI3D").arg(FI3D_DATA_PATH);
	if (QDir(dir).exists()) {
		QString fileName = QObject::tr("%1/%2").arg(dir).arg("FI3D.log");
		logFile->setFileName(fileName);
	} else {
		logFile->setFileName("./FI3D.log");
	}
    
	logFile->open(QIODevice::Append | QIODevice::Text);

	qInstallMessageHandler(Logger::messageOutput);

	logFile->resize(0);

	// Redirect VTK's messages to this same log file
	vtkFileOutputWindow* outputWin = vtkFileOutputWindow::New();
	outputWin->SetFileName("vtkFI3D.log");
	outputWin->AppendOn();
	vtkOutputWindow::SetInstance(outputWin);
	outputWin->Delete();

	Logger::isInit = true;
	return Logger::isInit;
}

void Logger::clean() {
	if (logFile != Q_NULLPTR) {
		logFile->close();
		delete logFile;
	}
}

void Logger::messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg) {

#ifndef QT_MESSAGELOGCONTEXT
	if (type == QtDebugMsg) {
		return;
	}
#endif

	QString log = QObject::tr("%1 | %2 | ")
		.arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"))
		.arg(Logger::contextNames.value(type));

#ifdef QT_MESSAGELOGCONTEXT
	log = QObject::tr("%1%2 | %3 | %4 | ").arg(log)
		.arg(context.line)
		.arg(QString(context.file)
			.section('\\', -1))			// File name without file path											
		.arg(QString(context.function)
			.section('(', -2, -2)		// Function name only
			.section(' ', -1)
			.section(':', -1)); 		
#endif

	log += msg + "\n";

	logFile->write(log.toLocal8Bit());
	logFile->flush();
}
