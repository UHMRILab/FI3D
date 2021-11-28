#pragma once
/*!
*	@author		VelazcoJD
*   @file		EFileExtension.h
*	@enum		fi3d::EFileExtension
*	@brief		Enumeration for the type of file extensions.
*/

#include <QHash>
#include <QSharedPointer>

namespace fi3d {
class EFileExtension {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EFileExtension>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EFileExtension*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EFileExtension*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EFileExtension* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EFileExtension(EFileExtension* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown file type.
		UNKNOWN = 0,
		/// @brief A FI3D file.
		FI3D = 1,
		/// @brief JPG/JPEG Image.
		JPEG = 2,
		/// @brief JPG/JPEG Image.
		JPG = 3,
		/// @brief PNG image.
		PNG = 4,
		/// @brief STL Model
		STL = 5,
		/// @brief OBJ Model
		OBJ = 6,
		/// @brief PLY Model
		PLY = 7,
		/// @brief JSON file
		JSON = 8
	};

	/// @brief Constructs to an UNKNOWN state.
	EFileExtension();

	/// @brief Constructs with the given state.
	EFileExtension(const EFileExtension& mtype);

	/// @brief Constructs with the given string.
	EFileExtension(const QString& value);

	/// @brief Constructs with the given state. Defined by its value.
	EFileExtension(const int& value);

	/// @brief Destructor.
	~EFileExtension();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EFileExtension& operator=(const int& valueType);
	EFileExtension& operator=(const QString& valueType);
	EFileExtension& operator=(const EFileExtension& other);
	bool operator==(const int& valueType) const;
	bool operator==(const QString& valueType) const;
	bool operator==(const EFileExtension& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const QString& valueType) const;
	bool operator!=(const EFileExtension& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
	
	/// @brief Whether the file is a HCIS file.
	bool isFI3DFile();

	/// @brief Whether the file is an image.
	bool isImage();

	/// @brief Whether the file is a JPG/JPEG image.
	bool isJPGImage();

	/// @brief Whether the file is a model.
	bool isModel();
};
}
