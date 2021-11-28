#pragma once
/*!
* @author	VelazcoJD
* @file		CommandParser.h
* @class	fi3d::CommandParser
* @brief	Parses through the given string and emits the appropriate command.
*
* Derived modules may derive their own CommandParser for module-specific 
* commands. This class calls the handleCommand function when a given command is 
* unknown. Essentially, derived classes should override handleCommand function
* to implement the logic to their own commands.
*/

#include <fi3d/modules/ModuleElement.h>
#include <fi3d/modules/IFeedbackEmitter.h>

namespace fi3d {
class CommandParser : public QObject, public ModuleElement,
	virtual public IFeedbackEmitter 
{

	Q_OBJECT
	Q_INTERFACES(fi3d::IFeedbackEmitter)

public:
	/// @brief Constructor.
	CommandParser();

	/// @brief Destructor.
	~CommandParser();

public slots:
	/// @brief Parses the command and emits a signal based on its action.
	void parseCommand(const QString& command);

	/*! 
	 * @brief When parseCommand gets an unknown command, this function is 
	 *	called. 
	 *	
	 * This function does nothing and must be overriden by inhering classes
	 * to implement other logic on other custom-made commands.
	 */
	void handleCommand(const QString& command);

signals:
	/*!
	*	@name Implementation of the IFeedbackEmitter interface.
	*/
	/// @{
    void feedback(const QString& text) override;
    void feedbackWarning(const QString& text) override;
    void feedbackError(const QString& text) override;
    void feedbackGlobal(const QString& text) override;
    void feedbackColor(const QString& text, const Qt::GlobalColor& color) override;
	/// @}
};

/// @brief Alias for a smart pointer of this class.
using CommandParserPtr = QSharedPointer<CommandParser>;

}
