#pragma once
/*!
*	@author		VelazcoJD
*   @file		EModuleResponse.h
*	@enum		fi3d::EModuleResponse
*	@brief		Defines the type of Module responses there can be.
*/

#include <QHash>
#include <QSharedPointer>

namespace fi3d {
class EModuleResponse {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EModuleResponse>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EModuleResponse*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EModuleResponse*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EModuleResponse* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EModuleResponse(EModuleResponse* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown module response.
		UNKNOWN = 0,
		/// @brief Add a module interaction.
		ADD_MODULE_INTERACTION = 1,
		/// @brief Updates a module interaction.
		UPDATE_MODULE_INTERACTION = 2,
		/// @brief Removes a module interaction.
		REMOVE_MODULE_INTERACTION = 3,
		/// @brief Notifies a new Visual3D was added.
		ADD_VISUAL = 4,
		/// @brief Notifies a Visual3D was refreshed.
		REFRESH_VISUAL = 5,
		/// @brief Notifies a Visual3D was removed.
		REMOVE_VISUAL = 6,
		/// @brief Notified a Visual3D was updated.
		DATA_CHANGE = 7,
		/// @brief Notifies a Visual3D visibility was changed.
		HIDE_VISUAL = 8,
		/// @brief Notifies a Visual3D was transformed.
		TRANSFORM_VISUAL = 9,
		/// @brief Notifies the parent of a Visual3D was changed.
		PARENT_CHANGE = 10,
		/// @brief Notifies the opacity of a Visual3D was changed.
		SET_VISUAL_OPACITY = 11,
		/// @brief Notifies the ImageSlice changed slice.
		SET_SLICE = 12,
		/// @brief Notifies the Model changed color.
		SET_OBJECT_COLOR = 13
	};

	/// @brief Constructs to an UNKNOWN state.
	EModuleResponse();

	/// @brief Constructs with the given state.
	EModuleResponse(const EModuleResponse& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EModuleResponse(const int& value);

	/// @brief Destructor.
	~EModuleResponse();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EModuleResponse& operator=(const int& valueType);
	EModuleResponse& operator=(const EModuleResponse& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EModuleResponse& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EModuleResponse& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}