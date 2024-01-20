% for ks in c.kss:
<div class="ks" id="${ks["name"]}">
    <div class="grid_3 alpha">
        <div>
            <h4>
                ${ks["name"]}
            </h4>
            <p>
                <strong>
                    ${ks["state"]}
                </strong>
            </p>
            % for info in ks["info"]:
            <p>
                ${info}
            </p>
            % endfor
        </div>
    </div>
    <div class="grid_1 omega">
        <div class="align-right">
            <a href="#" class="start-ks">
                <img src="/img/media-playback-start.png" alt="Start agent" />
            </a>
            <a href="#" class="stop-ks">
                <img src="/img/media-playback-stop.png" alt="Stop agent" />
            </a>
            % if ks["is_local"]:
            <a href="#" class="view-log">
                <img src="/img/utilities-system-monitor.png" alt="View log" />
            </a>
            % endif
        </div>
    </div>
    <div class="clearfix">
        &nbsp;
    </div>
</div>
% endfor

% if not c.kss:
<div class="empty-notice">
    <p>
        <strong>
            No agents running
        </strong>
    </p>
</div>
% endif

