#pragma once
/*
 *      Copyright (C) 2012-2013 Team XBMC
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

#include <string>
#include <vector>

struct AInputEvent;
class IGenericJoystickInputHandler;

typedef struct {
  float     flat;
  float     fuzz;
  float     min;
  float     max;
  float     range;
  float     value;
  // internal helper values
  bool      enabled;
  float     deadzone;
  float     buttonclamp;
} APP_InputDeviceAxis;

typedef struct {
  int32_t id;
  char    name[256];
  int     vid;
  int     pid;
  APP_InputDeviceAxis x_hat;
  APP_InputDeviceAxis y_hat;
  APP_InputDeviceAxis x_axis;
  APP_InputDeviceAxis y_axis;
  APP_InputDeviceAxis z_axis;
  APP_InputDeviceAxis rz_axis;
  IGenericJoystickInputHandler *input_handler;
} APP_InputDevice;

class CAndroidJoyStick
{
public:
  CAndroidJoyStick() { }
 ~CAndroidJoyStick();

  bool onJoyStickKeyEvent(AInputEvent* event);
  bool onJoyStickMotionEvent(AInputEvent* event);

private:
  void  ProcessMotionEvents(AInputEvent *event, size_t pointer_index,
          int32_t device, APP_InputDevice *devices);
  void  ProcessAxis(AInputEvent *event, size_t pointer_index,
          APP_InputDeviceAxis &axis, int device, int keymap_axis, int android_axis);

  std::vector<APP_InputDevice*> m_input_devices;
};
