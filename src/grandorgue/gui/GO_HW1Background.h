/**
 * @file:      GO_HW1Background.h
 * @author:    Nick Appleton
 *
 *
 */

#ifndef GO_HW1BACKGROUND_H_
#define GO_HW1BACKGROUND_H_

#include "GO_IRenderable.h"

class GOrgueDisplayMetrics;

class GO_HW1Background : public GO_IRenderable
{

private:

	GOrgueDisplayMetrics& m_DisplayMetrics;

public:

	GO_HW1Background(GOrgueDisplayMetrics& display_metrics);

	virtual unsigned GetLayer();
	virtual unsigned GetX();
	virtual unsigned GetY();
	virtual unsigned GetWidth();
	virtual unsigned GetHeight();
	virtual void Draw(wxDC& dc);
	virtual bool IsDisplayed();

};

#endif /* GO_HW1BACKGROUND_H_ */
