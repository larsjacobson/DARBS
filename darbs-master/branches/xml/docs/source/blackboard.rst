Blackboard
============

What is the Blackboard?
-----------------------
The blackboard acts as the central store for information, and is the backbone for communication in darbs.  Agents send commands to the blackboard requesting information, and instructing it to make changes to the information stored.

The commands take form of xml messages, which are described in more detail below.  To give a flavour of how communcation with the blackboard works, here is an example of two requests which the blackboard would understand.  You can try this out yourself using the :doc:`terminal` application::

    <!-- Adds information about the temperature to a partition named
         weather_info on the blackboard.  The blackboard responds with an xml
         message to confirm the information has been added -->
    <request type="add">
        <partition name="weather_info">
            <pattern>
                the temperature in Leicester is 5 degrees c
            </pattern>
        </partition>
    </request>

    <!-- Checks whether there is matching information on the weather_info
         partition.  The blackboard responds with a message to confirm that the
         information exists -->
    <request type="present">
        <partition name="weather_info">
            <pattern>
                the temperature is Leicester is ?temp degrees c
            </pattern>
        </partition>
    </request>

Notice how the information which is added here is written as a gramatically complete sentence.  This is typical of how DARBS is used, and is one of its great strengths, making DARBS applications very easily understood, and relatively simple to write, even for those with little or no programming experience.

How does it work?
-----------------
Internally, the blackboard is very simple, consisting of a single xml document.  The document is split into individual partitions, which can be thought of as a kind of organising structure to the content on the blackboard.  For example, if you were writing a program to predict weather patterns, you might have a partition for storing temperature data, another for storing air-pressure data, and so on.

When a pattern is added to a partition it is simply appended to the partition's xml node.  So for example::

    <request type="add">
        <partition name="weather_info">
            <pattern>
                the temperature in Leicester is 5 degrees c
            </pattern>
        </partition>
    </request>

    <request type="add">
        <partition name="weather_info">
            <pattern>
                the temperature in Northampton is 7 degrees c
            </pattern>
        </partition>
    </request> 

Would result in the following blackboard content::

    <blackboard>
        <partition name="weather_info">
            <pattern>
                the temperature in Leicester is 5 degrees c
            </pattern>
            <pattern>
                the temperature in Northampton is 7 degrees c
            </pattern>
        </partition>
    </blackboard>

Accessing and modifying information in the partition is therefore a simple matter of searching through the patterns until the correct piece of information is found, and making the appropriate changes to it.

When the data on a partition is changed a message is broadcast to all other connected clients, informing them of the change.

Multiple operations
-------------------

In the example above two separate requests were used to add patterns to the same partition.  This, and other similar requests, can be expressed in a more compact manner by nesting the patterns.  This means that only a single request has to be send to the blackboard, significantly reducing the amount of data which has to be sent and parsed.  For example, the following will add both temperature and pressure data to the blackboard in a single request::
    
    <request type="add">
        <partition name="temp_info">
            <pattern>
                The temperature in Leicester is 5 degrees c
            </pattern>
            <pattern>
                The temperature in Northampton is 7 degrees c
            </pattern>
        </partition>
        <partition name="pressure_info">
            <pattern>
                The pressure in Leicester is 1020 hPa
            </pattern>
        </partition>
    </request>

Wildcards
---------

You may have noticed in the first example that in checking whether a pattern exists on the partition we use a wildcard in our pattern::

    <pattern>
        the temperature in Leicester is ?temp degrees c
    </pattern>

This means that the blackboard would match either of the following:

* the temperature in Leicester is 7 degrees c
* the temperature in Leicester is foo degrees c

Notice how the wildcard implies nothing about the type of content it matches, only that it exists.

The ? character is not the only wildcard which can be used with the blackboard.  The following are all valid to use within patterns:

?name
    Matches a single word (no whitespace), and stores it with a name
??name
    Matches one or more words, and stores them with a name
==
    Matches one or more words, and discards them

The storing (and returning, using the get request) of wildcard matches is discussed more extensively in :doc:`agent`.

Additionally the ? wildcard can be used in some requests to search all partitions on the blackboard, rather than looking in a specific partition.  For example to delete all instances of a pattern from the blackboard the following request can be used::
    
    <!-- Delete all temperature data from the blackboard -->
    <request type="delete">
        <partition name="?">  <!-- Search all partitions -->
            <pattern>
                the temperature in ?place is ?temp
            </pattern>
        </partition>
    </request>

What requests does it understand?
---------------------------------

**add** - Adds patterns and partitions to the blackboard.  For example::

    <!-- Add an empty partition to the blackboard -->
    <request type="add">
        <partition name="weather_info" />
    </request>

    <!-- Add a pattern to a partition, creating the partition if necessary -->
    <request type="add">
        <partition name="weather_info">
            <pattern>
                the temperature in Leicester is 5 degrees c
            </pattern>
        </partition>
    </request>

**delete** - Deletes patterns or partitions from the blackboard.  For example::
    
    <!-- Delete a partition, and its contents from the blackboard -->
    <request type="delete">
        <partition name="weather_info" />
    </request>

    <!-- Delete all instances of a pattern from a partition -->
    <request type="delete">
        <partition name="delete">
            <pattern>
                the temperature in Leicester is 5 degrees c
            </pattern>
        </partition>
    </request>

**replace** - Replaces patterns on the blackboard.  For example::

    <!-- Replace all pieces of data about Leicester with data about Northampton
         on a partition -->
    <request type="replace">
        <partition name="weather_info">
            <!-- The pattern to find -->
            <pattern>
                the temperature in Leicester is 5 degrees c
            </pattern>
            <!-- The pattern to replace it with -->
            <pattern>
                the temperature in Northampton is 5 degrees c
            </pattern>
        </partition>
    </request>



**clear** - Clears all data from partitions, or clears the entire blackboard.  For example::

    <!-- Remove all patterns from a partition, leaving an empty partition -->
    <request type="clear">
        <partition name="weather_info" />
    </request>

    <!-- Remove all partitions and patterns from the blackboard -->
    <request type="clear" />

**present** - Checks whether a partition or pattern exists.  For example::

    <!-- Check whether a partition exists -->
    <request type="present">
        <partition name="weather_info" />
    </request>

    <!-- Check whether a pattern exists on a partition -->
    <request type="present">
        <partition name="weather_info">
            <pattern>
                the temperature in Leicester is 5 degrees c
            </pattern>
        </partition>
    </request>


**get** - Gets the contents of the blackboard or a partition, or returns matches to wildcards in a pattern.  For example::

    <!-- Return the entire contents of the blackboard -->
    <request type="get" />

    <!-- Returns the entire contents of a partition -->
    <request type="get">
        <partition name="weather_info" />
    </request>

    <!-- Returns all matches to the wildcards on a partition-->
    <request type="get">
        <partition name="weather_info">
            <pattern>
                The temperature in Leicester is ?temp degrees c
            </pattern>
        </partition>
    </request>


