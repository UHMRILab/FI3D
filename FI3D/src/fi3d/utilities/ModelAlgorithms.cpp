#include <fi3d/utilities/ModelAlgorithms.h>

#include <fi3d/logger/Logger.h>

#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkClipPolyData.h>
#include <vtkContourFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkKochanekSpline.h>
#include <vtkNew.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkPointData.h>
#include <vtkReverseSense.h>
#include <vtkStripper.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkTransformPolyDataFilter.h>

using namespace fi3d;

void ModelAlgorithms::combineModelData(vtkSmartPointer<vtkPolyData> firstData, 
	vtkSmartPointer<vtkPolyData> secondData,
	vtkSmartPointer<vtkPolyData> outData)
{
	if (firstData.Get() == Q_NULLPTR || secondData.Get() == Q_NULLPTR || outData.Get() == Q_NULLPTR) {
		return;
	}

	vtkNew<vtkAppendPolyData> appender;
	appender->AddInputData(firstData);
	appender->AddInputData(secondData);

	vtkNew<vtkCleanPolyData> cleaner;
	cleaner->SetInputConnection(appender->GetOutputPort());
	cleaner->SetOutput(outData);
	cleaner->Update();
}

ModelDataVPtr ModelAlgorithms::combineModelData(
	vtkSmartPointer<vtkPolyData> firstData,
	vtkSmartPointer<vtkPolyData> secondData)
{
	ModelDataVPtr modelData = ModelDataVPtr::New();
	ModelAlgorithms::combineModelData(firstData, secondData, modelData);
	return modelData;
}

void ModelAlgorithms::combineModelData(
	QVector<vtkSmartPointer<vtkPolyData>> modelDatas, 
	vtkSmartPointer<vtkPolyData> outData) 
{
	if (outData.Get() == Q_NULLPTR) {
		return;
	}
	
	vtkNew<vtkAppendPolyData> appender;
	for (int i = 0; i < modelDatas.count(); i++) {
		appender->AddInputData(modelDatas[i]);
	}

	// Remove duplicated points
	vtkNew<vtkCleanPolyData> cleaner;
	cleaner->SetInputConnection(appender->GetOutputPort());
	cleaner->SetOutput(outData);
	cleaner->Update();
}

ModelDataVPtr ModelAlgorithms::combineModelData(QVector<vtkSmartPointer<vtkPolyData>> modelDatas) {
	ModelDataVPtr modelData = ModelDataVPtr::New();
	ModelAlgorithms::combineModelData(modelDatas, modelData);
	return modelData;
}

ModelDataVPtr ModelAlgorithms::capClip(vtkSmartPointer<vtkPolyData> modelData, vtkSmartPointer<vtkPlane> cutPlane) {
	if (modelData.Get() == Q_NULLPTR || cutPlane.Get() == Q_NULLPTR) {
		return Q_NULLPTR;
	}

	vtkNew<vtkClipPolyData> clipper;
	clipper->SetInputData(modelData);
	clipper->SetClipFunction(cutPlane);
	clipper->SetValue(0);

	// Now extract feature edges
	vtkNew<vtkFeatureEdges> boundaryEdges;
	boundaryEdges->SetInputConnection(clipper->GetOutputPort());
	boundaryEdges->BoundaryEdgesOn();
	boundaryEdges->FeatureEdgesOff();
	boundaryEdges->NonManifoldEdgesOff();
	boundaryEdges->ManifoldEdgesOff();

	vtkNew<vtkStripper> boundaryStrips;
	boundaryStrips->SetInputConnection(boundaryEdges->GetOutputPort());
	boundaryStrips->Update();

	// Change the polylines into polygons
	ModelDataVPtr cap = ModelDataVPtr::New();
	cap->SetPoints(boundaryStrips->GetOutput()->GetPoints());
	cap->SetPolys(boundaryStrips->GetOutput()->GetLines());

	return cap;
}

ModelDataVPtr ModelAlgorithms::clipModel(vtkSmartPointer<vtkPolyData> modelData, vtkSmartPointer<vtkPlane> cutPlane) {
	if (modelData.Get() == Q_NULLPTR || cutPlane.Get() == Q_NULLPTR) {
		return Q_NULLPTR;
	}

	ModelDataVPtr clippedModel = ModelDataVPtr::New();

	vtkNew<vtkClipPolyData> clipper;
	clipper->SetInputData(modelData);
	clipper->SetClipFunction(cutPlane);
	clipper->SetValue(0);
	clipper->SetOutput(clippedModel);
	clipper->Update();

	return clippedModel;
}

