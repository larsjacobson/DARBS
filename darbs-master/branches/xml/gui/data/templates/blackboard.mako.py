# -*- coding:utf-8 -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
STOP_RENDERING = runtime.STOP_RENDERING
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 10
_modified_time = 1560126563.917
_enable_loop = True
_template_filename = 'C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/blackboard.mako'
_template_uri = '/blackboard.mako'
_source_encoding = 'utf-8'
from webhelpers.html import escape
_exports = []


def render_body(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_locals = __M_dict_builtin(pageargs=pageargs)
        c = context.get('c', UNDEFINED)
        __M_writer = context.writer()
        for partition in c.partitions:
            __M_writer(u'<div class="partition revision-')
            __M_writer(escape(partition["revision"]))
            __M_writer(u'" id="')
            __M_writer(escape(partition["name"]))
            __M_writer(u'">\n    <div class="grid_3 alpha">\n        <div>\n            <h4>\n                ')
            __M_writer(escape(partition["name"]))
            __M_writer(u'\n            </h4>\n            <p>\n                ')
            __M_writer(escape(partition["num_patterns"]))
            __M_writer(u' patterns contained\n            </p>\n        </div>\n    </div>\n    <div class="grid_1 omega">\n        <div class="align-right">\n            <a href="#" class="delete-partition" name="')
            __M_writer(escape(partition["name"]))
            __M_writer(u'">\n                <img src="/img/edit-delete.png" alt="Delete partition" />\n            </a>\n        </div>\n    </div>\n    <div class="clearfix">\n        &nbsp;\n    </div>\n</div>\n')
        __M_writer(u'\n')
        if not c.partitions:
            __M_writer(u'<div class="empty-notice">\n    <p>\n        <strong>\n            No partitions on the Blackboard\n        </strong>\n    </p>\n</div>\n')
        __M_writer(u'\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


"""
__M_BEGIN_METADATA
{"source_encoding": "utf-8", "line_map": {"32": 9, "33": 15, "34": 15, "35": 25, "36": 26, "37": 27, "38": 35, "44": 38, "17": 0, "23": 1, "24": 2, "25": 2, "26": 2, "27": 2, "28": 2, "29": 6, "30": 6, "31": 9}, "uri": "/blackboard.mako", "filename": "C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/blackboard.mako"}
__M_END_METADATA
"""
