#! /bin/bash
killall -q agentclient
agentclient Init.dkf &
agentclient HarmonyAnalyser.dkf &
agentclient HarmonyConstrainer.dkf &
agentclient NoteConstrainer.dkf &
agentclient Determinator.dkf &
agentclient MelodyAnalyser.dkf &
agentclient MelodyConstrainer.dkf &
