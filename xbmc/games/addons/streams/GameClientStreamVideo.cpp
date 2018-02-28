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

#include "GameClientStreamVideo.h"
#include "games/addons/GameClientCallbacks.h"
#include "games/addons/GameClientTranslator.h"
#include "utils/log.h"

using namespace KODI;
using namespace GAME;

CGameClientStreamVideo::CGameClientStreamVideo(IGameVideoCallback *video, const game_stream_video_properties &properties) :
  m_video(video),
  m_format(properties.format),
  m_nominalWidth(properties.nominal_width),
  m_nominalHeight(properties.nominal_height),
  m_maxWidth(properties.max_width),
  m_maxHeight(properties.max_height),
  m_aspectRatio(properties.aspect_ratio)
{
}

bool CGameClientStreamVideo::Open()
{
  if (m_video == nullptr)
    return false;

  const AVPixelFormat pixelFormat = CGameClientTranslator::TranslatePixelFormat(m_format);
  if (pixelFormat == AV_PIX_FMT_NONE)
  {
    CLog::Log(LOGERROR, "GAME: Unknown pixel format: %d", m_format);
    return false;
  }

  if (m_nominalWidth == 0 || m_nominalHeight == 0)
  {
    CLog::Log(LOGERROR, "GAME: Invalid nominal dimensions: %ux%u", m_nominalWidth, m_nominalHeight);
    return false;
  }

  if (m_maxWidth == 0 || m_maxHeight == 0)
  {
    CLog::Log(LOGERROR, "GAME: Invalid max dimensions: %ux%u", m_maxWidth, m_maxHeight);
    return false;
  }

  return m_video->OpenStream(pixelFormat, m_nominalWidth, m_nominalHeight, m_maxWidth, m_maxHeight, m_aspectRatio);
}

void CGameClientStreamVideo::Close()
{
  if (m_video != nullptr)
    m_video->CloseStream();
}

void CGameClientStreamVideo::AddData(const game_stream_packet &packet)
{
  if (packet.type != GAME_STREAM_VIDEO)
    return;

  const game_stream_video_packet &video = packet.video;

  // Translate from rotation to orientation
  unsigned int orientationDegCCW = 0;
  switch (video.rotation)
  {
  case GAME_VIDEO_ROTATION_90_CCW:
    orientationDegCCW = 360 - 90;
    break;
  case GAME_VIDEO_ROTATION_180_CCW:
    orientationDegCCW = 360 - 180;
    break;
  case GAME_VIDEO_ROTATION_270_CCW:
    orientationDegCCW = 360 - 270;
    break;
  default:
    break;
  }

  m_video->AddData(video.data, video.size, video.width, video.height, orientationDegCCW);
}
