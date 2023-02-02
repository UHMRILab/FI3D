#include <fi3d/utilities/StudyAlgorithms.h>

#include <fi3d/logger/Logger.h>

#include <QtMath>
#include <QFile>

#include <vtkTransform.h>
#include <vtkMath.h>

using namespace fi3d;

StudyAlgorithms::StudyAlgorithms(QObject *parent)
	: QObject(parent)
{}

StudyAlgorithms::~StudyAlgorithms() {}

StudyPtr StudyAlgorithms::fixStudySeriesOrder(StudyPtr inputStudy) {
	qWarning() << "Method has not yet been implemented";
	return StudyPtr();

	//TODO: Old code used within a study. Was moved here as it makes more sense.
	// This code needs to be modified.
	//if (mIsInOrder) {
	//	return;
	//}

	//QVector<int> indexOrder;
	//QMap<int, double> matrixDistances;

	//// find distance of point 1, 1, 1 from 0 after it is transformed from each matrix
	//for (int i = 0; i < mSeriesSet.size(); i++) {
	//	double p[4] = {0, 0, 0, 1};
	//	//mSeriesSet.at(i)->getPatientMatrix()->MultiplyPoint(p, p);
	//	double distance = p[0] * p[0] + p[1] * p[1] + p[2] * p[2];
	//	matrixDistances.insert(i, distance);
	//}

	//// Order the indices by the distances obtained on previous loop
	//for (int i = 0; i < mSeriesSet.size(); i++) {
	//	QMap<int, double>::iterator it = matrixDistances.begin();
	//	double minDist = it.value();
	//	int minIndex = it.key();
	//	it++;
	//	for (; it != matrixDistances.end(); it++) {
	//		if (minDist > it.value()) {
	//			minDist = it.value();
	//			minIndex = it.key();
	//		}
	//	}
	//	matrixDistances.remove(minIndex);
	//	indexOrder.push_back(minIndex);
	//}

	//// Assign the saved elements in the right order
	//QVector<vtkSmartPointer<ImageData>> orderedData;
	//QVector<QVector<QString>> orderedFileSets;
	//QVector<vtkSmartPointer<vtkMatrix4x4>> orderedPatientMatrices;
	//for (int i = 0; i < indexOrder.size(); i++) {
	//	orderedData.push_back(mSeriesSet.at(indexOrder[i]));
	//	//orderedFileSets.push_back(mDataFiles.at(indexOrder[i]));
	//	//orderedPatientMatrices.push_back(mDataPatientMatrices.at(indexOrder[i]));
	//}

	//mSeriesSet = orderedData;
	////mDataFiles = orderedFileSets;
	////mDataPatientMatrices = orderedPatientMatrices;
	//mIsInOrder = true;
}

