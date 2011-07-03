/*
 * GO_OrganScreen.h
 *
 *  Created on: Jul 2, 2011
 *      Author: nick
 */

#ifndef GO_ORGANSCREEN_H_
#define GO_ORGANSCREEN_H_

#include <wx/wx.h>
#include <vector>

class wxPanel;
class GO_IRenderable;
class GO_IControl;

#define MOUSE_CLICK_LEFT 0
#define MOUSE_CLICK_RIGHT 1

class GO_OrganScreen
{

private:

	std::vector<GO_IRenderable*> m_Renderables;
	std::vector<GO_IControl*> m_Controls;

public:

	void Clear();
	void AddRenderable(GO_IRenderable* renderable);
	void AddControl(GO_IControl* control);
	void Draw(wxDC& dc, GO_IRenderable* renderable);
	void Scroll(const unsigned x, const unsigned y, const int scroll);
	void MouseButtonDown(const unsigned x, const unsigned y, const int button);
	void KeyEvent(const int wx_key, const unsigned flags);

};

#endif /* GO_ORGANSCREEN_H_ */
