#include <fi3d/data/ModelData.h>

#include <vtkCylinderSource.h>
#include <vtkLineSource.h>
#include <vtkObjectFactory.h>
#include <vtkSphereSource.h>
#include <vtkTriangleFilter.h>
#include <vtkTubeFilter.h>

using namespace fi3d;

vtkStandardNewMacro(ModelData)

ModelData::ModelData()
	: DataObject()
{
}

ModelData::~ModelData() {}

EData ModelData::getDataType() const {
	return EData::MODEL;
}

ModelDataVPtr ModelData::createCylinder(const double& radius, const double& height, const int& resolution) {
	ModelDataVPtr cylinderData = ModelDataVPtr::New();
	vtkNew<vtkCylinderSource> cylinderSource;
	cylinderSource->SetRadius(radius);
	cylinderSource->SetHeight(height);
	cylinderSource->SetResolution(resolution);
	cylinderSource->SetOutput(cylinderData);
	cylinderSource->Update();
	return cylinderData;
}

ModelDataVPtr ModelData::createTube(const double & radius, double(&p0)[3], 
	double(&p1)[3], const bool& closedEnds, const int& resolution) 
{
	ModelDataVPtr cylinder = ModelDataVPtr::New();
	vtkNew<vtkLineSource> line;
	line->SetPoint1(p0);
	line->SetPoint2(p1);

	vtkNew<vtkTubeFilter> tube;
	tube->SetInputConnection(line->GetOutputPort());
	tube->SetRadius(radius);
	tube->SetCapping(closedEnds);
	tube->SetNumberOfSides(resolution);

	vtkNew<vtkTriangleFilter> triangleFilter;
	triangleFilter->SetInputConnection(tube->GetOutputPort());
	triangleFilter->SetOutput(cylinder);
	triangleFilter->Update();

	return cylinder;
}

ModelDataVPtr ModelData::createSphere(const double& radius, double(&centerPoint)[3], const int& resolution) {
	ModelDataVPtr sphere = ModelDataVPtr::New();
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->SetRadius(radius);
	sphereSource->SetCenter(centerPoint);
	sphereSource->SetOutput(sphere);
	sphereSource->Update();

	return sphere;
}