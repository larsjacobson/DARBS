Sample Rule
===========

::

    RULE CheckRectArea
    IF [
        [on_partition [the size of ?rect is ?size] line_info]
        AND
        [compare [~size GreaterThanOrEqual 200]
    ]
    THEN [
        [add [the rectangle ~rect covers a large area] line_info]
        AND
        [run [show_area [~rect]] returned_value]
    ]
    BECAUSE [
        [the rectangle ~rect covers a large area]
    ]
    END


