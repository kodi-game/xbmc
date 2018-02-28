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

#include "GameClientStreams.h"
#include "GameClientStreamAudio.h"
#include "GameClientStreamVideo.h"
#include "addons/kodi-addon-dev-kit/include/kodi/kodi_game_types.h"
#include "games/addons/GameClient.h"

#include <memory>

using namespace KODI;
using namespace GAME;


CGameClientStreams::CGameClientStreams(CGameClient &gameClient) :
  m_gameClient(gameClient)
{
}

void CGameClientStreams::Initialize(IGameAudioCallback *audio, IGameVideoCallback *video)
{
  m_audio = audio;
  m_video = video;
};

void CGameClientStreams::Deinitialize()
{
  m_audio = nullptr;
  m_video = nullptr;
}

IGameClientStream *CGameClientStreams::OpenStream(const game_stream_properties &properties)
{
  std::unique_ptr<IGameClientStream> stream;

  switch (properties.type)
  {
    case GAME_STREAM_AUDIO:
    {
      stream.reset(new CGameClientStreamAudio(m_audio, properties.audio, m_gameClient.GetSampleRate()));
      break;
    }
    case GAME_STREAM_VIDEO:
    {
      stream.reset(new CGameClientStreamVideo(m_video, properties.video));
      break;
    }
    default:
      break;
  }

  if (stream && stream->Open())
    return stream.release();

  return nullptr;
}

void CGameClientStreams::CloseStream(IGameClientStream *stream)
{
  delete stream;
}
