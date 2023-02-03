#include <FI/data/CachedModel.h>

#include <vtkObjectFactory.h>

using namespace fi;

vtkStandardNewMacro(CachedModel)

CachedModel::CachedModel() {}

void CachedModel::release() {
	// TODO: Release the memory taken by the underlying polygon mesh.
}
