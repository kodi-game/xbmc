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

#include "GameClientStreamAudio.h"
#include "cores/AudioEngine/Utils/AEChannelData.h"
#include "cores/AudioEngine/Utils/AEChannelInfo.h"
#include "games/addons/GameClientCallbacks.h"
#include "games/addons/GameClientTranslator.h"
#include "utils/log.h"

#include <cmath>

using namespace KODI;
using namespace GAME;

CGameClientStreamAudio::CGameClientStreamAudio(IGameAudioCallback *audio, const game_stream_audio_properties &properties, double sampleRate) :
  m_audio(audio),
  m_format(properties.format),
  m_sampleRate(sampleRate),
  m_channelLayout(GetChannelLayout(properties.channel_map))
{
}

bool CGameClientStreamAudio::Open()
{
  if (m_audio == nullptr)
    return false;

  const AEDataFormat pcmFormat = CGameClientTranslator::TranslatePCMFormat(m_format);
  if (pcmFormat == AE_FMT_INVALID)
  {
    CLog::Log(LOGERROR, "GAME: Unknown PCM format: %d", m_format);
    return false;
  }

  unsigned int sampleRate = std::round(m_sampleRate);
  if (sampleRate == 0)
  {
    CLog::Log(LOGERROR, "GAME: Invalid samplerate: %f", m_sampleRate);
    return false;
  }

  CAEChannelInfo channelLayout;
  for (GAME_AUDIO_CHANNEL gameChannel : m_channelLayout)
  {
    AEChannel channel = CGameClientTranslator::TranslateAudioChannel(gameChannel);
    if (channel == AE_CH_NULL)
    {
      CLog::Log(LOGERROR, "GAME: Unknown channel ID: %d", gameChannel);
      return false;
    }
    channelLayout += channel;
  }

  if (m_channelLayout.empty())
  {
    CLog::Log(LOGERROR, "GAME: Empty channel layout");
    return false;
  }

  return m_audio->OpenStream(pcmFormat, sampleRate, channelLayout);
}

void CGameClientStreamAudio::Close()
{
  if (m_audio != nullptr)
    m_audio->CloseStream();
}

void CGameClientStreamAudio::AddData(const game_stream_packet &packet)
{
  if (packet.type != GAME_STREAM_AUDIO)
    return;

  const game_stream_audio_packet &audio = packet.audio;

  m_audio->AddData(audio.data, audio.size);
}

std::vector<GAME_AUDIO_CHANNEL> CGameClientStreamAudio::GetChannelLayout(const GAME_AUDIO_CHANNEL* channelMap)
{
  std::vector<GAME_AUDIO_CHANNEL> channelLayout;

  if (channelMap != nullptr)
  {
    for (const GAME_AUDIO_CHANNEL* channelPtr = channelMap; *channelPtr != GAME_CH_NULL; channelPtr++)
      channelLayout.push_back(*channelPtr);
  }

  return channelLayout;
}
