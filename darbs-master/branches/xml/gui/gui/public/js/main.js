$(document).ready(function() {
    // Load the select partition notice
    $("div#contents").load("/partition/select");

    // Hide the add pattern button
    $("a.add-pattern").hide();
    
    // Update the blackboard view
    $.PeriodicalUpdater('/blackboard/index',
        {minTimeout: 500, maxTimeout: 5000, multiplier: 2},
        function(data) {
            $("div#blackboard").empty();
            $("div#blackboard").append(data);
            // Set the active class from grabbing the active partition
            var partitionName = $("div#contents div").attr("id");
            if (partitionName) {
                // Remove -patterns suffix
                var end = partitionName.indexOf("-patterns");
                partitionName = partitionName.slice(0, end);
                // Check whether the partition exists
                if ($("div#" + partitionName).length) {
                    $("div#" + partitionName).addClass("active");
                    // Check whether the content div needs updating
                    var revision = $("div#contents div").attr("class");
                    if (!$("div#" + partitionName + "." + revision).length) {
                        var url = '/partition/index/' + partitionName;
                        $("div#contents").load(url, function() {
                            $("div.pattern .contents").truncatable({less:true});
                        });
                    }   
                }
                else {
                    // Must have been deleted, so empty the contents div
                    $("a.add-pattern").fadeOut();
                    $("div#contents").fadeOut("default", function() {
                        $(this).load("/partition/select", function() {
                            $(this).fadeIn();
                        });
                    });
                }
            }
        }
    );

    // When a partition is clicked on, load the contents
    $("div.partition").live('click', function() {
        var partition = $(this);
        $("div.partition").removeClass("active");
        $(this).addClass("active");
        $("div#contents").fadeOut("default", function() {
            $("div#contents").load('/partition/index/'+partition.attr("id"),
                function() {
                    $("form#add-pattern-form input[name=\"partition\"]").val(partition.attr("id"));
                    $("form#edit-pattern-form input[name=\"partition\"]").val(partition.attr("id"));

                    $("div.pattern .contents").truncatable({less: true});
                    
                    $("a.add-pattern").fadeIn();
                    $("div#contents").fadeIn();
                }
            );
        });
    });

    // Hover over partitions
    $("div.partition").live('mouseover mouseout', function(event) {
        if (event.type == 'mouseover') {
            $(this).addClass("hover");
        }
        else {
            $(this).removeClass("hover");
        }
    });

    // Delete partition buttons
    $("a.delete-partition").live('click', function(event) {
        var link = $(this);
        $(this).parents("div.partition").fadeOut('default', function() {
            $.get("/partition/delete/" + link.attr("name"),
                  function(data) {
            });
        });
        // Stop div.partition click event firing
        return false;
    });

    // Add partition button
    $("div#add-partition-dialog").dialog({autoOpen: false, modal: true,
                                          title: "Add Partition"});
    $("a.add-partition").live('click', function() {
        $("div#add-partition-dialog").dialog('open');
    });

    // Add partition form
    $("form#add-partition-form").ajaxForm({beforeSubmit: function(formData, jqForm, options) {
        if (!jqForm[0].name.value) {

            return false;
        }
    }, success:function() {
        $("div#add-partition-dialog").dialog("close");
        $("form#add-partition-form").resetForm();
    }});

    // Add partition button
    $("div#add-pattern-dialog").dialog({autoOpen: false, modal: true,
                                          title: "Add Pattern"});
    $("a.add-pattern").live('click', function() {
        $("div#add-pattern-dialog").dialog('open');
    });
        
    // Add pattern form
    $("form#add-pattern-form").ajaxForm({beforeSubmit: function(formData, jqForm, options) {
        if (!jqForm[0].pattern.value) {
            return false;
        }
    }, success:function() {
        $("div#add-pattern-dialog").dialog("close");
        $("form#add-pattern-form").resetForm();
    }});

    // Delete pattern button
    $("a.delete-pattern").live('click', function() {
        $(this).parents("div.pattern").fadeOut("default", function() {
            var pattern = $(this).find("p.contents").text();
            var partitionName = $(this).parent().attr("id");
            var end = partitionName.indexOf("-patterns");
            partitionName = partitionName.slice(0, end);
            $.post("/pattern/delete", {"pattern" : pattern,
                                       "partition" : partitionName});
        });
    });



    // Edit pattern button
    $("div#edit-pattern-dialog").dialog({autoOpen: false, modal: true,
                                              title: "Edit Pattern"});
    $("a.edit-pattern").live("click", function() {
        var pattern = $(this).parents("div.pattern").find("p.contents").text();
        pattern = $.trim(pattern);
        // Set new-pattern and old-pattern value to the existing value
        $("div#edit-pattern-dialog input[name$=\"pattern\"]").val(pattern);
        $("div#edit-pattern-dialog").dialog('open');
    });

    // Edit pattern form
    $("form#edit-pattern-form").ajaxForm({beforeSubmit: function(formData, jqForm, options) {
        if (!jqForm[0]["new-pattern"].value ||
            $.trim(jqForm[0]["new-pattern"].value) ==
            $.trim(jqForm[0]["old-pattern"].value)) {
            return false;
        }
    }, success:function() {
        $("div#edit-pattern-dialog").dialog("close");
        $("form#edit-pattern-form").resetForm();
    }});


    // Update the ks view
    $.PeriodicalUpdater('/ks/index',
        {minTimeout: 500, maxTimeout: 5000, multiplier: 2},
        function(data) {
            $("div#ks").empty();
            $("div#ks").append(data);
        }
    );

    // Add ks button
    $("div#add-ks-dialog").dialog({autoOpen: false, modal: true,
                                   title: "Add Agent"});
    $("a.add-ks").live("click", function() {
        $("div#add-ks-dialog").dialog('open');
    });

    // Add KS form
    $("form#add-ks-form").ajaxForm({success:function() {
        $("div#add-ks-dialog").dialog("close");
        $("form#add-ks-form").resetForm();
    }});

    // Start KS button
    $("a.start-ks").live("click", function() {
        var ks_name = $(this).parents("div.ks").attr("id");
        $.get("/ks/start/" + ks_name);
    });

    // Stop KS button
    $("a.stop-ks").live("click", function() {
        var ks_name = $(this).parents("div.ks").attr("id");
        $.get("/ks/stop/" + ks_name);
    });
            


    // View log button
    $("div#view-log-dialog").dialog({autoOpen: false, modal: true,
                                     width: 640, height: 480});

    $("a.view-log").live("click", function() {
        var ks_name = $(this).parents("div.ks").attr("id");
        $("div#view-log-dialog").load("/ks/output/" + ks_name, function() {
            $("div#view-log-dialog").dialog("option", "title",
                                            ks_name + " Log");
            $("div#view-log-dialog").dialog("open");
        })
    });

});

