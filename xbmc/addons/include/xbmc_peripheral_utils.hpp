/*
 *      Copyright (C) 2014 Team XBMC
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
#pragma once

#include "xbmc_peripheral_types.h"

#include <string>
#include <string.h>
#include <vector>

namespace ADDON
{
  class JoystickButton
  {
  public:
    JoystickButton(void) : m_id(), m_type() { }

    JoystickButton(JOYSTICK_BUTTON_ID id, JOYSTICK_BUTTON_TYPE type, const std::string& strLabel)
      : m_id(id), m_type(type), m_strLabel(strLabel) { }

    JoystickButton(const JOYSTICK_BUTTON& button)
      : m_id(button.id), m_type(button.type), m_strLabel(button.label ? button.label : "") { }

    JOYSTICK_BUTTON_ID   ID(void) const    { return m_id; }
    JOYSTICK_BUTTON_TYPE Type(void) const  { return m_type; }
    const std::string&   Label(void) const { return m_strLabel; }

    void SetID(JOYSTICK_BUTTON_ID id)          { m_id = id; }
    void SetType(JOYSTICK_BUTTON_TYPE type)    { m_type = type; }
    void SetLabel(const std::string& strLabel) { m_strLabel = strLabel; }

    void ToStruct(JOYSTICK_BUTTON& button)
    {
      button.id = m_id;
      button.type = m_type;
      button.label = new char[m_strLabel.size() + 1];
      strcpy(button.label, m_strLabel.c_str());
    }

    static void FreeStruct(JOYSTICK_BUTTON& button)
    {
      delete[] button.label;
    }

  private:
    JOYSTICK_BUTTON_ID   m_id;
    JOYSTICK_BUTTON_TYPE m_type;
    std::string          m_strLabel;
  };

  class JoystickAnalogStick
  {
  public:
    JoystickAnalogStick(void) : m_id() { }

    JoystickAnalogStick(JOYSTICK_ANALOG_STICK_ID id, const std::string& strLabel)
      : m_id(id), m_strLabel(strLabel) { }

    JoystickAnalogStick(const JOYSTICK_ANALOG_STICK& analogStick)
      : m_id(analogStick.id), m_strLabel(analogStick.label ? analogStick.label : "") { }

    JOYSTICK_ANALOG_STICK_ID ID(void) const    { return m_id; }
    const std::string&       Label(void) const { return m_strLabel; }

    void SetID(JOYSTICK_ANALOG_STICK_ID id)    { m_id = id; }
    void SetLabel(const std::string& strLabel) { m_strLabel = strLabel; }

    void ToStruct(JOYSTICK_ANALOG_STICK& analogStick)
    {
      analogStick.id = m_id;
      analogStick.label = new char[m_strLabel.size() + 1];
      strcpy(analogStick.label, m_strLabel.c_str());
    }

    static void FreeStruct(JOYSTICK_ANALOG_STICK& analogStick)
    {
      delete[] analogStick.label;
    }

  private:
    JOYSTICK_ANALOG_STICK_ID   m_id;
    std::string          m_strLabel;
  };

  class JoystickInfo
  {
  public:
    JoystickInfo(void) : m_requestedPlayer(0), m_buttonCount(0), m_hatCount(0), m_axisCount(0) { }

    JoystickInfo(JOYSTICK_INFO& info)
    : m_strName(info.name),
      m_requestedPlayer(info.requested_player_num),
      m_buttonCount(info.virtual_layout.button_count),
      m_hatCount(info.virtual_layout.hat_count),
      m_axisCount(info.virtual_layout.axis_count)
    {
      for (unsigned int i = 0; i < info.physical_layout.button_count; i++)
        m_buttons.push_back(JoystickButton(info.physical_layout.buttons[i]));

      for (unsigned int i = 0; i < info.physical_layout.analog_stick_count; i++)
        m_analogSticks.push_back(JoystickAnalogStick(info.physical_layout.analog_sticks[i]));
    }

    const std::string& Name(void) const            { return m_strName; }
    unsigned int       RequestedPlayer(void) const { return m_requestedPlayer; }
    unsigned int       ButtonCount(void) const     { return m_buttonCount; }
    unsigned int       HatCount(void) const        { return m_hatCount; }
    unsigned int       AxisCount(void) const       { return m_axisCount; }

    const std::vector<JoystickButton>&      Buttons(void) const      { return m_buttons; }
    const std::vector<JoystickAnalogStick>& AnalogSticks(void) const { return m_analogSticks; }

    void SetName(const std::string& strName)              { m_strName         = strName; }
    void SetRequestedPlayer(unsigned int requestedPlayer) { m_requestedPlayer = requestedPlayer; }
    void SetButtonCount(unsigned int buttonCount)         { m_buttonCount     = buttonCount; }
    void SetHatCount(unsigned int hatCount)               { m_hatCount        = hatCount; }
    void SetAxisCount(unsigned int axisCount)             { m_axisCount       = axisCount; }

    std::vector<JoystickButton>&      Buttons(void)      { return m_buttons; }
    std::vector<JoystickAnalogStick>& AnalogSticks(void) { return m_analogSticks; }

    void ToStruct(JOYSTICK_INFO& info)
    {
      info.name                               = new char[m_strName.size() + 1];
      strcpy(info.name, m_strName.c_str());
      info.requested_player_num               = m_requestedPlayer;
      info.virtual_layout.button_count        = m_buttonCount;
      info.virtual_layout.hat_count           = m_hatCount;
      info.virtual_layout.axis_count          = m_axisCount;
      info.physical_layout.button_count       = m_buttons.size();
      info.physical_layout.buttons            = NULL;
      info.physical_layout.analog_stick_count = m_analogSticks.size();
      info.physical_layout.analog_sticks      = NULL;

      if (m_buttons.size())
      {
        info.physical_layout.buttons = new JOYSTICK_BUTTON[m_buttons.size()];
        for (unsigned int i = 0; i < m_buttons.size(); i++)
          m_buttons[i].ToStruct(info.physical_layout.buttons[i]);
      }

      if (m_analogSticks.size())
      {
        info.physical_layout.analog_sticks = new JOYSTICK_ANALOG_STICK[m_analogSticks.size()];
        for (unsigned int i = 0; i < m_analogSticks.size(); i++)
          m_analogSticks[i].ToStruct(info.physical_layout.analog_sticks[i]);
      }
    }

    void FreeStruct(JOYSTICK_INFO& info)
    {
      delete[] info.name;

      for (unsigned int i = 0; i < info.physical_layout.button_count; i++)
        JoystickButton::FreeStruct(info.physical_layout.buttons[i]);
      delete[] info.physical_layout.buttons;

      for (unsigned int i = 0; i < info.physical_layout.analog_stick_count; i++)
        JoystickAnalogStick::FreeStruct(info.physical_layout.analog_sticks[i]);
      delete[] info.physical_layout.analog_sticks;
    }

  private:
    std::string  m_strName;
    unsigned int m_requestedPlayer;
    unsigned int m_buttonCount;
    unsigned int m_hatCount;
    unsigned int m_axisCount;

    std::vector<JoystickButton>      m_buttons;
    std::vector<JoystickAnalogStick> m_analogSticks;
  };

  class PeripheralEvent
  {
  public:
    PeripheralEvent(void) : m_index(0), m_type(), m_data(NULL) { }

    PeripheralEvent(const PERIPHERAL_EVENT& event)
    : m_index(event.peripheral_index),
      m_type(event.type),
      m_data(NULL)
    {
      SetData(event.event_data);
    }

    ~PeripheralEvent(void)
    {
      ClearData();
    }

    unsigned int        PeripheralIndex(void) const { return m_index; }
    JOYSTICK_EVENT_TYPE Type(void) const            { return m_type; }

    template <typename EVENT_TYPE>
    const EVENT_TYPE& EventAsType(void) const
    {
      static const EVENT_TYPE emptyEvent = { };
      return m_data ? *static_cast<const EVENT_TYPE*>(m_data) : emptyEvent;
    }

    void ClearData(void)
    {
      PERIPHERAL_EVENT event = { m_index, m_type, m_data };
      FreeStruct(event);
    }

    void SetData(void* event)
    {
      ClearData();

      if (event)
      {
        switch (m_type)
        {
          case JOYSTICK_EVENT_TYPE_VIRTUAL_BUTTON:
          {
            JOYSTICK_EVENT_VIRTUAL_BUTTON* setEvent = static_cast<JOYSTICK_EVENT_VIRTUAL_BUTTON*>(event);
            JOYSTICK_EVENT_VIRTUAL_BUTTON* newEvent = new JOYSTICK_EVENT_VIRTUAL_BUTTON;
            *newEvent = *setEvent;
            m_data = newEvent;
            break;
          }
          case JOYSTICK_EVENT_TYPE_VIRTUAL_HAT:
          {
            JOYSTICK_EVENT_VIRTUAL_HAT* setEvent = static_cast<JOYSTICK_EVENT_VIRTUAL_HAT*>(event);
            JOYSTICK_EVENT_VIRTUAL_HAT* newEvent = new JOYSTICK_EVENT_VIRTUAL_HAT;
            *newEvent = *setEvent;
            m_data = newEvent;
            break;
          }
          case JOYSTICK_EVENT_TYPE_VIRTUAL_AXIS:
          {
            JOYSTICK_EVENT_VIRTUAL_AXIS* setEvent = static_cast<JOYSTICK_EVENT_VIRTUAL_AXIS*>(event);
            JOYSTICK_EVENT_VIRTUAL_AXIS* newEvent = new JOYSTICK_EVENT_VIRTUAL_AXIS;
            *newEvent = *setEvent;
            m_data = newEvent;
            break;
          }
          case JOYSTICK_EVENT_TYPE_BUTTON_DIGITAL:
          {
            JOYSTICK_EVENT_BUTTON_DIGITAL* setEvent = static_cast<JOYSTICK_EVENT_BUTTON_DIGITAL*>(event);
            JOYSTICK_EVENT_BUTTON_DIGITAL* newEvent = new JOYSTICK_EVENT_BUTTON_DIGITAL;
            *newEvent = *setEvent;
            m_data = newEvent;
            break;
          }
          case JOYSTICK_EVENT_TYPE_BUTTON_ANALOG:
          {
            JOYSTICK_EVENT_BUTTON_ANALOG* setEvent = static_cast<JOYSTICK_EVENT_BUTTON_ANALOG*>(event);
            JOYSTICK_EVENT_BUTTON_ANALOG* newEvent = new JOYSTICK_EVENT_BUTTON_ANALOG;
            *newEvent = *setEvent;
            m_data = newEvent;
            break;
          }
          case JOYSTICK_EVENT_TYPE_ANALOG_STICK:
          {
            JOYSTICK_EVENT_ANALOG_STICK* setEvent = static_cast<JOYSTICK_EVENT_ANALOG_STICK*>(event);
            JOYSTICK_EVENT_ANALOG_STICK* newEvent = new JOYSTICK_EVENT_ANALOG_STICK;
            *newEvent = *setEvent;
            m_data = newEvent;
            break;
          }
          case JOYSTICK_EVENT_TYPE_ACCELEROMETER:
          {
            JOYSTICK_EVENT_ACCELEROMETER* setEvent = static_cast<JOYSTICK_EVENT_ACCELEROMETER*>(event);
            JOYSTICK_EVENT_ACCELEROMETER* newEvent = new JOYSTICK_EVENT_ACCELEROMETER;
            *newEvent = *setEvent;
            m_data = newEvent;
            break;
          }
          case JOYSTICK_EVENT_TYPE_NONE:
          default:
            break;
        }
      }
    }

    static void ToStructs(const std::vector<PeripheralEvent>& events, PERIPHERAL_EVENT** eventStructs)
    {
      if (!events.empty() && eventStructs)
      {
        *eventStructs = new PERIPHERAL_EVENT[events.size()];
        for (unsigned int i = 0; i < events.size(); i++)
        {
          (*eventStructs)[i].peripheral_index = events.at(i).m_index;
          (*eventStructs)[i].type             = events.at(i).m_type;
          (*eventStructs)[i].event_data       = NULL;

          switch (events.at(i).m_type)
          {
            case JOYSTICK_EVENT_TYPE_VIRTUAL_BUTTON:
              (*eventStructs)[i].event_data = new JOYSTICK_EVENT_VIRTUAL_BUTTON(events.at(i).EventAsType<JOYSTICK_EVENT_VIRTUAL_BUTTON>());
              break;
            case JOYSTICK_EVENT_TYPE_VIRTUAL_HAT:
              (*eventStructs)[i].event_data = new JOYSTICK_EVENT_VIRTUAL_HAT(events.at(i).EventAsType<JOYSTICK_EVENT_VIRTUAL_HAT>());
              break;
            case JOYSTICK_EVENT_TYPE_VIRTUAL_AXIS:
              (*eventStructs)[i].event_data = new JOYSTICK_EVENT_VIRTUAL_AXIS(events.at(i).EventAsType<JOYSTICK_EVENT_VIRTUAL_AXIS>());
              break;
            case JOYSTICK_EVENT_TYPE_BUTTON_DIGITAL:
              (*eventStructs)[i].event_data = new JOYSTICK_EVENT_BUTTON_DIGITAL(events.at(i).EventAsType<JOYSTICK_EVENT_BUTTON_DIGITAL>());
              break;
            case JOYSTICK_EVENT_TYPE_BUTTON_ANALOG:
              (*eventStructs)[i].event_data = new JOYSTICK_EVENT_BUTTON_ANALOG(events.at(i).EventAsType<JOYSTICK_EVENT_BUTTON_ANALOG>());
              break;
            case JOYSTICK_EVENT_TYPE_ANALOG_STICK:
              (*eventStructs)[i].event_data = new JOYSTICK_EVENT_ANALOG_STICK(events.at(i).EventAsType<JOYSTICK_EVENT_ANALOG_STICK>());
              break;
            case JOYSTICK_EVENT_TYPE_ACCELEROMETER:
              (*eventStructs)[i].event_data = new JOYSTICK_EVENT_ACCELEROMETER(events.at(i).EventAsType<JOYSTICK_EVENT_ACCELEROMETER>());
              break;
            case JOYSTICK_EVENT_TYPE_NONE:
            default:
              break;
          }
        }
      }
    }

    static void FreeStruct(PERIPHERAL_EVENT& event)
    {
      switch (event.type)
      {
        case JOYSTICK_EVENT_TYPE_VIRTUAL_BUTTON:
          delete static_cast<JOYSTICK_EVENT_VIRTUAL_BUTTON*>(event.event_data);
          break;
        case JOYSTICK_EVENT_TYPE_VIRTUAL_HAT:
          delete static_cast<JOYSTICK_EVENT_VIRTUAL_HAT*>(event.event_data);
          break;
        case JOYSTICK_EVENT_TYPE_VIRTUAL_AXIS:
          delete static_cast<JOYSTICK_EVENT_VIRTUAL_AXIS*>(event.event_data);
          break;
        case JOYSTICK_EVENT_TYPE_BUTTON_DIGITAL:
          delete static_cast<JOYSTICK_EVENT_BUTTON_DIGITAL*>(event.event_data);
          break;
        case JOYSTICK_EVENT_TYPE_BUTTON_ANALOG:
          delete static_cast<JOYSTICK_EVENT_BUTTON_ANALOG*>(event.event_data);
          break;
        case JOYSTICK_EVENT_TYPE_ANALOG_STICK:
          delete static_cast<JOYSTICK_EVENT_ANALOG_STICK*>(event.event_data);
          break;
        case JOYSTICK_EVENT_TYPE_ACCELEROMETER:
          delete static_cast<JOYSTICK_EVENT_ACCELEROMETER*>(event.event_data);
          break;
        case JOYSTICK_EVENT_TYPE_NONE:
        default:
          break;
      }
      event.event_data = NULL;
    }

    static void FreeStructs(unsigned int eventCount, PERIPHERAL_EVENT* events)
    {
      for (unsigned int i = 0; i < eventCount; i++)
        FreeStruct(events[i]);
      delete[] events;
    }

  private:
    unsigned int        m_index;
    JOYSTICK_EVENT_TYPE m_type;
    void*               m_data;
  };


  /*
  class ButtonMapValue { };

  class ButtonMapButton
  {

  };


  typedef std::map<JoystickElement*, ButtonMapValue> ButtonMap;

  typedef std::map<>

  class ButtonMap
  {
  public:
    ButtonMap(void) : m_index(0), m_id(JOYSTICK_ID_BUTTON_UNKNOWN) { }

    ButtonMap(const JOYSTICK_MAP_BUTTON& buttonMap)
    : m_index(buttonMap.index),
      m_id(buttonMap.id),
      m_strLabel(buttonMap.label ? buttonMap.label : "")
    {
    }

    unsigned int       Index(void) const { return m_index; }
    JOYSTICK_ID_BUTTON ID(void) const    { return m_id; }
    const std::string& Label(void) const { return m_strLabel; }

    void SetIndex(unsigned int index)          { m_index = index; }
    void SetID(JOYSTICK_ID_BUTTON buttonId)    { m_id = buttonId; }
    void SetLabel(const std::string& strLabel) { m_strLabel = strLabel; }

    void ToButtonMap(JOYSTICK_MAP_BUTTON& buttonMap) const
    {
      buttonMap.index = m_index;
      buttonMap.id    = m_id;
      buttonMap.label = new char[m_strLabel.size() + 1];
      std::strcpy(buttonMap.label, m_strLabel.c_str());
    }

    static void Free(JOYSTICK_MAP_BUTTON& buttonMap)
    {
      delete[] buttonMap.label;
    }

  private:
    unsigned int       m_index;
    JOYSTICK_ID_BUTTON m_id;
    std::string        m_strLabel;
  };

  class TriggerMap
  {
  public:
    TriggerMap(void) : m_axisIndex(0), m_axisSign(JOYSTICK_AXIS_POSITIVE), m_id(JOYSTICK_ID_TRIGGER_UNKNOWN) { }

    TriggerMap(const JOYSTICK_MAP_TRIGGER& triggerMap)
    : m_axisIndex(triggerMap.axis_index),
      m_axisSign(triggerMap.axis_sign),
      m_id(triggerMap.id),
      m_strLabel(triggerMap.label ? triggerMap.label : "")
    {
    }

    unsigned int        AxisIndex(void) const { return m_axisIndex; }
    JOYSTICK_AXIS_SIGN  AxisSign(void) const  { return m_axisSign; }
    JOYSTICK_ID_TRIGGER ID(void) const        { return m_id; }
    const std::string&  Label(void) const     { return m_strLabel; }

    void SetAxisIndex(unsigned int axisIndex)     { m_axisIndex = axisIndex; }
    void SetAxisSign(JOYSTICK_AXIS_SIGN axisSign) { m_axisSign = axisSign; }
    void SetID(JOYSTICK_ID_TRIGGER triggerId)     { m_id = triggerId; }
    void SetLabel(const std::string& strLabel)    { m_strLabel = strLabel; }

    void ToTriggerMap(JOYSTICK_MAP_TRIGGER& triggerMap) const
    {
      triggerMap.axis_index = m_axisIndex;
      triggerMap.axis_sign  = m_axisSign;
      triggerMap.id         = m_id;
      triggerMap.label      = new char[m_strLabel.size() + 1];
      std::strcpy(triggerMap.label, m_strLabel.c_str());
    }

    static void Free(JOYSTICK_MAP_TRIGGER& triggerMap)
    {
      delete[] triggerMap.label;
    }

  private:
    unsigned int        m_axisIndex;
    JOYSTICK_AXIS_SIGN  m_axisSign;
    JOYSTICK_ID_TRIGGER m_id;
    std::string         m_strLabel;
  };

  class AnalogStickMap
  {
  public:
    AnalogStickMap(void)
    : m_horizAxisIndex(0),
      m_horizPositiveDir(JOYSTICK_POSITIVE_DIR_UP_RIGHT),
      m_vertAxisIndex(0),
      m_vertPositiveDir(JOYSTICK_POSITIVE_DIR_UP_RIGHT),
      m_id(JOYSTICK_ID_ANALOG_STICK_UNKNOWN)
   {
   }

    AnalogStickMap(const JOYSTICK_MAP_ANALOG_STICK& analogStickMap)
    : m_horizAxisIndex(analogStickMap.horiz_axis_index),
      m_horizPositiveDir(analogStickMap.horiz_positive_dir),
      m_vertAxisIndex(analogStickMap.vert_axis_index),
      m_vertPositiveDir(analogStickMap.vert_positive_dir),
      m_id(analogStickMap.id),
      m_strLabel(analogStickMap.label ? analogStickMap.label : "")
    {
    }

    unsigned int             HorizAxisIndex(void) const { return m_horizAxisIndex; }
    JOYSTICK_POSITIVE_DIR    HorizAxisSign(void) const  { return m_horizPositiveDir; }
    unsigned int             VertAxisIndex(void) const  { return m_vertAxisIndex; }
    JOYSTICK_POSITIVE_DIR    VertAxisSign(void) const   { return m_vertPositiveDir; }
    JOYSTICK_ID_ANALOG_STICK ID(void) const             { return m_id; }
    const std::string&       Label(void) const          { return m_strLabel; }

    void SetHorizAxisIndex(unsigned int horizAxisIndex)           { m_horizAxisIndex = horizAxisIndex; }
    void SetHorizAxisSign(JOYSTICK_POSITIVE_DIR horizPositiveDir) { m_horizPositiveDir = horizPositiveDir; }
    void SetVertAxisIndex(unsigned int vertAxisIndex)             { m_vertAxisIndex = vertAxisIndex; }
    void SetVertAxisSign(JOYSTICK_POSITIVE_DIR vertPositiveDir)   { m_vertPositiveDir = vertPositiveDir; }
    void SetID(JOYSTICK_ID_ANALOG_STICK analogStickId)            { m_id = analogStickId; }
    void SetLabel(const std::string& strLabel)                    { m_strLabel = strLabel; }

    void ToAnalogStickMap(JOYSTICK_MAP_ANALOG_STICK& analogStickMap) const
    {
      analogStickMap.horiz_axis_index   = m_horizAxisIndex;
      analogStickMap.horiz_positive_dir = m_horizPositiveDir;
      analogStickMap.vert_axis_index    = m_vertAxisIndex;
      analogStickMap.vert_positive_dir  = m_vertPositiveDir;
      analogStickMap.id                 = m_id;
      analogStickMap.label              = new char[m_strLabel.size() + 1];
      std::strcpy(analogStickMap.label, m_strLabel.c_str());
    }

    static void Free(JOYSTICK_MAP_ANALOG_STICK& analogStickMap)
    {
      delete[] analogStickMap.label;
    }

  private:
    unsigned int             m_horizAxisIndex;
    JOYSTICK_POSITIVE_DIR    m_horizPositiveDir;
    unsigned int             m_vertAxisIndex;
    JOYSTICK_POSITIVE_DIR    m_vertPositiveDir;
    JOYSTICK_ID_ANALOG_STICK m_id;
    std::string              m_strLabel;
  };

  class JoystickLayout
  {
  public:
    JoystickLayout(void) { }

    JoystickLayout(const JOYSTICK_LAYOUT& layout)
    {
      for (unsigned int i = 0; i < layout.button_count; i++)
        m_buttonMap.push_back(layout.buttons[i]);

      for (unsigned int i = 0; i < layout.trigger_count; i++)
        m_triggerMap.push_back(layout.triggers[i]);

      for (unsigned int i = 0; i < layout.analog_stick_count; i++)
        m_analogStickMap.push_back(layout.analog_sticks[i]);
    }

    const std::vector<ButtonMap>&      ButtonMaps(void) const      { return m_buttonMap; }
    const std::vector<TriggerMap>&     TriggerMaps(void) const     { return m_triggerMap; }
    const std::vector<AnalogStickMap>& AnalogStickMaps(void) const { return m_analogStickMap; }

    std::vector<ButtonMap>&      ButtonMaps(void)      { return m_buttonMap; }
    std::vector<TriggerMap>&     TriggerMaps(void)     { return m_triggerMap; }
    std::vector<AnalogStickMap>& AnalogStickMaps(void) { return m_analogStickMap; }

    void ToJoystickLayout(JOYSTICK_LAYOUT& layout) const
    {
      layout.button_count = m_buttonMap.size();
      layout.buttons = new JOYSTICK_MAP_BUTTON[m_buttonMap.size()];
      for (unsigned int i = 0; i < m_buttonMap.size(); i++)
        m_buttonMap[i].ToButtonMap(layout.buttons[i]);

      layout.trigger_count = m_triggerMap.size();
      layout.triggers = new JOYSTICK_MAP_TRIGGER[m_triggerMap.size()];
      for (unsigned int i = 0; i < m_triggerMap.size(); i++)
        m_triggerMap[i].ToTriggerMap(layout.triggers[i]);

      layout.analog_stick_count = m_analogStickMap.size();
      layout.analog_sticks = new JOYSTICK_MAP_ANALOG_STICK[m_analogStickMap.size()];
      for (unsigned int i = 0; i < m_analogStickMap.size(); i++)
        m_analogStickMap[i].ToAnalogStickMap(layout.analog_sticks[i]);
    }

    static void Free(JOYSTICK_LAYOUT& layout)
    {
      for (unsigned int i = 0; i < layout.button_count; i++)
        ButtonMap::Free(layout.buttons[i]);
      delete[] layout.buttons;

      for (unsigned int i = 0; i < layout.trigger_count; i++)
        TriggerMap::Free(layout.triggers[i]);
      delete[] layout.triggers;

      for (unsigned int i = 0; i < layout.analog_stick_count; i++)
        AnalogStickMap::Free(layout.analog_sticks[i]);
      delete[] layout.analog_sticks;
    }

  private:
    std::vector<ButtonMap>      m_buttonMap;
    std::vector<TriggerMap>     m_triggerMap;
    std::vector<AnalogStickMap> m_analogStickMap;
  };

  class JoystickConfiguration
  {
  public:
    JoystickConfiguration(void) : m_index(0), m_requestedPlayer(0) { }

    JoystickConfiguration(const JOYSTICK_CONFIGURATION& joystick)
    : m_index(joystick.index),
      m_requestedPlayer(joystick.requested_player),
      m_strName(joystick.name ? joystick.name : ""),
      m_strIconPath(joystick.icon_path ? joystick.icon_path : ""),
      m_layout(joystick.layout)
    {
      m_buttonIndexes.reserve(joystick.button_count);
      for (unsigned int i = 0; i < joystick.button_count; i++)
        m_buttonIndexes.push_back(joystick.buttons[i]);

      m_hatIndexes.reserve(joystick.hat_count);
      for (unsigned int i = 0; i < joystick.hat_count; i++)
        m_hatIndexes.push_back(joystick.hats[i]);

      m_axisIndexes.reserve(joystick.axis_count);
      for (unsigned int i = 0; i < joystick.axis_count; i++)
        m_axisIndexes.push_back(joystick.axes[i]);
    }

    unsigned int       Index(void) const { return m_index; }
    unsigned int       RequestedPlayer(void) const { return m_requestedPlayer; }
    const std::string& Name(void) const { return m_strName; }
    const std::string& IconPath(void) const { return m_strIconPath; }

    const std::vector<unsigned int>& ButtonIndexes(void) const { return m_buttonIndexes; }
    const std::vector<unsigned int>& HatIndexes(void) const    { return m_hatIndexes; }
    const std::vector<unsigned int>& AxisIndexes(void) const   { return m_axisIndexes; }

    void SetIndex(unsigned int index)                     { m_index = index; }
    void SetRequestedPlayer(unsigned int requestedPlayer) { m_requestedPlayer = requestedPlayer; }
    void SetName(const std::string& strName)              { m_strName = strName; }
    void SetIconPath(const std::string& strIconPath)      { m_strIconPath = strIconPath; }

    std::vector<unsigned int>& ButtonIndexes(void) { return m_buttonIndexes; }
    std::vector<unsigned int>& HatIndexes(void)    { return m_hatIndexes; }
    std::vector<unsigned int>& AxisIndexes(void)   { return m_axisIndexes; }

    void ToJoystickConfiguration(JOYSTICK_CONFIGURATION& joystick) const
    {
      joystick.index            = m_index;
      joystick.requested_player = m_requestedPlayer;
      joystick.name             = new char[m_strName.length() + 1];
      std::strcpy(joystick.name, m_strName.c_str());
      joystick.icon_path        = new char[m_strIconPath.length() + 1];
      std::strcpy(joystick.icon_path, m_strIconPath.c_str());

      joystick.button_count = m_buttonIndexes.size();
      joystick.buttons = new unsigned int[m_buttonIndexes.size()];
      for (unsigned int i = 0; i < m_buttonIndexes.size(); i++)
        joystick.buttons[i] = m_buttonIndexes[i];

      joystick.hat_count = m_hatIndexes.size();
      joystick.hats = new unsigned int[m_hatIndexes.size()];
      for (unsigned int i = 0; i < m_hatIndexes.size(); i++)
        joystick.hats[i] = m_hatIndexes[i];

      joystick.axis_count = m_axisIndexes.size();
      joystick.axes = new unsigned int[m_axisIndexes.size()];
      for (unsigned int i = 0; i < m_axisIndexes.size(); i++)
        joystick.axes[i] = m_axisIndexes[i];

      m_layout.ToJoystickLayout(joystick.layout);
    }

    static void Free(JOYSTICK_CONFIGURATION& joystick)
    {
      delete[] joystick.name;
      delete[] joystick.icon_path;
      delete[] joystick.buttons;
      delete[] joystick.hats;
      delete[] joystick.axes;

      JoystickLayout::Free(joystick.layout);
    }

  private:
    unsigned int              m_index;
    unsigned int              m_requestedPlayer;
    std::string               m_strName;
    std::string               m_strIconPath;
    std::vector<unsigned int> m_buttonIndexes;
    std::vector<unsigned int> m_hatIndexes;
    std::vector<unsigned int> m_axisIndexes;
    JoystickLayout            m_layout;
  };
  */
}
