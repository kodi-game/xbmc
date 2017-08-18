/*
 *      Copyright (C) 2017 Team Kodi
 *      http://kodi.tv
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
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
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
