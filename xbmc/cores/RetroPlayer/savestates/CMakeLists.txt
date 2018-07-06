set(SOURCES SavestateDatabase.cpp
            SavestateFlatBuffer.cpp
            SavestateUtils.cpp
)

set(HEADERS ISavestate.h
            SavestateDatabase.h
            SavestateFlatBuffer.h
            SavestateTypes.h
            SavestateUtils.h
)

set(DEPENDS retroplayer_messages)

core_add_library(retroplayer_savestates)
add_dependencies(retroplayer_savestates retroplayer_messages)
