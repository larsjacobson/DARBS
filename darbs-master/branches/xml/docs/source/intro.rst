Introduction
============

What is DARBS?
--------------
DARBS (Distributed Algorithmic and Rule-Based Blackboard System) is a framework for building multi-agent intelligent systems.

What can it be used for?
------------------------
DARBS (and it's predecessor ARBS) has been used to tackle a wide variety of problems.  To give you a taste of the range of potential applications, here are just some of the ways in which DARBS has been used:

* Monitoring and addressing bottlenecks in telecommunications systems
* Automatically controlling plasma deposition
* Simulating multi-agent systems
* Automatically recognising printing defects

DARBS is particularly well suited to tackling problems where using a hybrid approach (combining a number of different AI techniques) is helpful.

How does it work?
-----------------
DARBS is split into two main sections - the blackboard, and the agents.  The Blackboard runs as a standalone server application, and is responsible for storing and retrieving 'knowledge'.  Agents act as client to the blackboard, adding to, removing, altering and analysing the knowledge contained on the blackboard.  In doing this the agents are entirely opportunistic.  If they see information which is relevant to them, the agents will act straight away.  Using this process of adding to and refining the knowledge on the blackboard, the agents work toward a solution to the problem.

