/**
 * @file:      GO_HW1Background.cpp
 * @author:    Nick Appleton
 *
 *
 */

#include "GO_HW1Background.h"
#include "GOrgueDisplayMetrics.h"
#include "OrganPanel.h"

GO_HW1Background::GO_HW1Background(GOrgueDisplayMetrics& display_metrics) :
	m_DisplayMetrics (display_metrics)
{

}

unsigned GO_HW1Background::GetLayer()
{

	return 0;

}

unsigned GO_HW1Background::GetX()
{

	return 0;

}

unsigned GO_HW1Background::GetY()
{

	return 0;

}

unsigned GO_HW1Background::GetWidth()
{

	return m_DisplayMetrics.GetScreenWidth();

}

unsigned GO_HW1Background::GetHeight()
{

	return m_DisplayMetrics.GetScreenHeight();

}

void GO_HW1Background::Draw(wxDC& dc)
{

	unsigned centre_x      = m_DisplayMetrics.GetCenterX();
	unsigned centre_width  = m_DisplayMetrics.GetCenterWidth();
	unsigned height        = m_DisplayMetrics.GetScreenHeight();
	unsigned width         = m_DisplayMetrics.GetScreenWidth();

	/* Left drawstop panel */
	OrganPanel::TileWood
		(dc
		,m_DisplayMetrics.GetDrawstopBackgroundImageNum()
		,0
		,0
		,centre_x
		,height
		);

	/* Centre panel for keyboards etc */
	OrganPanel::TileWood
		(dc
		,m_DisplayMetrics.GetConsoleBackgroundImageNum()
		,centre_x
		,0
		,centre_width
		,height
		);

	/* Right drawstop panel */
	OrganPanel::TileWood
		(dc
		,m_DisplayMetrics.GetDrawstopBackgroundImageNum()
		,centre_x + centre_width
		,0
		,width - (centre_x + centre_width)
		,height
		);

	if (m_DisplayMetrics.HasPairDrawstopCols())
	{

		for (unsigned i = 0; i < (m_DisplayMetrics.NumberOfDrawstopColsToDisplay() >> 2); i++)
		{

			OrganPanel::TileWood
				(dc
				,m_DisplayMetrics.GetDrawstopInsetBackgroundImageNum()
				,i * 174 + m_DisplayMetrics.GetJambLeftX() - 5
				,m_DisplayMetrics.GetJambLeftRightY()
				,166
				,m_DisplayMetrics.GetJambLeftRightHeight()
				);

			OrganPanel::TileWood
				(dc
				,m_DisplayMetrics.GetDrawstopInsetBackgroundImageNum()
				,i * 174 + m_DisplayMetrics.GetJambRightX() - 5
				,m_DisplayMetrics.GetJambLeftRightY()
				,166
				,m_DisplayMetrics.GetJambLeftRightHeight()
				);

		}

	}

	if (m_DisplayMetrics.HasTrimAboveExtraRows())
		OrganPanel::TileWood
			(dc
			,m_DisplayMetrics.GetKeyVertBackgroundImageNum()
			,centre_x
			,m_DisplayMetrics.GetCenterY()
			,centre_width
			,8
			);

	if (m_DisplayMetrics.GetJambTopHeight() + m_DisplayMetrics.GetPistonTopHeight())
		OrganPanel::TileWood
			(dc
			,m_DisplayMetrics.GetKeyHorizBackgroundImageNum()
			,centre_x
			,m_DisplayMetrics.GetJambTopY()
			,centre_width
			,m_DisplayMetrics.GetJambTopHeight() + m_DisplayMetrics.GetPistonTopHeight()
			);

}

bool GO_HW1Background::IsDisplayed()
{

	return true;

}
