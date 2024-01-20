# -*- coding:utf-8 -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
STOP_RENDERING = runtime.STOP_RENDERING
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 10
_modified_time = 1560127292.103
_enable_loop = True
_template_filename = 'C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/partition.mako'
_template_uri = '/partition.mako'
_source_encoding = 'utf-8'
from webhelpers.html import escape
_exports = []


def render_body(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_locals = __M_dict_builtin(pageargs=pageargs)
        c = context.get('c', UNDEFINED)
        __M_writer = context.writer()
        __M_writer(u'<div id="')
        __M_writer(escape(c.partition_name))
        __M_writer(u'-patterns" class="revision-')
        __M_writer(escape(c.revision))
        __M_writer(u'">\n')
        for pattern in c.patterns:
            __M_writer(u'    <div class="pattern">\n        <div class="grid_3 alpha">\n            <div>\n')
            __M_writer(u'                <p class="contents">')
            __M_writer(escape(pattern.strip()))
            __M_writer(u'</p>\n            </div>\n        </div>\n        <div class="grid_1 omega">\n            <div class="align-right">\n                <a href="#" class="delete-pattern">\n                    <img src="/img/edit-delete.png" alt="Delete pattern" />\n                </a>\n                <a href="#" class="edit-pattern">\n                    <img src="/img/accessories-text-editor.png" alt="Edit pattern" />\n                </a>\n            </div>\n        </div>\n        <div class="clearfix">\n            &nbsp;\n        </div>\n    </div>\n')
        __M_writer(u'\n\n')
        if not c.patterns:
            __M_writer(u'    <div class="empty-notice">\n        <p>\n            <strong>\n                No patterns on partition\n            </strong>\n        </p>\n    </div>\n')
        __M_writer(u'</div>\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


"""
__M_BEGIN_METADATA
{"source_encoding": "utf-8", "line_map": {"32": 7, "33": 25, "34": 27, "35": 28, "36": 36, "42": 36, "17": 0, "23": 1, "24": 1, "25": 1, "26": 1, "27": 1, "28": 2, "29": 3, "30": 7, "31": 7}, "uri": "/partition.mako", "filename": "C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/partition.mako"}
__M_END_METADATA
"""
