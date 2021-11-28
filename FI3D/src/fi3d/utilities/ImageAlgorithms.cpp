#include "fi3d/utilities/ImageAlgorithms.h"

#include <fi3d/logger/Logger.h>

#include <fi3d/data/Filer.h>

#include <vtkImageThreshold.h>
#include <vtkFlyingEdges2D.h>
#include <vtkPointData.h>

using namespace fi3d;

void ImageAlgorithms::applyThreshold(const double& min, const double& max, 
	vtkSmartPointer<vtkImageData> source, vtkSmartPointer<vtkImageData> output) 
{
	vtkNew<vtkImageThreshold> threshold;
	threshold->ThresholdBetween(min, max);
	threshold->SetInValue(255);
	threshold->SetOutValue(0);
	threshold->SetInputData(source);
	threshold->SetOutputScalarTypeToUnsignedChar();
	threshold->SetOutput(output);
	threshold->Update();
}

ImageDataVPtr ImageAlgorithms::applyThreshold(const double& min, const double& max, 
	vtkSmartPointer<vtkImageData> source) 
{
	ImageDataVPtr output = ImageDataVPtr::New();
	ImageAlgorithms::applyThreshold(min, max, source, output);
	return output;
}

ModelDataVPtr ImageAlgorithms::computeEdges(vtkSmartPointer<vtkImageData> image) {
	ModelDataVPtr segmentation = ModelDataVPtr::New();
	vtkNew<vtkFlyingEdges2D> edgeFinder;
	edgeFinder->SetInputData(image);
	edgeFinder->SetValue(0, 255);
	edgeFinder->SetOutput(segmentation);
	edgeFinder->Update();

	if (segmentation->GetPointData() != Q_NULLPTR) {
		for (int i = 0; i < segmentation->GetPointData()->GetNumberOfArrays(); i++) {
			segmentation->GetPointData()->RemoveArray(i);
		}
	}

	return segmentation;
}

ImageDataVPtr ImageAlgorithms::loadGeologyData(const QString& dirPath) {
	ImageDataVPtr data = ImageDataVPtr::New();
	data->SetDimensions(236, 210, 13);
	data->SetSpacing(0.1, 0.01, 0.05);
	data->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	double max = 4000;
	double min = -1000;

	for (int i = 0; i < 13; i++) {
		QString fileNumber;
		if (i < 10) {
			fileNumber = QObject::tr("0%1").arg(i);
		} else {
			fileNumber = QObject::tr("%1").arg(i);
		}

		QString filePath = QObject::tr("%1/Line%2.csv").arg(dirPath).arg(fileNumber);

		QVector<QVector<QString>> entries;
		Filer::readCSVFile(filePath, entries, true);

		for (int y = 0; y < entries.count(); y++) {
			for (int x = 0; x < entries[y].count(); x++) {
				double value = entries[y][x].toDouble();
				value = (value - min) / (max - min);
				value = value * 256;
				if (value < 0) {
					value = 0;
				} else if (value > 255) {
					value = 255;
				}

				data->SetScalarComponentFromDouble(x, y, i, 0, value);
			}
		}
	}

	return data;
}
