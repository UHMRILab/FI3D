#pragma once
/*!
* @author	VelazcoJD
* @file		Subtitle.h
* @class	fi3d::Subtitle
* @brief	A Subtitle is a piece of 2D text rendered on the scene.
*/

#include <fi3d/rendering/visuals/Visual.h>

#include <vtkActor2D.h>
#include <vtkSmartPointer.h>

class vtkTextMapper;

namespace fi3d {
class Subtitle : public Visual {

	Q_OBJECT

signals:
	/// @brief Emitted when the text of the subtitle has changed.
	void changedText(const QString& newText);

	/// @brief Emitted when the text color of the subtitle has changed.
	void changedColor(const double& r, const double& g, const double& b);

	/// @brief Emitted when the 2D position of the subtitle has changed.
	void changedLocation(const double& x, const double& y);

protected:
	/*! @brief The text to display. */
	QString mText;

	/*! @brief The mapper that maps the data to the screen. */
	vtkSmartPointer<vtkTextMapper> mMapper;

	/*! @brief The actor that renders the mapped data to the screen. */
	vtkSmartPointer<vtkActor2D> mActor;

public:
	/*! @brief Constructor for the object. */
	Subtitle(const QString& visualID, const QString& text = "");

	/*! @breif Destructor. */
	~Subtitle();

public slots:
	/*! @brief Set displayed text. */
	void setText(const QString& text);

public:
	/*! @brief Get displayed text. */
	QString getText() const;

public slots:
	/*! @brief Set text color. */
	virtual void setColor(const double& r, const double& g, const double& b);

public:
	/*! @brief Get text color. */
	virtual void getColor(double& r, double& g, double& b) const;

public slots:
	/*!
	* @name Text Justifiers
	* @brief Justifies the subtitle text.
	*/
	/// @{
	void justifyTextTop();
	void justifyTextBottom();
	void justifyTextLeft();
	void justifyTextRight();
	/// @}

	/*!
	* @name Text Locations
	* @brief Set location of subtitle text on the screen.
	*
	* Location ranges from (0,0)-bottomLeft to (1, 1)-topRight. Any
	* value outside boundary is capped to it.
	*/
	/// @{
	/*! @brief  */
	void setLocation(const double& x, const double& y);
	void setLocationTopLeft();
	void setLocationTopRight();
	void setLocationBottomLeft();
	void setLocationBottomRight();
	/// @}

public:
	/*! @brief Get the location on the scene. */
	void getLocation(double& x, double& y) const;

	/*!
	*	@name Visual interface implementations.
	*	@brief See Visual for more information.
	*/
	/// @{
	virtual EVisual getVisualType() const override;
	virtual vtkActor2D* getActor() override;
	virtual void setOpacity(const double& a) override;
	virtual double getOpacity() const override;
	virtual void setVisible(const bool& isVisible) override;
	virtual bool isVisible() const override;
	virtual void setPickable(const bool& isPickable) override;
	virtual bool isPickable() const override;
	/// @}
};

/// @brief Alias for a smart pointer of this class.
using SubtitlePtr = QSharedPointer<Subtitle>;

}

