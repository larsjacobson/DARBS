# -*- coding:utf-8 -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
STOP_RENDERING = runtime.STOP_RENDERING
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 10
_modified_time = 1560126563.955
_enable_loop = True
_template_filename = 'C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/ks.mako'
_template_uri = '/ks.mako'
_source_encoding = 'utf-8'
from webhelpers.html import escape
_exports = []


def render_body(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_locals = __M_dict_builtin(pageargs=pageargs)
        c = context.get('c', UNDEFINED)
        __M_writer = context.writer()
        for ks in c.kss:
            __M_writer(u'<div class="ks" id="')
            __M_writer(escape(ks["name"]))
            __M_writer(u'">\n    <div class="grid_3 alpha">\n        <div>\n            <h4>\n                ')
            __M_writer(escape(ks["name"]))
            __M_writer(u'\n            </h4>\n            <p>\n                <strong>\n                    ')
            __M_writer(escape(ks["state"]))
            __M_writer(u'\n                </strong>\n            </p>\n')
            for info in ks["info"]:
                __M_writer(u'            <p>\n                ')
                __M_writer(escape(info))
                __M_writer(u'\n            </p>\n')
            __M_writer(u'        </div>\n    </div>\n    <div class="grid_1 omega">\n        <div class="align-right">\n            <a href="#" class="start-ks">\n                <img src="/img/media-playback-start.png" alt="Start agent" />\n            </a>\n            <a href="#" class="stop-ks">\n                <img src="/img/media-playback-stop.png" alt="Stop agent" />\n            </a>\n')
            if ks["is_local"]:
                __M_writer(u'            <a href="#" class="view-log">\n                <img src="/img/utilities-system-monitor.png" alt="View log" />\n            </a>\n')
            __M_writer(u'        </div>\n    </div>\n    <div class="clearfix">\n        &nbsp;\n    </div>\n</div>\n')
        __M_writer(u'\n')
        if not c.kss:
            __M_writer(u'<div class="empty-notice">\n    <p>\n        <strong>\n            No agents running\n        </strong>\n    </p>\n</div>\n')
        __M_writer(u'\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


"""
__M_BEGIN_METADATA
{"source_encoding": "utf-8", "line_map": {"17": 0, "23": 1, "24": 2, "25": 2, "26": 2, "27": 6, "28": 6, "29": 10, "30": 10, "31": 13, "32": 14, "33": 15, "34": 15, "35": 18, "36": 28, "37": 29, "38": 33, "39": 40, "40": 41, "41": 42, "42": 50, "48": 42}, "uri": "/ks.mako", "filename": "C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/ks.mako"}
__M_END_METADATA
"""