StudyPtr StudyAlgorithms::convertStudyToSpatialDomain(StudyPtr inputStudy) {
	qWarning() << "Method has not yet been implemented";
	return StudyPtr();

	//TODO: Old code used within a study. Was moved here as it makes more sense. This code needs to be modified.
	//return;

	//if (mIsSpatialDomain) return;
	//this->fixSeriesOrder();

	//QVector<vtkSmartPointer<ImageData>> newDataSets;
	//QVector<vtkSmartPointer<vtkMatrix4x4>> newPatientMatrices;
	//QVector<QVector<QString>> newFileSets;

	//// find how many data sets we will need
	//int newDataSetCount = mSeriesSet.at(0)->GetDimensions()[2];		// 3rd dimension is the time dimension

	//																// Store pointer to the patient matrix that will be used for all new data sets
	//																//vtkSmartPointer<vtkMatrix4x4> patientMatrix = mDataPatientMatrices.at(0);

	//																// find the new dimensions of each data set
	//int newDimension[3];
	//mSeriesSet.at(0)->GetDimensions(newDimension);
	//newDimension[2] = mSeriesSet.count();

	//// Find the new origin of the data sets
	//double newOrigin[3];
	//mSeriesSet.at(0)->GetOrigin(newOrigin);

	//// find the new spacings between each slice on all three orientations
	//double newSpacings[3];
	//mSeriesSet.at(0)->GetSpacing(newSpacings);
	//double p1[4] = {0, 0, 0, 1};
	//double p2[4] = {0, 0, 0, 1};
	////mDataPatientMatrices.at(0)->MultiplyPoint(p1, p1);
	////mDataPatientMatrices.at(1)->MultiplyPoint(p2, p2);
	//double dist = qSqrt(qPow(p2[0] - p1[0], 2) + qPow(p2[1] - p1[1], 2) + qPow(p2[2] - p1[2], 2));
	//newSpacings[2] = dist;

	//// Keep the count of scalar components in the image data
	//int numberOfScalarComponents = mSeriesSet.at(0)->GetNumberOfScalarComponents();

	//// Create the new data sets
	//for (int i = 0; i < newDataSetCount; i++) {
	//	vtkSmartPointer<ImageData> im = vtkSmartPointer<ImageData>::New();
	//	im->SetDimensions(newDimension);
	//	im->SetSpacing(newSpacings);
	//	im->SetOrigin(newOrigin);
	//	im->AllocateScalars(mSeriesSet.at(0)->GetScalarType(), numberOfScalarComponents);

	//	for (int j = 0; j < mSeriesSet.count(); j++) {
	//		ImageData* data = mSeriesSet.at(j);
	//		for (int x = 0; x < newDimension[0]; x++) {
	//			for (int y = 0; y < newDimension[1]; y++) {
	//				for (int k = 0; k < numberOfScalarComponents; k++) {
	//					im->SetScalarComponentFromDouble(x, y, j, k, data->GetScalarComponentAsDouble(x, y, i, k));
	//				}
	//			}
	//		}
	//	}

	//	newDataSets.push_back(im);
	//	//newPatientMatrices.push_back(patientMatrix);
	//	newFileSets.push_back(QVector<QString>());
	//}

	//mSeriesSet = newDataSets;
	////mDataPatientMatrices = newPatientMatrices;
	////mDataFiles = newFileSets;
	//mIsSpatialDomain = true;
}

