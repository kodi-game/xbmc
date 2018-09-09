/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DialogGameOSD.h"
#include "games/controllers/guicontrols/GUIGameController.h"
#include "games/GameServices.h"
#include "games/GameSettings.h"
#include "guilib/GUIMessage.h"
#include "guilib/LocalizeStrings.h"
#include "guilib/WindowIDs.h"
#include "utils/StringUtils.h"
#include "ServiceBroker.h"

using namespace KODI;
using namespace GAME;

static constexpr int CONTROL_OSD_HELP_TEXT = 1101;
static constexpr int CONTROL_OSD_GAME_CONTROLLER = 1102;

CDialogGameOSD::CDialogGameOSD() :
  CGUIDialog(WINDOW_DIALOG_GAME_OSD, "GameOSD.xml")
{
  // Initialize CGUIWindow
  m_loadType = KEEP_IN_MEMORY;
}

void CDialogGameOSD::OnInitWindow()
{
  CGUIDialog::OnInitWindow();

  // Set help text
  // "Press {0:s} to open the menu."
  std::string helpText = StringUtils::Format(g_localizeStrings.Get(35235), "Select + X");
  SET_CONTROL_LABEL(CONTROL_OSD_HELP_TEXT, helpText);

  // Set controller
  if (CServiceBroker::IsServiceManagerUp())
  {
    CGameServices& gameServices = CServiceBroker::GetGameServices();

    ControllerPtr controller = gameServices.GetController("game.controller.snes");
    if (controller)
    {
      //! @todo Activate controller for all game controller controls
      CGUIGameController* guiController = dynamic_cast<CGUIGameController*>(GetControl(CONTROL_OSD_GAME_CONTROLLER));
      if (guiController != nullptr)
        guiController->ActivateController(controller);
    }
  }
}

void CDialogGameOSD::OnDeinitWindow(int nextWindowID)
{
  CGUIDialog::OnDeinitWindow(nextWindowID);

  if (CServiceBroker::IsServiceManagerUp())
  {
    // Hide OSD help
    GAME::CGameSettings &gameSettings = CServiceBroker::GetGameServices().GameSettings();
    gameSettings.SetShowOSDHelp(false);
  }
}
