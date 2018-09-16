/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DialogGameOSD.h"
#include "DialogGameOSDHelp.h"
#include "games/GameServices.h"
#include "games/GameSettings.h"
#include "guilib/GUIMessage.h"
#include "guilib/WindowIDs.h"
#include "utils/log.h"
#include "ServiceBroker.h"

using namespace KODI;
using namespace GAME;

CDialogGameOSD::CDialogGameOSD() :
  CGUIDialog(WINDOW_DIALOG_GAME_OSD, "GameOSD.xml"),
  m_helpDialog(new CDialogGameOSDHelp(*this))
{
  // Initialize CGUIWindow
  m_loadType = KEEP_IN_MEMORY;
}

bool CDialogGameOSD::OnMessage(CGUIMessage& message)
{
  switch (message.GetMessage())
  {
    case GUI_MSG_WINDOW_DEINIT:
    {
      // Intercept the close action if a help control is visible
      if (m_helpDialog->IsVisible() && CServiceBroker::IsServiceManagerUp())
      {
        // Check if the help dialog should be shown
        GAME::CGameSettings &gameSettings = CServiceBroker::GetGameServices().GameSettings();
        if (gameSettings.ShowOSDHelp())
        {
          // Block close action and don't show help dialog in the future
          CLog::Log(LOGDEBUG, "------ Game OSD deinit intercepted by help dialog ------");
          gameSettings.SetShowOSDHelp(false);
          return true;
        }
      }
      break;
    }
    default:
      break;
  }

  return CGUIDialog::OnMessage(message);
}

void CDialogGameOSD::OnInitWindow()
{
  // Init parent class
  CGUIDialog::OnInitWindow();

  // Init help dialog
  m_helpDialog->OnInitWindow();
}
