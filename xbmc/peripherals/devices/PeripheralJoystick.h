#pragma once
/*
 *      Copyright (C) 2005-2013 Team XBMC
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

#include "Peripheral.h"
#include "peripherals/PeripheralAddon.h"
#include "addons/include/xbmc_peripheral_utils.hpp"

namespace PERIPHERALS
{
  class CPeripheralJoystick : public CPeripheral // TODO: extend CPeripheralHID
  {
  public:
    CPeripheralJoystick(const PeripheralScanResult& scanResult);
    virtual ~CPeripheralJoystick(void) { }

    virtual bool InitialiseFeature(const PeripheralFeature feature);

    const std::string& Name(void) const            { return m_info.Name(); }
    unsigned int       RequestedPlayer(void) const { return m_info.RequestedPlayer(); }

    void OnEvent(const ADDON::PeripheralEvent& event);

  private:
    PeripheralAddonPtr  m_addon;
    unsigned int        m_index;
    ADDON::JoystickInfo m_info;
  };
}
