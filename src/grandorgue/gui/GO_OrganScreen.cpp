/*
 * GO_OrganScreen.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: nick
 */

#include "GO_OrganScreen.h"
#include "GO_IRenderable.h"
#include "GO_IControl.h"

void GO_OrganScreen::AddRenderable(GO_IRenderable* renderable)
{

	m_Renderables.push_back(renderable);

}

void GO_OrganScreen::AddControl(GO_IControl* control)
{

	m_Controls.push_back(control);

}

void GO_OrganScreen::Clear()
{

	m_Renderables.clear();

}

void GO_OrganScreen::Draw(wxDC& dc, GO_IRenderable* renderable)
{

	if (!renderable)
	{

		unsigned layer = 0;
		unsigned nb_drawn = 0;

		while (nb_drawn < m_Renderables.size() + m_Controls.size())
		{

			for (unsigned i = 0; i < m_Renderables.size(); i++)
				if (m_Renderables[i]->GetLayer() == layer)
				{
					if (m_Renderables[i]->IsDisplayed())
						m_Renderables[i]->Draw(dc);
					nb_drawn++;
				}

			for (unsigned i = 0; i < m_Controls.size(); i++)
				if (m_Controls[i]->GetLayer() == layer)
				{
					if (m_Controls[i]->IsDisplayed())
						m_Controls[i]->Draw(dc);
					nb_drawn++;
				}

			layer++;

		}

	}
	else
	{

		renderable->Draw(dc);

	}


}

void GO_OrganScreen::Scroll(const unsigned x, const unsigned y, const int scroll)
{

	for (unsigned i = 0; i < m_Controls.size(); i++)
		if (
			(m_Controls[i]->IsDisplayed()) &&
			(x >= m_Controls[i]->GetX()) &&
			(y >= m_Controls[i]->GetY()) &&
			(x < m_Controls[i]->GetX() + m_Controls[i]->GetWidth()) &&
			(y < m_Controls[i]->GetY() + m_Controls[i]->GetHeight())
			)
		{

			m_Controls[i]->Scroll
				(x - m_Controls[i]->GetX()
				,y - m_Controls[i]->GetY()
				,scroll
				);

		}

}

void GO_OrganScreen::MouseButtonDown(const unsigned x, const unsigned y, const int click)
{

	for (unsigned i = 0; i < m_Controls.size(); i++)
		if (
			(m_Controls[i]->IsDisplayed()) &&
			(x >= m_Controls[i]->GetX()) &&
			(y >= m_Controls[i]->GetY()) &&
			(x < m_Controls[i]->GetX() + m_Controls[i]->GetWidth()) &&
			(y < m_Controls[i]->GetY() + m_Controls[i]->GetHeight())
			)
		{

			m_Controls[i]->MouseButtonDown
				(x - m_Controls[i]->GetX()
				,y - m_Controls[i]->GetY()
				,click == MOUSE_CLICK_RIGHT ? GO_IControl::GO_MB_RIGHT : GO_IControl::GO_MB_LEFT
				);

		}

}

void GO_OrganScreen::KeyEvent(const int wx_key, const unsigned flags)
{

	for (unsigned i = 0; i < m_Controls.size(); i++)
		if (m_Controls[i]->IsDisplayed())
			m_Controls[i]->OnKeyEvent(wx_key, flags);

}

