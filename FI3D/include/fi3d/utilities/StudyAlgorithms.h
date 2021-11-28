#pragma once
/*!
* @author	VelazcoJD
* @file		ModelAlgorithms.h
* @class	fi3d::ModelAlgorithms
* @brief	Static functions to manipulate Study.
*/
#include <fi3d/data/Study.h>

namespace fi3d {
class StudyAlgorithms : public QObject {
	Q_OBJECT

private:
	/// @brief Private constructor. Only static methods.
	StudyAlgorithms(QObject *parent);

public:
	/// @brief Destructor.
	~StudyAlgorithms();

	/// @brief Creates new study with series in spatial order.
	static StudyPtr fixStudySeriesOrder(StudyPtr inputStudy);

	/*!
	* @brief Set the domain in the data sets from spatial to time, if applicable.
	*
	* The data sets will changed to be on the spatial domain (instead of time
	* domain), when applicable. For instance, a singular data set may contain 
	* the same region scanned at different instances in time. If there are 10
	* data sets with 25 time slots each, this will produce 25 data sets with 
	* 10 slices. By doing so, the number of file sets and patient matrices 
	* will no longer match the number of data sets. To fix that, the file 
	* lists are empty strings and patient matrix is equal for all sets. This
	* is okay so long as we can safely assume that the patient's matrix did 
	* not change over time. To determine the spacing between each slice,
	* the patient matrices of the first 2 slices are used by calculating the
	* spacing between the point (0, 0) after it is transformed by the 
	* respective matrix.
	*/
	static StudyPtr convertStudyToSpatialDomain(StudyPtr inputStudy);

	/*!
	 * @brief This function is used to fix the Heart Data ordering.
	 *
	 * The heart data was stored in time domain and with the slices out of 
	 * order. This function changes from time to spatial domain. The function
	 * is tailored to work with the heart data only and is not generilized. 
	 * Ideally, this this function needs to break down into generalized 
	 * functions so this functionality can be used for other studies. This 
	 * function is left here as a reference for the future.
	 *
	 * TODO: Generalize function.
	 */
	static StudyPtr fixHeartStudy_DEBUG(StudyPtr heartStudy);

	/*!
	 * @brief Loads the brain data which was given as a text file.
	 * 
	 * The brain data used as part of the Holo-Neuro project was provided in 
	 * text form which needed a specific routine to load. This function is used
	 * specifically for that data set and is here in case it is needed again
	 * or may be used as reference for other cases.
	 */
	static StudyPtr loadBrainStudyFromText_DEBUG(const QString& filePath);

	/*!
	 * @brief Converts the INLINE cardiac cycle received from Jie to proper
	 * format for spatial visualization.
	 */
	static StudyPtr convertJieCardiacData_DEBUG(StudyPtr cardiacInline);
};
}
