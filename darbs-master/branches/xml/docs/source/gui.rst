GUI
===

DARBS comes with a web-based GUI which allows you to view and modify the :doc:`blackboard`, and run :doc:`agent` using a simple graphical interface.  

Dependencies
------------
The DARBS GUI is written in python using the pylons web framework.  In order to run it you need:

* Python 2.x (Tested using >= 2.5) - http://www.python.org
* Pylons 1.0 - http://www.pylonshq.com

Running the GUI
---------------
In order to run the GUI we can use the web server built into pylons.  Firstly start up the :doc:`blackboard` in a new terminal.  Then run the webserver by typing the following commands into a terminal::

    source /PATH/TO/PYLONS/bin/activate
    cd /PATH/TO/DARBS/gui
    paster serve --reload development.ini

You should see the server starting with a message which says something like::
    
    serving on http://127.0.0.1:5000

Navigate your web browser to the IP address given, and you should see the DARBS GUI running.

Using the GUI
-------------
The GUI is split into three columns.  On the left you have a column which shows all of the running agents.  The centre column lists the partitions currently on the blackboard.  The right hand column shows the contents of one of the partitions.

Editing the Blackboard
----------------------
Firstly, try creating a partition on the blackboard.  You can do this by clicking the plus sign under the centre column, and typing a name for the partition.  If you click on the partition, the right hand column will show that the partition is empty.  You can then add a pattern to the selected partition by clicking the plus sign underneath the right hand column.  Patterns and partitions can be edited and deleted using the button at the right hand side of their containers.

**Activity** - Try adding further partitions and patterns to the blackboard.  Familiarise yourself with the edit and delete controls.

Running agents
-------------------------
To run an agent from the GUI it first needs copying to a location where the GUI can find it.  Either use your file manager, or from a command line run::
    
    cp -r /PATH/TO/AGENT /PATH/TO/DARBS/gui/gui/lib/agent

If you then click the plus sign beneath the left column you should see your agent in a drop-down menu.  If you select your agent and run it it will appear in the left hand column, along with information about the agent type and its current running state.  The agent can be stopped and restarted using the buttons to the right of the container, and also the log file can be viewed for debugging the agent.

