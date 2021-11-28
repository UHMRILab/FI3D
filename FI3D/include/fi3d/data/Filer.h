#pragma once
/*!
*	@author		VelazcoJD
*   @file		Filer.h
*	@class		fi3d::Filer
*	@brief		Compilation of functions used to read/write data from/to disk.
*/

#include <fi3d/data/data_manager/EFileExtension.h>
#include <fi3d/data/ImageData.h>
#include <fi3d/data/ModelData.h>
#include <fi3d/data/Study.h>

#include <fi3d/FI3D/FI3D.h>
#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>

#include <QVector>
#include <QString>

#include <vtkSmartPointer.h>
#include <vtkStringArray.h>

namespace fi3d {
class Filer {
public:
	/*!
	 * @brief Checks whether the given directory exists, and If not, 
	 *			it attempts to create it.
	 *
	 * @param dirPath Path to the directory.
	 * @return True if the directory exists or was created, False if 
	 *			it failed to create directory.
	 */
	static bool checkAndCreateDirectory(const QString& dirPath);

	/*!
	 * @brief Reads the entire contents of a text file and returns them
	 *
	 * @param filePath Path to the file.
	 * @return The read text or empty string if file not found.
	 */
	static QString readTextFile(const QString& filePath);

	/*!
	 * @brief Reads the entire contents of a json file and returns them
	 *
	 * @param filePath Path to the file.
	 * @return The read JSON object if read successful. 
	 */
	static QJsonObject readJSONFile(const QString& filePath);

	/*!
	* @brief Read CSV file and store in to an array of entries, 
	*			each entry is an array of elements.
	*
	* @param path The path to the directory containing the DICOMs.
	* @param outEntries The vector of vectors to store the read data.
	* @param keepHeader Whether the header line should be included in the 
	*			output data. Defaults to true.
	*/
	static void readCSVFile(const QString& filePath, 
		QVector<QVector<QString>>& outEntries, const bool& keepHeader = true);

	/*!
	* @brief Reads ImageData from the given file path.
	*
	* @param filePath The path to the file.
	* @param outData Object where to store read data.
	*/
	static void readImageData(const QString& filePath, ImageDataVPtr outData);

	/*!
	* @brief Reads ImageData from the given file path.
	*
	* @param filePath The path to the file.
	* @return The ImageData read, null if no data found in file.
	*/
	static fi3d::ImageDataVPtr readImageData(const QString& filePath);

	/// @brief Reads a JPEG file into an image data object.
	static void readImageDataFromJPEG(const QString& filePath, ImageDataVPtr outData);

	/// @brief Reads a JPEG file into an image data object.
	static fi3d::ImageDataVPtr readImageDataFromJPEG(const QString& filePath);

	/// @brief Reads a JPEG file into an image data object.
	static void readImageDataFromPNG(const QString& filePath, ImageDataVPtr outData);

	/// @brief Reads a JPEG file into an image data object.
	static ImageDataVPtr readImageDataFromPNG(const QString& filePath);

	/// @brief Reads ModelData from the file given path. 
	static void readModelData(const QString& filePath, ModelDataVPtr outData);

	/// @brief Reads ModelData from the given file path.
	static ModelDataVPtr readModelData(const QString& filePath);

	/// @brief Reads ModelData from the given STL file.
	static void readModelDataFromSTL(const QString& filePath, ModelDataVPtr outData);

	/// @brief Reads ModelData from the given STL file.
	static ModelDataVPtr readModelDataFromSTL(const QString& filePath);

	/// @brief Reads ModelData from the given OBJ file.
	static void readModelDataFromOBJ(const QString& filePath, ModelDataVPtr outData);

	/// @brief Reads ModelData from the given OBJ file.
	static ModelDataVPtr readModelDataFromOBJ(const QString& filePath);

	/// @brief Reads the SeriesData from the given list of DICOM files.
	static SeriesDataVPtr readSeriesData(vtkSmartPointer<vtkStringArray> dicomFiles);

