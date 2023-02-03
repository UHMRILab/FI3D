#include <fi3d/data/ImageData.h>

#include <vtkObjectFactory.h>

using namespace fi3d;

vtkStandardNewMacro(ImageData)

ImageData::ImageData()
	: DataObject()
{
}

EData ImageData::getDataType() const {
	return EData::IMAGE;
}

ImageDataVPtr ImageData::createImageData(
	const int & dimX, const int & dimY, const int & dimZ, 
	const double & spaceX, const double & spaceY, const double spaceZ, 
	int type) 
{
	ImageDataVPtr data = ImageDataVPtr::New();
	data->SetDimensions(dimX, dimY, dimZ);
	data->SetSpacing(spaceX, spaceY, spaceZ);
	data->AllocateScalars(type, 1);
		
	return data;
}
