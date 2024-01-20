% for partition in c.partitions:
<div class="partition revision-${partition["revision"]}" id="${partition["name"]}">
    <div class="grid_3 alpha">
        <div>
            <h4>
                ${partition["name"]}
            </h4>
            <p>
                ${partition["num_patterns"]} patterns contained
            </p>
        </div>
    </div>
    <div class="grid_1 omega">
        <div class="align-right">
            <a href="#" class="delete-partition" name="${partition["name"]}">
                <img src="/img/edit-delete.png" alt="Delete partition" />
            </a>
        </div>
    </div>
    <div class="clearfix">
        &nbsp;
    </div>
</div>
% endfor

% if not c.partitions:
<div class="empty-notice">
    <p>
        <strong>
            No partitions on the Blackboard
        </strong>
    </p>
</div>
% endif