	/*!
	 * @brief Find studies within the given directory and populate their meta-data.
	 *
	 * The given study name is set if one study is found. If more than one study is found, each study gets an enumerated name
	 * as follows: "studyName"_#, where # is the number of the study as it's found in the directory.
	 */
	static QList<StudyPtr> findStudies(const QString& directoryPath, 
		QVector<QVector<vtkSmartPointer<vtkStringArray>>>& outStudiesPaths);

	/// @brief Reads the Study from DICOM files listed in the given meta-data.
	static void readStudyDataFromDICOMs(QVector<vtkSmartPointer<vtkStringArray>> paths, StudyPtr study);

	/// @brief Reads the Study from an FI3D file listed in the given meta-data.
	static void readStudyDataFromFI3DFile(const QString& path, StudyPtr study);

	/// @brief Reads meta data of the Study from a FI3D file, no data.
	static void readStudyMetaDataFromHicsFile(const QString& path, StudyPtr study);

	/// @brief Reads the study and meta-data from the given FI3D file.
	static StudyPtr readStudyFromFI3DFile(const QString& filePath);

	/// @brief Saves text to a text file.
	static void saveTextFile(const QString& text, const QString& filePath);

	/// @brief Saves the given json object in a json file, 
	/// This function does not add .json extension.
	static void saveJSONFile(const QJsonObject& jsonObject, const QString& filePath);

	/*!
	 * @brief Saves the given image data as a single JPG.
	 *
	 * This function assumes the given ImageData has a z-dimension of 1. 
	 * In other words, a simple 2D image in the XY orientation of the data set.
	 */
	static void saveImageDataAsJPG(ImageData* imageData, const QString& fileName);

	/*!
	 * @brief Saves the given image data as a set of JPGs.
	 *
	 * The given ImageData will be stored as a set of JPGs with the given 
	 * named appeneded with a _# representing the slice number. All stored in 
	 * a folder with the given name.
	 *
	 * @param imageData The imaging data to save.
	 * @param sliceOrientation The orientation of the slices.
	 * @param fileName The name of the directory and JPGs.
	 * @param filePath The path where to store the directory with the JPGs.
	 */
	static void saveImageDataAsJPGs(ImageData* imageData, 
		const ESliceOrientation& sliceOrientation, const QString& fileName, 
		const QString& filePath = "./");

	/// @brief Saves the given model data to an STL format.
	static void saveModelDataToSTL(ModelDataVPtr data, const QString& filePath);

	/// @brief Saves the study data as a FI3D file.
	static void saveStudyAsFI3DFile(const StudyPtr study, const QString& filePath = FI3D_DATA_PATH);

private:
	/// @brief Given the file path. Determine the extension of the file.
	static EFileExtension getFileExtension(const QString& filePath);

	/*!
	 * @brief Helper function to extract series data from its JSON format.
	 *
	 * @param seriesJSON The array with the series data in JSON format.
	 * @param study	The study to insert the converted Series.
	 */
	static void convertSeriesDataFromJSON(QJsonArray& seriesJSONArray, StudyPtr study);

	/// Dialog functions
public:
	/*! 
	* @brief Pops up the filer explore dialog to search for a directory.
	* 
	* @param path The path to start the dialog in, default to FI3D's data.
	* @return The directory path chosen by the user.
	*/
	static QString getDirectoryPathDialog(const QString& path = FI3D::DATA_DIRECTORY);

	/*!
	 * @brief Pops up the file explorer dialog to search for a file with the 
	 *			given extension.
	 *
	 * Use '*' as extension for any file. Similar as an 'open' dialog.
	 * 
	 * @param extension The type of file the user can select.
	 * @param path The path to start the dialog in, default to FI3D's data.
	 * @return The path to the selected file.
	 */
	static QString getFilePathDialog(const QString& extension, const QString& path = FI3D::DATA_DIRECTORY);

	/*!
	 * @brief Like getFilePathDialog but lets the user specify the name of 
	 *			the file.
	 *
	 * Similar to a 'save as...' dialog.
	 * 
	 * @param extension The type of file the user can select.
	 * @param path The path to start the dialog in, default to FI3D's data.
	 * @return The path to the selected file, with its name.
	 */
	static QString getFilveSavePathDialog(const QString& extension, const QString& path = FI3D::DATA_DIRECTORY);

};
}
