#pragma once
/*!
* @author	VelazcoJD
* @file		Promise.h
* @class	fi3d::Promise
* @brief	This Promise works in a similar fashion to JavaScript promises with
*			less capabilities.
*
* This promise cannot run functions on success or fail (although this could be
* a feature to implement in the future). Instead, a signal is emmitted which
* can be used by other objects to connect to when on success and when on
* failure.
*/

#include <QObject>
#include <QString>

namespace fi3d {

/*!
* @brief Helper class for a Promise, defining its signals and slots.
*
* These signal/slot members have to be defined in a separate object since a
* Promise is a template and templates cannot inherit from QObject to use
* signals and slots.
*/
class PromiseSignalsSlots : public QObject {

	Q_OBJECT

signals:
	/// @brief Signal indicating the promise has resolved.
	void resolved();

	/// @brief Signal indicating the promise has failed.
	void failed();

public:
	/// @brief Constructor.
	PromiseSignalsSlots() {}

	/// @brief Destructor.
	~PromiseSignalsSlots() {}
};

/// @brief The actual Promise class.
template <typename T>
class Promise : public PromiseSignalsSlots {
private:
	/// @brief The error message stating why the promise failed.
	QString mErrorMessage;

	/*! 
	 * @brief The result of the promise once is resolved. 
	 * 
	 * Getting the result before resolution yields a default value. 
	 */
	T mResult;

	/// @brief Flag used to indicate whether this promise has been resolved.
	bool mIsResolved;

public:
	/// @brief Constructor.
	Promise() : mErrorMessage(""), mIsResolved(false) {}

	/// @brief Destructor.
	~Promise() {}

	/// @brief Resets the promise.
	void reset() {
		mErrorMessage = "";
		mIsResolved = false;
	}

	/// @brief Whether the promise has been resolved.
	bool isResolved() {
		return mIsResolved;
	}

	/// @brief Resolves the promise with the given result.
	void resolve(T result) {
		mResult = result;
		emit resolved();
		mIsResolved = true;
	}

	/// @brief Fails the promise with the given error message.
	void fail(QString& errorMessage) {
		mErrorMessage = errorMessage;
		emit failed();
	}

	/// @brief Gets the result applied when the promise is resolved.
	T getResult() {
		return mResult;
	}

	/// @brief Gets the error message given when the promise was failed.
	QString getErrorMessage() {
		return mErrorMessage;
	}
};
}