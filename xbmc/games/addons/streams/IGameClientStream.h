/*
 *      Copyright (C) 2018 Team Kodi
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

#include "addons/kodi-addon-dev-kit/include/kodi/kodi_game_types.h"

namespace KODI
{
namespace GAME
{

class IGameClientStream
{
public:
  virtual ~IGameClientStream() = default;

  /*!
   * \brief Get the stream type
   */
  virtual GAME_STREAM_TYPE Type() const = 0;

  /*!
   * \brief Open the stream
   *
   * \return True if the stream was opened, false otherwise
   */
  virtual bool Open() = 0;

  /*!
   * \brief Close the stream
   */
  virtual void Close() = 0;

  /*!
   * \brief Get a buffer for zero-copy stream data
   *
   * \param width The framebuffer width, or 0 for no width specified
   * \param height The framebuffer height, or 0 for no height specified
   * \param[out] buffer The buffer, or unmodified if false is returned
   *
   * If this returns true, buffer must be freed using ReleaseBuffer().
   *
   * \return True if buffer was set, false otherwise
   */
  virtual bool GetBuffer(unsigned int width, unsigned int height, game_stream_buffer &buffer) { return false; }

  /*!
   * \brief Free an allocated buffer
   *
   * \param buffer The buffer returned from GetStreamBuffer()
   */
  virtual void ReleaseBuffer(game_stream_buffer &buffer) { }

  /*!
   * \brief Add a data packet to a stream
   *
   * \param packet The data packet
   */
  virtual void AddData(const game_stream_packet &packet) = 0;
};

} // namespace GAME
} // namespace KODI
