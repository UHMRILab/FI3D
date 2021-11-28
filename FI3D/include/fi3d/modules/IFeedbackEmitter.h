#pragma once
/*!
* @author	VelazcoJD
* @file		IFeedbackEmitter.h
* @class	fi3d::IFeedbackEmitter
* @brief	Interface for a module object that wishes to provide user 
*			feedback.
*
* Each feedback type has its own color: green, orange, red, blue for info,
* warning, error, and global, respectively. Custom feedback is available which
* can be used to provide the desired color for the feedback message.
*/

#include <QObject>

namespace fi3d {
class IFeedbackEmitter {
signals:
	/// @brief Emitted when regular feedback is provided.
	virtual void feedback(const QString& text) = 0;

	/// @brief Emitted when warning feedback is provided.
	virtual void feedbackWarning(const QString& text) = 0;

	/// @brief Emitted when error feedback is provided.
	virtual void feedbackError(const QString& text) = 0;

	/*! 
	 * @brief Emitted when global feedback is provided. 
	 *
	 * Global feedback is displayed in every module's tab. The module's ID is
	 * is shown with the feedback.
	 */
	virtual void feedbackGlobal(const QString& text) = 0;

	/// @brief Emitted when custom color feedback is provided.
	virtual void feedbackColor(const QString& text, const Qt::GlobalColor& color) = 0;

public:
	/// @brief Connects the feedback of this object to the given emitter.
	void addFeedbackPropagator(IFeedbackEmitter* propagator) {
		QObject* emi = dynamic_cast<QObject*>(this);
		QObject* pro = dynamic_cast<QObject*>(propagator);

		QObject::connect(
			emi, SIGNAL(feedback(const QString&)),
			pro, SIGNAL(feedback(const QString&)),
			Qt::UniqueConnection);
		QObject::connect(
			emi, SIGNAL(feedbackWarning(const QString&)),
			pro, SIGNAL(feedbackWarning(const QString&)),
			Qt::UniqueConnection);
		QObject::connect(
			emi, SIGNAL(feedbackError(const QString&)),
			pro, SIGNAL(feedbackError(const QString&)),
			Qt::UniqueConnection);
		QObject::connect(
			emi, SIGNAL(feedbackGlobal(const QString&)),
			pro, SIGNAL(feedbackGlobal(const QString&)),
			Qt::UniqueConnection);
		QObject::connect(
			emi, SIGNAL(feedbackColor(const QString&, const Qt::GlobalColor&)),
			pro, SIGNAL(feedbackColor(const QString&, const Qt::GlobalColor&)),
			Qt::UniqueConnection);
	}

	/// @brief Disconnects the feedback of this object from the given emitter.
	void removeFeedbackPropagator(IFeedbackEmitter* propagator) {
		QObject* emi = dynamic_cast<QObject*>(this);
		QObject* pro = dynamic_cast<QObject*>(propagator);

		QObject::disconnect(
			emi, SIGNAL(feedback(const QString&)),
			pro, SIGNAL(feedback(const QString&)));
		QObject::disconnect(
			emi, SIGNAL(feedbackWarning(const QString&)),
			pro, SIGNAL(feedbackWarning(const QString&)));
		QObject::disconnect(
			emi, SIGNAL(feedbackError(const QString&)),
			pro, SIGNAL(feedbackError(const QString&)));
		QObject::disconnect(
			emi, SIGNAL(feedbackGlobal(const QString&)),
			pro, SIGNAL(feedbackGlobal(const QString&)));
		QObject::disconnect(
			emi, SIGNAL(feedbackColor(const QString&, const Qt::GlobalColor&)),
			pro, SIGNAL(feedbackColor(const QString&, const Qt::GlobalColor&)));
	}
};

/// @brief Alias for a smart pointer of this class.
using IFeedbackEmitterPtr = QSharedPointer<IFeedbackEmitter>;

}

Q_DECLARE_INTERFACE(fi3d::IFeedbackEmitter,	"fi3d.IFeedbackEmitter");