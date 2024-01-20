Agents
======

What are agents?
----------------
Agents are the brains behind a DARBS system.  They are responsible for adding to, removing from and interpreting data on the blackboard.  Each agent is specialized to tackle a small part of the overall problem, rather than trying to tackle the whole thing.  As a result there are a number of different type of agent available in DARBS, and new types can be added to provide additional functionality.

The simplest type of agent consists of an xml file containing a number of compulsory tags (plus optional extra tags and attributes depending on the type of agent).  The basic information contained is:

* The name of the agent
* The type of agent
* Preconditions to determine whether the agent should run
* Actions to execute if the preconditions are met

Structure of an agent
---------------------

You can see this basic structure of a agent in the following example::

    <agent name="square" type="procedural">
        <!-- A precondition -->
        <precondition type="not">
            <request type="present">
                <partition name="control">
                    <pattern>
                        Square is fired
                    </pattern>
                </partition>
            </request>
        </precondition>

        <!-- The actions -->
        <action>
            <request type="run" library="libsquare.so" function="square" variable="result">
                <pattern>
                    2.34
                </pattern>
            </request>

            <request type="add">
                <partition name="control">
                    <pattern>
                        Square is fired
                    </pattern>
                </partition>
            </request>
        </action>
    </agent>

How do agents work?
-------------------
Agents are fairly simple.  First the precondition is evaluated.  The precondition is made up of any combination of queries to the :doc:`blackboard` and additional agent commands, which return true or false.  You may have noticed in the example above that the syntax exactly follows that of blackboard commands.  Each precondition has a boolean type ("and", "or", or "not") which determines how the results of requests within the precondition are combined.  If the precondition is found to be true then the actions are executed.  These can be either commands to send to the blackboard, or agent commands.

When any necessary actions have been taken, the agent returns to the precondition, continually evaluating it until it is true, and further action needs to be taken.

Variables in agents
-------------------

While writing agents it is likely that you will come up against the need to process large groups of similar data.  For example your blackboard may contain 500 pieces of data looking something like::

    <blackboard>
        <partition name="weather_info">
            <pattern>
                The temperature in Leicester is 5 degrees c
            </pattern>
            <pattern>
                The temperature in Nottingham is 7 degrees c
            </pattern>
            <pattern>
                The temperature in London is 4 degrees c
            </pattern>
            ...
        </partition>
    </blackboard>

It would clearly not be very practical to write a separate agent for each place and temperature.  Instead we can use the wildcard feature described in the :doc:`blackboard` help file.  So for example to get all of the matching patterns in a precondition we could use the following::
    
    <agent name="adjust_temperature" type="procedural">
        <precondition>
            <request type="get">
                <partition name="weather_info">
                    <pattern>
                        The temperature in ?place is ?temp degrees c
                    </pattern>
                </partition>
            </request>
        </precondition>
        ...

The actions will then be fired for each matching pattern.

Another common requirement when working with large data sets is to be able to manipulate or use the matched data depending on its value.  In order to do this we can introduce another feature of DARBS: recalling matched variables.  This is done using the tilde (~) character, followed by the name of the wildcard.  So for example ~temperature would be replaced by the value matched in ?temperature.

Using this we can continue the above example to call an external function (see :doc:`procedural` for an example of how to do this) converting the temperatures to farenheit, and storing them on a new partition::

        ...
        <action>
            <!-- Call the function celcius2farenheit in libtemperature.so with the argument ~temperature, and store the result in a variable named farenheitTemp -->
            <request type="run" library="libtemperature.so" function="celcius2farenheit" variable="farenheitTemp">
                <pattern>
                    ~temperature
                </pattern>
            </request>

            <!-- Store the result in a new partition -->
            <request type="add">
                <partition name="farenheit_data">
                    <pattern>
                        The temperature in ~place is ~farenheitTemp farenheit
                    </pattern>
                </partition>
            </request>
        </action>
    </agent>

What types of agent are available?
----------------------------------

DARBS currently has two basic types of agent available.  These are:

* :doc:`rule_based` - Produces knowledge by following a number of simple rules
* :doc:`procedural` - Produces knowledge by running external algorithms

Agent Commands
--------------
**compare** - Compares two numeric values.  For example::
    
    <!-- Tests whether 0.25 <= 0.5 -->
    <request type="compare" operator="<=">
        <pattern>
            0.25
        </pattern>
        <pattern>
            0.5
        </pattern>
    </request>

**run** - Runs a function contained within an external shared library.  The shared library has to exist within the operating system's linker path.  The definition of the function must look like::
    
    extern "C"
    std::string aFunction(std::string argument);
    
For example::
    
    <!-- Run a function named square in the library libsquare.so with a single argument -->
    <request type="run" library="libsquare.so" function="square">
        <pattern>
            0.25
        </pattern>
    </request>

**report** - Prints a string to the command line.  For example::
    
    <request type="report">
        <pattern>
            Knowledge source has been executed
        </pattern>
    </report>
