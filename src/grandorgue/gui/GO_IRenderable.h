/**
 * @file:      GO_IRenderable.h
 * @author:    Nick Appleton
 *
 *
 */

#ifndef GO_IRENDERABLE_H_
#define GO_IRENDERABLE_H_

#include <wx/wx.h>

class GO_IRenderable
{

public:

	virtual ~GO_IRenderable() { };
	virtual unsigned GetLayer() = 0;
	virtual unsigned GetX() = 0;
	virtual unsigned GetY() = 0;
	virtual unsigned GetWidth() = 0;
	virtual unsigned GetHeight() = 0;
	virtual void Draw(wxDC& dc) = 0;

	/**
	 * Determines whether the control is displayed.
	 */
	virtual bool IsDisplayed() = 0;

};


#endif /* GO_IRENDERABLE_H_ */
