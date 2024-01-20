Knowledge Sources
=================

What are Knowledge Sources?
---------------------------
Knowledge Sources are the brains behind a DARBS system.  They are responsible for adding to, removing from and interpreting data on the blackboard.  Each Knowledge Source (KS) is specialized to tackle a small part of the overall problem, rather than trying to tackle the whole thing.  As a result there are a number of different type of KS available in DARBS, and new types can be added to provide additional functionality.

The simplest type of Knowledge Source consists simply of a text file containing a number of compulsory fields (plus optional extra fields depending on the type of KS).  The basic information contained is:

* The name fo the KS
* The type of KS
* Whether the KS is activated
* Preconditions to determine whether the KS should run
* Actions to execute if the preconditions are met

You can see this basic structure in the following example::

    KS Square /* The name of the KS */
    KS_TYPE procedural_KS /* The KS type */
    INFERENCE_MODE MI_Forwardchain /* The inference mode */
    FIRABILITY_FLAG true /* Whether the KS is activated */
    IF
    [  
        [not_on_partition [Square is fired] ControlChars] /* The precondition */
    ]
    THEN
    [
        /* The actions */
        [run_algorithm[libsquare.so square[2.34] result]]
        AND
        [add [Square is fired] ControlChars]
        AND
        [report [Square of 2.34 is ~result]]
    ]
    END

How do Knowledge Sources work?
------------------------------
Knowledge Sources are fairly simple.  First the activation flag is checked, to see whether the KS should be run.  If this is true, then the precondition is evaluated.  The precondition is made up of any combination of queries to the :doc:`blackboard` and additional KS commands, which return true or false.  You may have noticed in the example above that the syntax exactly follows that of blackboard commands.  If the precondition is found to be true then the actions are executed.  These can be either commands to send to the blackboard, or KS commands.

When any necessary actions have been taken, the KS returns to the precondition, continually evaluating it until it is true, and further action needs to be taken.


What types of Knowledge Source are available?
---------------------------------------------

DARBS currently has two basic types of Knowledge Source available.  These are:

* :doc:`rule_based` - Produces knowledge by following a number of simple rules
* :doc:`procedural` - Produces knowledge by running external algorithms

Knowledge Source Commands
-------------------------
**compare** *[value operator value]* - Compares two values.  Operator can be Equal, GreaterThan, LessThan, GreaterThanOrEqual, LessThanOrEqual, NotEqual.  For example the following will return true::
    
    compare [0.25 GreaterThanOrEqual 0.1]

**run_algorithm** *[library_name procedure_name [in_parameter in_parameter...] out_parameter]* - Runs a function contained within an external shared library.  For example to run a function square in the library libsquare.so with a single argument::
    
    run_algorithm [libsquare.so square [2.34] outval]

**report** *[pattern]* - Prints a string to the command line.  For example::
    
    report [Knowledge Source has executed]

