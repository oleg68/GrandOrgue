/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2015 GrandOrgue contributors (see AUTHORS)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef GORGUESETTER_H
#define GORGUESETTER_H

#include "ptrvector.h"
#include "GOrgueEnclosure.h"
#include "GOrgueLabel.h"
#include "GOrguePlaybackStateHandler.h"
#include "GOrgueSetterButtonCallback.h"

class GOGUIControl;
class GOGUILabel;
class GOGUIPanel;
class GOrgueConfigReader;
class GOrgueConfigWriter;
class GOrgueFrameGeneral;
class GOrgueMidiEvent;
class GOrgueSetterButton;
class GrandOrgueFile;
struct IniFileEnumEntry;

typedef enum 
{
	SETTER_REGULAR,
	SETTER_SCOPE,
	SETTER_SCOPED
} SetterType;

class GOrgueSetter : private GOrgueSetterButtonCallback, private GOrguePlaybackStateHandler
{
private:
	GrandOrgueFile* m_organfile;
	unsigned m_pos;
	unsigned m_bank;
	unsigned m_crescendopos;
	unsigned m_crescendobank;
	ptr_vector<GOrgueFrameGeneral> m_framegeneral;
	ptr_vector<GOrgueFrameGeneral> m_general;
	ptr_vector<GOrgueFrameGeneral> m_crescendo;
	ptr_vector<GOrgueSetterButton> m_button;
	GOrgueLabel m_PosDisplay;
	GOrgueLabel m_BankDisplay;
	GOrgueLabel m_CrescendoDisplay;
	GOrgueLabel m_TransposeDisplay;
	GOrgueLabel m_NameDisplay;
	GOrgueEnclosure m_swell;
	SetterType m_SetterType;

	void SetSetterType(SetterType type);
	void SetCrescendoType(unsigned no);
	void Crescendo(int pos, bool force = false);

	static const struct IniFileEnumEntry m_setter_element_types[];

	void SetterButtonChanged(GOrgueSetterButton* button);

	void AbortPlayback();
	void PreparePlayback();
	void StartPlayback();
	void PrepareRecording();

public:
	GOrgueSetter(GrandOrgueFile* organfile);
	virtual ~GOrgueSetter();

	GOGUIPanel* CreateCouplerPanel(GOrgueConfigReader& cfg, unsigned manual);
	GOGUIPanel* CreateGeneralsPanel(GOrgueConfigReader& cfg);
	GOGUIPanel* CreateSetterPanel(GOrgueConfigReader& cfg);
	GOGUIPanel* CreateCrescendoPanel(GOrgueConfigReader& cfg);
	GOGUIPanel* CreateDivisionalPanel(GOrgueConfigReader& cfg);
	GOGUIPanel* CreateFloatingPanel(GOrgueConfigReader& cfg);
	GOGUIPanel* CreateMasterPanel(GOrgueConfigReader& cfg);
	GOGUIControl* CreateGUIElement(GOrgueConfigReader& cfg, wxString group, GOGUIPanel* panel);

	void Load(GOrgueConfigReader& cfg);
	void Update();
	void ControlChanged(void* control);

	bool StoreInvisibleObjects();
	bool IsSetterActive();
	void ToggleSetter();
	void SetterActive(bool on);
	SetterType GetSetterType();

	void Next();
	void Prev();
	void Push();
	unsigned GetPosition();
	void UpdatePosition(int pos);
	void SetPosition(int pos, bool push = true);
	void ResetDisplay();
	void SetTranspose(int value);
	void UpdateTranspose();
	void UpdateModified(bool modified);
};

#endif
