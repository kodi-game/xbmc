/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "controllers/ControllerTypes.h"

#include <memory>
#include <string>

class CProfilesManager;
class CSettings;

namespace ADDON
{
  class CAddonMgr;
  class CBinaryAddonManager;
}

namespace PERIPHERALS
{
  class CPeripherals;
}

namespace KODI
{
namespace RETRO
{
  class CGUIGameRenderManager;
}

namespace SHADER
{
  class CVideoShaderPresetFactory;
}

namespace GAME
{
  class CControllerManager;
  class CGameSettings;

  class CGameServices
  {
  public:
    CGameServices(CControllerManager &controllerManager,
                  RETRO::CGUIGameRenderManager &renderManager,
                  CSettings &settings,
                  PERIPHERALS::CPeripherals &peripheralManager,
                  const CProfilesManager &profileManager,
                  ADDON::CAddonMgr &addons,
                  ADDON::CBinaryAddonManager &binaryAddons);
    ~CGameServices();

    ControllerPtr GetController(const std::string& controllerId);
    ControllerPtr GetDefaultController();
    ControllerPtr GetDefaultKeyboard();
    ControllerPtr GetDefaultMouse();
    ControllerVector GetControllers();

    std::string GetSavestatesFolder() const;

    CGameSettings& GameSettings() { return *m_gameSettings; }

    RETRO::CGUIGameRenderManager &GameRenderManager() { return m_gameRenderManager; }

    SHADER::CVideoShaderPresetFactory &VideoShaders() { return *m_videoShaders; }

  private:
    // Construction parameters
    CControllerManager &m_controllerManager;
    RETRO::CGUIGameRenderManager &m_gameRenderManager;
    const CProfilesManager &m_profileManager;

    // Game services
    std::unique_ptr<CGameSettings> m_gameSettings;
    std::unique_ptr<SHADER::CVideoShaderPresetFactory> m_videoShaders;
  };
}
}
