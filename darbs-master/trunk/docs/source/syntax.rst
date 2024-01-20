Rule Syntax
===========

*rule*::=       **RULE** *rule_name* **IF** *condition* **THEN** *conclusion* **BECAUSE** *reason* **END**


*rule_name*::=  <string>

*condition*::=  [*condition* **AND** *condition*]
                [*condition* **OR** *condition*]
                [**on_partition** *statement* *partition*]
                [**not_on_partition** *statement* *partition*]
                [**on_blackboard** *statement*]
                [**not_on_blackboard** *statement*]
                [**partition_exist** *partition*]
                [**partition_not_exist** *partition*]
                [**compare** [*operand* *operator* *operand*]]

*conclusion*::= [*conclusion* **AND** *conclusion*]
                [**add statement** *partition*]
                [**del_first** *statement* *partition*]
                [**del_all** *statement* *partition*]
                [**run_algorithm** [<library_name> <procedure_name> [input_parameters] output_parameter]]
                [**add_partitions** [*partition* *partition* ...]]
                [**clr_partition** *partition*]
                [**clr_board**]
                [**report** [statement]]

*reason*::=     statement

*partition*::=  <partition_name> 

*statement*::=  ~<variable_name>
                ?<variable_name>
                ~~<list_name>
                ??<list_name>
                <string>
                <constant>
                <a list containing any of the above>

*operand*::=    **Equal**
                **GreaterThan**
                **LessThan**
                **GreaterThanOrEqual**
                **LessThanOrEqual**
                **NotEqual**