ImageDataVPtr ModelAlgorithms::polyToImage(
	vtkSmartPointer<vtkPolyData> modelData,
	const double & vsX,	const double & vsY, const double & vsZ) 
{
	qDebug() << "Enter - Converting ModelData to ImageData with spacings: (" << vsX << "," << vsY << "," << vsZ << ")";
	double spacing[3] = {vsX, vsY, vsZ};
	ImageDataVPtr outData = ImageDataVPtr::New();
	outData->SetSpacing(spacing);

	double bounds[6];
	modelData->GetBounds(bounds);
	qDebug() << "Setting bounds: [" << bounds[0] << ", " << bounds[1] << 
		"], [" << bounds[2] << "," << bounds[3] << "], [" <<
		bounds[4] << ", " << bounds[5] << "]";

	int dimensions[3];
	for (int i = 0; i < 3; i++) {
		double value = (bounds[i * 2 + 1] - bounds[i * 2]) / spacing[i];
		dimensions[i] = static_cast<int>(ceil(value));
	}
	outData->SetDimensions(dimensions);
	qDebug() << "Setting dimensions: (" << dimensions[0] << ", " << dimensions[1] << ", " << dimensions[2] << ")";

	double origin[3];
	for (int i = 0; i < 3; i++) {
		origin[i] = bounds[i * 2] + spacing[i] / 2;
	}
	outData->SetOrigin(origin);
	qDebug() << "Setting origin: (" << origin[0] << "," << origin[1] << ", " << origin[2] << ")";

	outData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
	qDebug() << "Setting default image data to 0 for" << dimensions[0] * dimensions[1] * dimensions[2] << "points";
	// Set default data values to 0
	for (int x = 0; x < dimensions[0]; x++) {
		for (int y = 0; y < dimensions[1]; y++) {
			for (int z = 0; z < dimensions[2]; z++) {
				outData->SetScalarComponentFromDouble(x, y, z, 0, 0);
			}
		}
	}
	qDebug() << "Done setting default data to 0";

	qDebug() << "Setting model data to image data for" << modelData->GetNumberOfPoints() << "points";

	// for every point in the ModelData. Set 255 to the nearest voxel
	for (vtkIdType i = 0; i < modelData->GetNumberOfPoints(); i++) {
		double p[3];
		modelData->GetPoint(i, p);

		for (int i = 0; i < 3; i++) {
			p[i] = (p[i] - origin[i]) / spacing[i];
		}
		outData->SetScalarComponentFromDouble(p[0], p[1], p[2], 0, 255);
	}
	qDebug() << "Done setting model data to image data";

	qDebug() << "Exit";
	return outData;
}

ModelDataVPtr ModelAlgorithms::transform(
	vtkSmartPointer<vtkPolyData> modelData,
	vtkSmartPointer<vtkMatrix4x4> transformMatrix) 
{
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->SetMatrix(transformMatrix);
	return ModelAlgorithms::transform(modelData, transform);
}

ModelDataVPtr ModelAlgorithms::transform(
	vtkSmartPointer<vtkPolyData> modelData,
	vtkSmartPointer<vtkTransform> transform) 
{
	if (modelData.Get() == Q_NULLPTR || transform.Get() == Q_NULLPTR) {
		return Q_NULLPTR;
	}
	ModelDataVPtr outData = ModelDataVPtr::New();

	vtkNew<vtkTransformPolyDataFilter> transformFilter;
	transformFilter->SetInputData(modelData);
	transformFilter->SetTransform(transform);
	transformFilter->SetOutput(outData);
	transformFilter->Update();

	return outData;
}

ModelDataVPtr ModelAlgorithms::splineInterpolate(
	vtkSmartPointer<vtkPoints> points, 
	const double& resU, const double& resV, const double& resW, 
	const bool& closedSplined)
{
	ModelDataVPtr interpolatedData = ModelDataVPtr::New();
	
	ModelAlgorithms::splineInterpolate(points, resU, resV, resW, closedSplined, interpolatedData);

	return interpolatedData;
}

void ModelAlgorithms::splineInterpolate(vtkSmartPointer<vtkPoints> points, 
	const double & resU, const double & resV, const double & resW, 
	const bool & closedSplined, vtkSmartPointer<vtkPolyData> outData) 
{
	if (outData.Get() == Q_NULLPTR || points.Get() == Q_NULLPTR) {
		return;
	}

	vtkNew<vtkKochanekSpline> xSpline, ySpline, zSpline;
	vtkNew<vtkParametricSpline> spline;
	spline->SetXSpline(xSpline);
	spline->SetYSpline(ySpline);
	spline->SetZSpline(zSpline);
	if (closedSplined) {
		spline->ClosedOn();
	} else {
		spline->ClosedOff();
	}
	spline->SetPoints(points);

	vtkNew<vtkParametricFunctionSource> functionSource;
	functionSource->SetParametricFunction(spline);
	functionSource->SetUResolution(resU);
	functionSource->SetVResolution(resV);
	functionSource->SetWResolution(resW);
	functionSource->SetOutput(outData);
	functionSource->Update();

}

ModelDataVPtr ModelAlgorithms::reconstructSurface(vtkSmartPointer<vtkPolyData> pointCloud) {
	ModelDataVPtr surface = ModelDataVPtr::New();
	
	if (pointCloud.Get() == Q_NULLPTR) {
		return surface;
	}

	qDebug() << "Number of points: " << pointCloud->GetNumberOfPoints();

	vtkNew<vtkSurfaceReconstructionFilter> surfaceFilter;
	surfaceFilter->SetInputData(pointCloud);
	surfaceFilter->Update();

	vtkNew<vtkContourFilter> contourFilter;
	contourFilter->SetInputConnection(surfaceFilter->GetOutputPort());
	contourFilter->SetValue(0, 0.0);
	contourFilter->SetOutput(surface);
	contourFilter->Update();

	//vtkNew<vtkReverseSense> reverse;
	//reverse->SetInputConnection(contourFilter->GetOutputPort());
	//reverse->ReverseCellsOn();
	//reverse->ReverseNormalsOn();
	//reverse->SetOutput(surface);
	//reverse->Update();

	// TODO: The surface reconstruction creates these 2 data arrays, 
	// array 1 makes the model red, array 0 I have no idea what it does (the 
	// array has no name either). I removed them because I don't want it to be 
	// red, and because I saw no use in the rendering window for array 0. 
	// Sometimes, this function throws an error.
	if (surface.GetPointer() != Q_NULLPTR && surface->GetPointData() != Q_NULLPTR) {
		for (int i = 0; i < surface->GetPointData()->GetNumberOfArrays(); i++) {
			surface->GetPointData()->RemoveArray(i);
		}
	}

	return surface;
}
