#include <fi3d/rendering/visuals/2D/Subtitle.h>

#include <fi3d/logger/Logger.h>

#include <vtkProperty2D.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>

using namespace fi3d;

Subtitle::Subtitle(const QString& id, const QString& text)
	: Visual(id, false),
	mText(""),
	mMapper(vtkSmartPointer<vtkTextMapper>::New()),
	mActor(vtkSmartPointer<vtkActor2D>::New())
{
	// Create the text actor that displays coordinate information
	vtkSmartPointer<vtkTextProperty> textProp = 
		vtkSmartPointer<vtkTextProperty>::New();
	textProp->SetFontFamilyToTimes();
	textProp->SetFontSize(15);
	textProp->SetVerticalJustificationToTop();
	textProp->SetJustificationToLeft();
	
	mMapper->SetTextProperty(textProp);
	mActor->SetMapper(mMapper);
	mActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();

	this->setText(text);
}

Subtitle::~Subtitle() {}

void Subtitle::setText(const QString& text) {
	if (mText == text) {
		return;
	}
	mText = text;
	mMapper->SetInput(mText.toLatin1().data());
	emit changedText(text);
}

QString Subtitle::getText() const {
	return mText;
}

void Subtitle::setColor(const double& r, const double& g, const double& b) {
	double cr, cg, cb;
	this->getColor(cr, cg, cb);
	if (cr == r && cg == g && cb == b) {
		return;
	}

	mActor->GetProperty()->SetColor(r, g, b);
	emit changedColor(r, g, b);
}

void Subtitle::getColor(double& r, double& g, double& b) const {
	mActor->GetProperty()->GetColor(r, g, b);
}

void Subtitle::justifyTextTop() {
	mMapper->GetTextProperty()->SetVerticalJustificationToTop();
}

void Subtitle::justifyTextBottom() {
	mMapper->GetTextProperty()->SetVerticalJustificationToBottom();
}

void Subtitle::justifyTextLeft() {
	mMapper->GetTextProperty()->SetJustificationToLeft();
}

void Subtitle::justifyTextRight() {
	mMapper->GetTextProperty()->SetJustificationToRight();
}

void Subtitle::setLocation(const double& x, const double& y) {
	double cx, cy;
	this->getLocation(cx, cy);

	if (cx == x && cy == y)
		return;

	if (x < 0)
		cx = 0;
	else if (x > 1)
		cx = 1;
	else
		cx = x;

	if (y < 0)
		cy = 0;
	else if (y > 1)
		cy = 1;
	else
		cy = y;
	
	mActor->GetPositionCoordinate()->SetValue(cx, cy);
	emit changedLocation(x, y);

}

void Subtitle::setLocationTopLeft() {
	this->setLocation(0.02, 0.98);
}

void Subtitle::setLocationTopRight() {
	this->setLocation(0.98, 0.98);
}

void Subtitle::setLocationBottomLeft() {
	this->setLocation(0.02, 0.02);
}

void Subtitle::setLocationBottomRight() {
	this->setLocation(0.98, 0.02);
}

void Subtitle::getLocation(double& x, double& y) const {
	double coords[3];
	mActor->GetPositionCoordinate()->GetValue(coords);
	x = coords[0];
	y = coords[1];
}

EVisual Subtitle::getVisualType() const {
	return EVisual::SUBTITLE;
}

vtkActor2D* Subtitle::getActor() {
	return mActor;
}

void Subtitle::setOpacity(const double& a) {
	if (a == this->getOpacity()) {
		return;
	}
	mActor->GetProperty()->SetOpacity(a);
	emit changedOpacity(a);
}

double Subtitle::getOpacity() const {
	return mActor->GetProperty()->GetOpacity();
}

void Subtitle::setVisible(const bool& isVisible) {
	if (isVisible == this->isVisible()) {
		return;
	}
	if (isVisible)
		mActor->VisibilityOn();
	else
		mActor->VisibilityOff();
	emit changedVisibleState(isVisible);
}

bool Subtitle::isVisible() const {
	return mActor->GetVisibility();
}

void Subtitle::setPickable(const bool& isPickable) {
	if (isPickable == this->isPickable()) {
		return;
	}
	if (isPickable)
		mActor->PickableOn();
	else
		mActor->PickableOff();
	emit changedPickableState(isPickable);
}

bool Subtitle::isPickable() const {
	return mActor->GetPickable();
}
