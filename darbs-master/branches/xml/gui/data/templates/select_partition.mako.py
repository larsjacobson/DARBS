# -*- coding:utf-8 -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
STOP_RENDERING = runtime.STOP_RENDERING
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 10
_modified_time = 1560126563.815
_enable_loop = True
_template_filename = 'C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/select_partition.mako'
_template_uri = '/select_partition.mako'
_source_encoding = 'utf-8'
from webhelpers.html import escape
_exports = []


def render_body(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_locals = __M_dict_builtin(pageargs=pageargs)
        __M_writer = context.writer()
        __M_writer(u'<div class="empty-notice">\n    <p>\n        <strong>\n            Select a partition to view \n        </strong>\n    </p>\n</div>\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


"""
__M_BEGIN_METADATA
{"source_encoding": "utf-8", "line_map": {"17": 0, "28": 22, "22": 1}, "uri": "/select_partition.mako", "filename": "C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/select_partition.mako"}
__M_END_METADATA
"""
