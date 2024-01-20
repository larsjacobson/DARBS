<div id="${c.partition_name}-patterns" class="revision-${c.revision}">
    % for pattern in c.patterns:
    <div class="pattern">
        <div class="grid_3 alpha">
            <div>
                ## Don't indent so we don't truncate unnecessarily
                <p class="contents">${pattern.strip()}</p>
            </div>
        </div>
        <div class="grid_1 omega">
            <div class="align-right">
                <a href="#" class="delete-pattern">
                    <img src="/img/edit-delete.png" alt="Delete pattern" />
                </a>
                <a href="#" class="edit-pattern">
                    <img src="/img/accessories-text-editor.png" alt="Edit pattern" />
                </a>
            </div>
        </div>
        <div class="clearfix">
            &nbsp;
        </div>
    </div>
    % endfor


    % if not c.patterns:
    <div class="empty-notice">
        <p>
            <strong>
                No patterns on partition
            </strong>
        </p>
    </div>
    % endif
</div>
