/*
 *      Copyright (C) 2015 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "games/ControllerLayout.h"
#include "guilib/Geometry.h"
#include "guilib/GUIButtonControl.h"
#include "guilib/GUIControl.h"
#include "guilib/GUIControlGroupList.h"
#include "guilib/GUIFocusPlane.h"
#include "guilib/GUIWindowManager.h"
#include "guilib/WindowIDs.h"
#include "GUIDialogControllerFeatures.h"
#include "input/Key.h"
#include "utils/log.h"

#define GROUP_LIST             996
#define BUTTON_TEMPLATE       1000
#define BUTTON_START          1001

using namespace ADDON;
using namespace GAME;

CGUIDialogControllerFeatures::CGUIDialogControllerFeatures(void)
  : CGUIDialog(WINDOW_DIALOG_CONTROLLER_FEATURES, "DialogControllerFeatures.xml"),
    m_layout(NULL),
    m_focusControl(NULL)
{
  m_loadType = KEEP_IN_MEMORY;
}

void CGUIDialogControllerFeatures::DoModal(const ControllerLayoutPtr& layout, CGUIFocusPlane* focusControl)
{
  if (IsDialogRunning())
    return;

  Initialize();

  if (SetupButtons(layout, focusControl))
    CGUIDialog::DoModal();

  CleanupButtons();
}

bool CGUIDialogControllerFeatures::SetupButtons(const ControllerLayoutPtr& layout, CGUIFocusPlane* focusControl)
{
  if (!layout || !focusControl)
    return false;

  CGUIButtonControl* pButtonTemplate = GetButtonTemplate();
  CGUIControlGroupList* pGroupList = dynamic_cast<CGUIControlGroupList*>(GetControl(GROUP_LIST));

  if (!pButtonTemplate || !pGroupList)
    return false;

  const std::vector<GAME::Button>& buttons = layout->Buttons();

  unsigned int buttonId = BUTTON_START;
  for (std::vector<GAME::Button>::const_iterator it = buttons.begin(); it != buttons.end(); ++it)
  {
    CGUIButtonControl* pButton = MakeButton(it->strLabel, buttonId++, pButtonTemplate);

    // try inserting context buttons at position specified by template
    // button, if template button is not in grouplist fallback to adding
    // new buttons at the end of grouplist
    if (!pGroupList->InsertControl(pButton, pButtonTemplate))
      pGroupList->AddControl(pButton);
  }

  // update our default control
  m_defaultControl = GROUP_LIST;
  m_lastControlID = BUTTON_START;

  m_layout = layout;
  m_focusControl = focusControl;

  // restore last selected control
  std::map<std::string, unsigned int>::const_iterator it = m_lastControlIds.find(m_layout->Addon()->ID());
  if (it != m_lastControlIds.end())
    m_lastControlID = it->second;

  return true;
}

void CGUIDialogControllerFeatures::CleanupButtons(void)
{
  CGUIControlGroupList* pGroupList = dynamic_cast<CGUIControlGroupList*>(GetControl(GROUP_LIST));
  if (pGroupList)
    pGroupList->ClearAll();

  m_layout = NULL;
  m_focusControl = NULL;
}

CGUIButtonControl* CGUIDialogControllerFeatures::GetButtonTemplate(void)
{
  CGUIButtonControl* pButtonTemplate = dynamic_cast<CGUIButtonControl*>(GetFirstFocusableControl(BUTTON_TEMPLATE));
  if (!pButtonTemplate)
    pButtonTemplate = dynamic_cast<CGUIButtonControl*>(GetControl(BUTTON_TEMPLATE));
  return pButtonTemplate;
}

CGUIButtonControl* CGUIDialogControllerFeatures::MakeButton(const std::string& strLabel,
                                                            unsigned int       id,
                                                            CGUIButtonControl* pButtonTemplate)
{
  CGUIButtonControl* pButton = new CGUIButtonControl(*pButtonTemplate);

  // set the button's ID and position
  pButton->SetID(id);
  pButton->SetVisible(true);
  pButton->SetLabel(strLabel);
  pButton->SetPosition(pButtonTemplate->GetXPosition(), pButtonTemplate->GetYPosition());

  return pButton;
}

bool CGUIDialogControllerFeatures::OnMove(void)
{
  if (m_layout && m_focusControl)
  {
    const std::vector<GAME::Button>& buttons = m_layout->Buttons();

    int iSelectedIndex = GetSelectedControl(GROUP_LIST) - BUTTON_START;
    if (0 <= iSelectedIndex && iSelectedIndex < (int)buttons.size())
    {
      m_focusControl->SetFocus(buttons[iSelectedIndex].focusArea);
      return true;
    }
  }

  return false;
}

bool CGUIDialogControllerFeatures::OnClick(int iSelectedControl)
{
  if (m_layout && m_focusControl && iSelectedControl >= BUTTON_START)
  {
    const std::vector<GAME::Button>& buttons = m_layout->Buttons();

    unsigned int iSelectedIndex = iSelectedControl - BUTTON_START;
    if (iSelectedIndex < buttons.size())
    {
      // TODO: prompt user to press button
      return true;
    }
  }

  return false;
}

bool CGUIDialogControllerFeatures::OnMessage(CGUIMessage& message)
{
  switch (message.GetMessage())
  {
    case GUI_MSG_CLICKED:
    {
      if (OnClick(message.GetSenderId()))
        return true;
      break;
    }
  }

  if (CGUIDialog::OnMessage(message))
  {
    switch (message.GetMessage())
    {
      case GUI_MSG_MOVE:
      {
        OnMove();
        break;
      }
    }
    return true;
  }

  return false;
}

bool CGUIDialogControllerFeatures::OnAction(const CAction& action)
{
  if (action.GetID() == ACTION_CONTEXT_MENU)
  {
    Close();
    return true;
  }

  return CGUIDialog::OnAction(action);
}

void CGUIDialogControllerFeatures::OnInitWindow(void)
{
  CGUIDialog::OnInitWindow();

  // disable the template button control
  CGUIButtonControl* pButtonTemplate = GetButtonTemplate();
  if (pButtonTemplate)
    pButtonTemplate->SetVisible(false);

  SetSelectedControl(GROUP_LIST, m_lastControlID);

  OnMove();
}

void CGUIDialogControllerFeatures::OnDeinitWindow(int nextWindowID)
{
  if (m_focusControl)
    m_focusControl->Unfocus();

  // save selected item for next time
  if (m_layout)
  {
    int iSelectedControl = GetSelectedControl(GROUP_LIST);
    if (iSelectedControl >= BUTTON_START)
      m_lastControlIds[m_layout->Addon()->ID()] = iSelectedControl;
  }

  CGUIDialog::OnDeinitWindow(nextWindowID);
}

int CGUIDialogControllerFeatures::GetSelectedControl(int iControl)
{
  CGUIMessage msg(GUI_MSG_ITEM_SELECTED, GetID(), iControl);

  if (CGUIWindow::OnMessage(msg))
    return msg.GetParam1() >= 0 ? msg.GetParam1() : -1;

  return -1;
}

void CGUIDialogControllerFeatures::SetSelectedControl(int iControl, int iSelectedControl)
{
  CGUIMessage msg(GUI_MSG_ITEM_SELECT, GetID(), iControl, iSelectedControl);
  OnMessage(msg);
}
