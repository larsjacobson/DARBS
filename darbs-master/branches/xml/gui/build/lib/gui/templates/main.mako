<%inherit file="base.mako"/>
<%def name="head()">
    ${h.javascript_link('/js/main.js')}
</%def>

<div class="grid_4">
    <h2>
        Agents
    </h2>
    <div id="ks">
        &nbsp;
    </div>
    <div class="align-center">
        <a href="#" class="add-ks">
            <img src="/img/list-add.png" alt="Add partition">
        </a>
    </div>
</div>
<div class="grid_4">
    <h2>
        Partitions
    </h2>
    <div id="blackboard">
        &nbsp;
    </div>
    <div class="align-center">
        <a href="#" class="add-partition">
            <img src="/img/list-add.png" alt="Add partition">
        </a>
    </div>
</div>
<div class="grid_4">
    <h2>
        Contents
    </h2>
    <div id="contents">
        &nbsp;
    </div>
    <div class="align-center">
        <a href="#" class="add-pattern">
            <img src="/img/list-add.png" alt="Add pattern">
        </a>
    </div>
</div>

<div id="add-partition-dialog">
    <div class="align-center">
        <form id="add-partition-form" method="post" action="/partition/add">
            <input type="text" name="name" />
            <br />
            <input type="submit" value="Add" />
        </form>
    </div>
</div>

<div id="add-pattern-dialog">
    <div class="align-center">
        <form id="add-pattern-form" method="post" action="/pattern/add">
            <input type="hidden" name="partition" />
            <input type="text" name="pattern" />
            <br />
            <input type="submit" value="Add" />
        </form>
    </div>
</div>

<div id="edit-pattern-dialog">
    <div class="align-center">
        <form id="edit-pattern-form" method="post" action="/pattern/edit">
            <input type="hidden" name="partition" />
            <input type="hidden" name="old-pattern" />
            <input type="text" name="new-pattern" />
            <br />
            <input type="submit" value="Edit" />
        </form>
    </div>
</div>

<div id="add-ks-dialog">
    <div class="align-center">
        % if c.kss:
        <form id="add-ks-form" method="post" action="/ks/add">
            <select name="ks">
            % for ks in c.kss:
                <option value="${ks["path"]}">${ks["name"]}</option>
            % endfor
            </select>
            <br />
            <input type="submit" value="Add" />
        </form>
        % else:
            <p>
                <strong>
                    No agents in the lib/agent folder
                </strong>
            </p>
        % endif
    </div>
</div>

<div id="view-log-dialog" />

