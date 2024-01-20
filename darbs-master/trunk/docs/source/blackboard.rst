Blackboard
============

What is the Blackboard?
-----------------------
The blackboard acts as the central store for information, and is the backbone for communication in darbs.  Knowledge sources send commands to the blackboard requesting information, and instructing it to make changes to the information stored.

The commands follow a simple but powerful format, which is described in more detail below.  To give a flavour of how communcation with the blackboard works, here is an example of two commands which the blackboard would understand.  You can try this out yourself using the :doc:`terminal` application::

    /* Adds information about the temperature to a partition named
       weather_info on the blackboard.  The blackboard responds with "okay"
       to confirm the information has been added */
    add [the temperature in Leicester is 5 degrees c] weather_info

    /* Checks whether there is matching information on the weather_info
       partition.  The blackboard responds with "True" to confirm that the
       information exists */
    on_partition [the temperature is Leicester is ?temp degrees c] weather_info

Notice how the information which is added here is written as a gramatically complete sentence.  This is typical of how DARBS is used, and is one of its great strengths, making DARBS applications very easily understood, and relatively simple to write, even for those with little or no programming experience.

How does it work?
-----------------
Internally, the blackboard is very simple, consisting of little more than a series of strings of text.  The strings are organised into individual partitions, which can be thought of as a kind of organising structure to the content on the blackboard.  For example, if you were writing a program to predict weather patterns, you might have a partition for storing temperature data, another for storing air-pressure data, and so on.

When data is added to a partition it is simply added to the end of the partition's string of text.  So for example::

    add [the temperature in Leicester is 5 degrees c] weather_info
    add [the temperature in Northampton is 7 degrees c] weather_info

Would result in the following string stored on the weather_info partition::

    [the temperature in Leicester is 5 degrees c][the temperature in Northampton is 7 degrees c]

Accessing and modifying information in the partition is therefore a simple matter of searching through the string until the correct piece of information is found, and making the appropriate changes to it.

When the data on a partition is changed a message is broadcast to all other connected clients, informing them of the change.

Wildcards and substitution
--------------------------

What commands does it understand?
---------------------------------

**setup_blackboard** *[partition_1 partition_2 ...]* - Clears any existing information on the blackboard, and sets up a number of empty partitions.  For example to create three empty partitions on a clean blackboard::

    setup_blackboard [temperature_info air_pressure_info cloud_density_info]

**add_partitions** *[partition_1 partition_2 ...]* - Create a number of new empty partitions on the blackboard.  For example to create three additional empty partitions on the blackboard::

    add_partitions [temperature_info air_pressure_info cloud_density_info]

**add** *[pattern] partition* - Adds some information to a partition on the blackboard.  For example to add temperature data to the weather_info partition::

    add [the temperature in Leicester is 5 degrees c] weather_info

**del_first** *[pattern] partition* - Deletes the first string matching a pattern on a partition.  For example to delete the first piece of temperature data from the weather_info partition::

    del_first [the temperature in Leicester is ?temp degrees c] weather_info

**del_all** *[pattern] partition* - Deletes every string matching a pattern on a partition.  For example to delete all temperature data on Leicester from the weather_info partition::

    del_all [the temperature in Leicester is ?temp degrees c] weather_info

**clr_partition** *partition* - Clears all data from a partition.  For example to clear the weather_info partition::

    clr_partition weather_info

**clr_blackboard** - Clears the entire blackboard, deleting any existing partitions.  For example::

    clr_blackboard

**ret_first** *[pattern] partition* - Matches the first instance of a pattern on a partition, and returns the value matched by wildcards in the pattern.  For example to return the temperature in Leicester from the weather_info partition::

    ret_first [the temperature in Leicester is ?temp degree c] weather_info

**ret_all** *[pattern] partition* - Matches all instances of a pattern on a partition, and returns lists of values matched by the wildcards in the pattern.  For example to return every temperature in Leicester from the weather_info partition::

    ret_all [the temperature in Leicester is ?temp degrees c] weather_info

**get_contents** *partition* - Returns the entire contents of a partition.  For example to get the contents of the weather_info partition::

    get_contents weather_info

**get_partitions** - Returns a list of the partitions present on the blackboard.  For example::

    get_partitions

**partition_exist** *partition* - Returns whether a partition is present on the blackboard.  For example to check whether the weather_info partition is present::

    partition_exist weather_info

**partition_not_exist** *partition* - The inverse of partition_exist.  Returns True if the partition is not present.  For example to check whether the weather_info partition is not present::

    partition_not_exist weather_info

**on_partition** *[pattern] partition* - Check whether a string matching a pattern exists on a partitiion.  For example to check whether there is temperature data on the weather_info partition::

    on_partition [the temperature in Leicester is ?temp degrees c] weather_info

**not_on_partition** *[pattern] partition* - Check whether a string matching a pattern is not present on a partitiion.  For example to check whether no temperature data for Leicester exists on the weather_info partition::

    not_on_partition [the temperature in Leicester is ?temp degrees c] weather_info

**on_blackboard** *[pattern]* - Check whether a string matching a pattern exists on any partition on the blackboard.  For example to check whether there is temperature data for Leicester::
    
    on_blackboard [the temperature in Leicester is ?temp degrees c]

**not_on_blackboard** *[pattern]* - Check whether a string matching a pattern is not present on any partition on the blackboard.  For example to check whether no temperature data for Leicester exists::
    
    not_on_blackboard [the temperature in Leicester is ?temp degrees c]