StudyPtr StudyAlgorithms::fixHeartStudy_DEBUG(StudyPtr heartStudy) {
    // Find the order for which the series should be in
    QVector<int> orderIndices;
    QVector<double> distances;
    double origin[3] = {0.0, 0.0, 0.0};
    for (int i = 0; i < heartStudy->getSeriesCount(); i++) {
        double imageOrigin[3] = {0, 0, 0};
        vtkNew<vtkTransform> imageTransform;
        imageTransform->SetMatrix(heartStudy->getSeries(i)->getPatientMatrix());
        imageTransform->TransformPoint(imageOrigin, imageOrigin);

        double distance = vtkMath::Distance2BetweenPoints(origin, imageOrigin);
        bool inserted = false;
        for (int d = 0; d < distances.count(); d++) {
            if (distance < distances[d]) {
                distances.insert(d, distance);
                orderIndices.insert(d, i);
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            distances.append(distance);
            orderIndices.append(i);
        }
    }

    // Put them in the found order
    StudyPtr orderedStudy(new Study(heartStudy->getStudyID()));
    for (int i = 0; i < heartStudy->getSeriesCount(); i++) {
        orderedStudy->addSeries(heartStudy->getSeries(orderIndices[i]));
    }

    /* Swap from Time domain to Space domain */
	QString newStudyID = QObject::tr("%1_fixed").arg(heartStudy->getStudyID());
    StudyPtr swappedStudy(new Study(newStudyID));
    swappedStudy->setPatientID(heartStudy->getPatientID());
    swappedStudy->setPatientName(heartStudy->getPatientName());
    swappedStudy->setStudyDate(heartStudy->getStudyDate());
    swappedStudy->setStudyTime(heartStudy->getStudyTime());

    // Get the dimensions of the series
    int* dims = orderedStudy->getSeries(0)->GetDimensions();
    double* spacing = orderedStudy->getSeries(0)->GetSpacing();

    // Compute the spacing between each slice in spatial domain
    double o1[3] = {0, 0, 0};
    double o2[3] = {0, 0, 0};

    vtkNew<vtkTransform> t1, t2;
    t1->SetMatrix(orderedStudy->getSeries(0)->getPatientMatrix());
    t2->SetMatrix(orderedStudy->getSeries(1)->getPatientMatrix());

    t1->TransformPoint(o1, o1);
    t2->TransformPoint(o2, o2);

    double spacingZ = vtkMath::Distance2BetweenPoints(o1, o2);
    spacingZ = qSqrt(spacingZ);

    // One data set should be created per time frame. The time dimension is the 3rd dimension.
    for (int i = 0; i < dims[2]; i++) {
        SeriesDataVPtr series = SeriesDataVPtr::New();
        series->SetOrigin(0, 0, 0);
        series->SetDimensions(dims[0], dims[1], orderedStudy->getSeriesCount());
        series->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
        series->SetSpacing(spacing[0], spacing[1], spacingZ);

        for (int z = 0; z < orderedStudy->getSeriesCount(); z++) {
            vtkSmartPointer<SeriesData> timeData = orderedStudy->getSeries(z);
            for (int y = 0; y < dims[1]; y++) {
                for (int x = 0; x < dims[0]; x++) {
                    double value = timeData->GetScalarComponentAsDouble(x, y, i, 0);
                    series->SetScalarComponentFromDouble(x, y, z, 0, value);
                }
            }
        }
        series->setPatientMatrix(orderedStudy->getSeries(0)->getPatientMatrix());
        swappedStudy->addSeries(series);
    }

    return swappedStudy;
}

StudyPtr StudyAlgorithms::loadBrainStudyFromText_DEBUG(const QString & filePath) {
	SeriesDataVPtr brainData = SeriesDataVPtr::New();

	double origin[3] = {0.0, 0.0, 0.0};
	double spacing[3] = {1.0, 1.0, 1.0};
	int dimensions[3] = {144, 206, 166};

	brainData->SetOrigin(origin);
	brainData->SetSpacing(spacing);
	brainData->SetDimensions(dimensions);
	brainData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	int min = 0;
	int max = 602;
	float ratio = 255.0 / 602.0;

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Failed to read data file.";
		return Q_NULLPTR;
	}

	while (!file.atEnd()) {
		QString line = file.readLine();
		line = line.trimmed();
		QList<QString> values = line.split(",");

		if (values.count() != 4) {
			continue;
		}

		int x = values[0].toInt();
		int y = values[1].toInt();
		int z = values[2].toInt();

		int intensity = values[3].toInt();
		unsigned char i = intensity;

		unsigned char* val = static_cast<unsigned char*>(brainData->GetScalarPointer(x, y, z));
		val[0] = i;
	}
	file.close();

	StudyPtr study(new Study("Brain"));
	study->addSeries(brainData);
	
	return study;
}

StudyPtr StudyAlgorithms::convertJieCardiacData_DEBUG(StudyPtr cardiacInline) {
	SeriesDataVPtr cycle = cardiacInline->getSeries(0);
	int* dims = cycle->GetDimensions();
	double* spac = cycle->GetSpacing();

	QString studyID = tr("%1_fixed").arg(cardiacInline->getStudyID());
	StudyPtr study(new Study(studyID));
	study->setPatientID(cardiacInline->getPatientID());
	study->setPatientName(cardiacInline->getPatientName());
	study->setStudyDate(cardiacInline->getStudyDate());
	study->setStudyTime(cardiacInline->getStudyTime());

	for (int i = 0; i < 25; i++) {
		SeriesDataVPtr series = study->createAndAddSeries();
		series->SetDimensions(dims[0], dims[2], 9);
		// TODO: This many not be the right spacing for the z-dimension.
		series->SetSpacing(spac[0], spac[1], spac[2] * 25);
		series->AllocateScalars(cycle->GetScalarType(), 1);

		for (int x = 0; x < dims[0]; x++) {
			for (int y = 0; y < dims[1]; y++) {
				for (int z = 0; z < 9; z++) {
					double value = cycle->GetScalarComponentAsDouble(x, y, 25 * z + i, 0);
					series->SetScalarComponentFromDouble(x, y, z, 0, value);
				}
			}
		}
	}

	return study;
}
