/*
 *  Copyright (C) 2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

namespace KODI
{
namespace RETRO
{
class IPlayback;

class IGameCallback
{
public:
  virtual ~IGameCallback() = default;

  /*!
   * \brief Get the game client being used to play the game
   *
   * \return The game client's ID, or empty if no game is being played
   */
  virtual std::string GameClientID() const = 0;

  /*!
   * \brief Get the game that is being played
   *
   * \return The path to the game, or empty if no game is being played
   */
  virtual std::string GetPlayingGame() const = 0;

  virtual IPlayback* GetPlayback() const = 0;

  virtual void ResetGame() = 0;

};
} // namespace RETRO
} // namespace KODI
