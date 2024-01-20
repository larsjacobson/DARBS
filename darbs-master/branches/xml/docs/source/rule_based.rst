Rule Based agent
===========================

Rule Based agents are :doc:`agent` which contain a list of rules which are evaluated in order if the precondition is met.  The rules themselves are almost identical in structure to agents, consisting of a preconditions and actions.

Structure of a rule based agent
----------------------------

The following example shows the basic structure of a rule based agent containing three rules::

    <agent name="type="rule-based">
        <precondition type="not">
            <request type="present">
                <partition name="control">
                    <pattern>
                        Rule based agent is fired
                    </pattern>
                </partition>
            </request>
        </precondition>

        <rule>Rule1.drf</rule>
        <rule>Rule2.drf</rule>
        <rule>Rule3.drf</rule>

        <action>
            <request type="add">
                <partition name="control">
                    <pattern>
                        Rule based agent is fired
                    </pattern>
                </partition>
            </request>
        </action>
    </agent>

How do rule based agents work?
-----------------------------------------

Rule based agents work in the same way as any other agent, with an additional stage between the evaluation of the precondition and actions.  Firstly the precondition is evaluated.  If it is found to be true then the rules (which are contained in .drf files located in the same folder as the agent) are evaluated in order.  No matter what the outcome of each rule is, the agent will continue to work through the rules one-by-one until each rule has been evaluated.  At this point the actions are evaluated, and the process begins again.
