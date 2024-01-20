# -*- coding:utf-8 -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
STOP_RENDERING = runtime.STOP_RENDERING
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 10
_modified_time = 1560126563.094
_enable_loop = True
_template_filename = 'C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/main.mako'
_template_uri = 'main.mako'
_source_encoding = 'utf-8'
from webhelpers.html import escape
_exports = ['head']


def _mako_get_namespace(context, name):
    try:
        return context.namespaces[(__name__, name)]
    except KeyError:
        _mako_generate_namespaces(context)
        return context.namespaces[(__name__, name)]
def _mako_generate_namespaces(context):
    pass
def _mako_inherit(template, context):
    _mako_generate_namespaces(context)
    return runtime._inherit_from(context, u'base.mako', _template_uri)
def render_body(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_locals = __M_dict_builtin(pageargs=pageargs)
        c = context.get('c', UNDEFINED)
        __M_writer = context.writer()
        __M_writer(u'\n')
        __M_writer(u'\n\n<div class="grid_4">\n    <h2>\n        Agents\n    </h2>\n    <div id="ks">\n        &nbsp;\n    </div>\n    <div class="align-center">\n        <a href="#" class="add-ks">\n            <img src="/img/list-add.png" alt="Add partition">\n        </a>\n    </div>\n</div>\n<div class="grid_4">\n    <h2>\n        Partitions\n    </h2>\n    <div id="blackboard">\n        &nbsp;\n    </div>\n    <div class="align-center">\n        <a href="#" class="add-partition">\n            <img src="/img/list-add.png" alt="Add partition">\n        </a>\n    </div>\n</div>\n<div class="grid_4">\n    <h2>\n        Contents\n    </h2>\n    <div id="contents">\n        &nbsp;\n    </div>\n    <div class="align-center">\n        <a href="#" class="add-pattern">\n            <img src="/img/list-add.png" alt="Add pattern">\n        </a>\n    </div>\n</div>\n\n<div id="add-partition-dialog">\n    <div class="align-center">\n        <form id="add-partition-form" method="post" action="/partition/add">\n            <input type="text" name="name" />\n            <br />\n            <input type="submit" value="Add" />\n        </form>\n    </div>\n</div>\n\n<div id="add-pattern-dialog">\n    <div class="align-center">\n        <form id="add-pattern-form" method="post" action="/pattern/add">\n            <input type="hidden" name="partition" />\n            <input type="text" name="pattern" />\n            <br />\n            <input type="submit" value="Add" />\n        </form>\n    </div>\n</div>\n\n<div id="edit-pattern-dialog">\n    <div class="align-center">\n        <form id="edit-pattern-form" method="post" action="/pattern/edit">\n            <input type="hidden" name="partition" />\n            <input type="hidden" name="old-pattern" />\n            <input type="text" name="new-pattern" />\n            <br />\n            <input type="submit" value="Edit" />\n        </form>\n    </div>\n</div>\n\n<div id="add-ks-dialog">\n    <div class="align-center">\n')
        if c.kss:
            __M_writer(u'        <form id="add-ks-form" method="post" action="/ks/add">\n            <select name="ks">\n')
            for ks in c.kss:
                __M_writer(u'                <option value="')
                __M_writer(escape(ks["path"]))
                __M_writer(u'">')
                __M_writer(escape(ks["name"]))
                __M_writer(u'</option>\n')
            __M_writer(u'            </select>\n            <br />\n            <input type="submit" value="Add" />\n        </form>\n')
        else:
            __M_writer(u'            <p>\n                <strong>\n                    No agents in the lib/agent folder\n                </strong>\n            </p>\n')
        __M_writer(u'    </div>\n</div>\n\n<div id="view-log-dialog" />\n\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_head(context):
    __M_caller = context.caller_stack._push_frame()
    try:
        h = context.get('h', UNDEFINED)
        __M_writer = context.writer()
        __M_writer(u'\n    ')
        __M_writer(escape(h.javascript_link('/js/main.js')))
        __M_writer(u'\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


"""
__M_BEGIN_METADATA
{"source_encoding": "utf-8", "line_map": {"34": 1, "35": 4, "36": 81, "37": 82, "38": 84, "39": 85, "40": 85, "41": 85, "42": 85, "43": 85, "44": 87, "45": 91, "46": 92, "47": 98, "66": 60, "53": 2, "58": 2, "59": 3, "28": 0, "60": 3}, "uri": "main.mako", "filename": "C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/main.mako"}
__M_END_METADATA
"""
