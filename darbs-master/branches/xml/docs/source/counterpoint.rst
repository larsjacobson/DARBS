Counterpoint Example
====================

Now that we have looked through the basics of DARBS we will look in more detail at a larger system.  The counterpoint example (found in the examples/counterpoint directory) is a set of agents for generating a harmonically and melodically pleasant accompaniment to a simple tune.

Running the Example
-------------------
The example can be run by starting up the blackboard server, and executing the run.sh script contained within the example's directory.  Alternatively each agent file can be run individually using the agentclient application.

The Agents
----------
The counterpoint example uses 8 agents, each with a specific set of skills, in order to generate the accompaniment.  The agents are run simultaneously, and act as soon as there is enough information in the system for them to do so.

**Init**
    The Init agent adds any static values to the blackboard, and creates the cantus firmus partition.  The cantus firmus is the tune which the system will create an accompaniment for.  Patterns of the form "Cantus Firmus value N is N" should be added to the partition to specify this initial melody.  The rule AddCantusFirmus gives an example of how to add these patterns.

**OptionAdder**
    The OptionAdder agent adds patterns giving the possible values for each note in the counterpoint.  The example only takes a fairly trivial example where we want a single counterpoint note for each note in the cantus firmus, so the agent will simply add a new set of options for each note that is added to the cantus firmus.

**HarmonyAnalyser**
    The HarmonyAnalyser agent looks at the harmonic relationship between notes in the cantus firmus, and the options for notes in the counterpoint.  It classifies the musical interval between the two notes and stores it in the intervals partition.

**HarmonyConstrainer**
    The HarmonyConstrainer agent uses the results from the HarmonyAnalyser agent to remove any options which are considered to have an inharmonic relationship with the cantus firmus.

**MelodyAnalyser**
    The MelodyAnalyser agent looks at the melodic relationship between subsequent notes in the counterpoint.  It classifies the musical interval between the subsequent notes, and stores it in the movement partition.

**MelodyConstrainer**
    The MelodyConstrainer agent uses the results from the MelodyAnalyser agent to remove any options which are considered to have an unmelodic relationship with the previous note in the counterpoint.

**NoteConstrainer**
    The NoteConstrainer agent removes any options for notes which are not contained within the C Major scale.

**Determinator**
    The Determinator agent checks to see whether enough analysis and constraint has taken place to choose one of the options to become our final note in the counterpoint and, if it has, adds a pattern to the counterpoint partition.

Listening to the counterpoint
-----------------------------
In the examples/counterpoint/player directory there is an HTML5 audio player which allows you to listen back to the cantus firmus, the counterpoint, or both simultaneously.  The input boxes take a list of the notes generated, separated by spaces (i.e. "62 65 64 62").  The player will work with Firefox 4.0 or later.

