#pragma once
/*!
* @author	VelazcoJD
* @file		Logger.h
* @class	fi3d::logger::Logger
* @brief	Logs information to file.
*
* In charge of logging information to file. Include this header in every file
* that wants to log something.
*
* Each log contains date, severity, file, function, line, and message.
*
* Severities: trace, debug, info, warning, critical, fatal.
*
* To log something do: qSeverity() << things to log.
* e.g. qCritical() << "Error! 5 + 3 != " << 9;
*
* Logs are stored in FI3D.log
*
* On Release mode, the logs do not display file, function, and line information.
* Debug logs are also ignored.
*/

#include <QDebug>

/// Forward declare QFile
class QFile;

/// Forward declrate the Qt Hash
template <typename K, typename V>
class QHash;

namespace fi3d {
class Logger {
private:
	/// @brief The file object where logs are written to.
	static QFile* logFile;

	/// @brief Whether the logger has being initialized.
	static bool isInit;

	/// @brief The different type of contexts.
    static QHash<QtMsgType, QString> contextNames;

public:
	/// @brief Initializes the logger.
	static bool init();

	/// @brief Cleans up the logger.
	static void clean();

	/// @brief The function which handles the logging of text.
	static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};
}
