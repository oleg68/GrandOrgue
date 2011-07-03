/*
 * GO_HW1Images.h
 *
 *  Created on: Jul 3, 2011
 *      Author: nick
 */

#ifndef GO_HW1IMAGES_H_
#define GO_HW1IMAGES_H_

#include <wx/wx.h>
#include <vector>

class GO_HW1Images
{

private:

	wxBitmap m_Label;
	std::vector<wxBitmap> m_Stops;
	std::vector<wxBitmap> m_Panels;

public:

	GO_HW1Images();

	wxBitmap& GetStopBitmap(unsigned index);
	wxBitmap& GetLabelBackgroundBitmap();
	wxBitmap& GetWoodBitmap(unsigned index);

};

#endif /* GO_HW1IMAGES_H_ */
