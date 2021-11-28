#pragma once
/*!
* @author	VelazcoJD
* @file		EVisual.h
* @enum		fi3d::EVisual
* @brief	Defines the type of Visual that may appear in a scene.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class EVisual {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EVisual>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EVisual*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EVisual*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EVisual* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EVisual(EVisual* type);

public:
     /// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown visual type.
		UNKNOWN = 0,
		/// @brief Slice of 3D Image data. 
		IMAGE_SLICE = 1,
		/// @brief A slice from a study.
		STUDY_IMAGE_SLICE = 2,
		/// @brief A Polygon mesh.
		MODEL = 3,
		/// @brief Composition of many models.
		ASSEMBLY = 4,
		/// @brief A 2D text, a label.
		SUBTITLE = 5,
		/// @brief Animated study slice
		ANIMATED_STUDY_SLICE = 6,
		/// @brief Animated model
		ANIMATED_MODEL = 7
	};

	/// @brief Constructs to an UNKNOWN state.
	EVisual();

	/// @brief Constructs with the given state.
	EVisual(const EVisual& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EVisual(const int& value);

	/// @brief Destructor.
	~EVisual();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EVisual& operator=(const int& valueType);
	EVisual& operator=(const EVisual& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EVisual& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EVisual& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;

	/*!
	 * @brief Whether the Visual is a slice.
	 *
	 * True if it is an IMAGE_SLICE, STUDY_IMAGE_SLICE, or
	 * ANIMATED_STUDY_SLICE.
	 */
	bool isSlice();

	/*!
	 * @brief Whether the Visual is a study lisce.
	 *
	 * True if it is a STUDY_IMAGE_SLICE or ANIMATED_STUDY_SLICE.
	 */
	bool isStudySlice();

	/*!
	 * @brief Whether the Visual is a model.
	 *
	 * True if it is a MODEL or ANIMATED_MODEL.
	 */
	bool isModel();
};
}
