# -*- coding:utf-8 -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
STOP_RENDERING = runtime.STOP_RENDERING
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 10
_modified_time = 1560126563.13
_enable_loop = True
_template_filename = u'C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/base.mako'
_template_uri = u'base.mako'
_source_encoding = 'utf-8'
from webhelpers.html import escape
_exports = []


def render_body(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_locals = __M_dict_builtin(pageargs=pageargs)
        h = context.get('h', UNDEFINED)
        self = context.get('self', UNDEFINED)
        next = context.get('next', UNDEFINED)
        __M_writer = context.writer()
        __M_writer(u'<!DOCTYPE html>\n<html>\n    <head>\n        <meta charset="utf-8" />\n        <title>DARBS</title>\n')
        __M_writer(u'        <!--\n        ')
        __M_writer(escape(h.stylesheet_link('/css/960/reset.css')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.stylesheet_link('/css/960/960.css')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.stylesheet_link('/css/960/text.css')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.javascript_link('/js/jquery-1.4.2.min.js')))
        __M_writer(u'\n        -->\n\n')
        __M_writer(u'        ')
        __M_writer(escape(h.stylesheet_link('/css/960/uncompressed/reset.css')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.stylesheet_link('/css/960/uncompressed/960.css')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.stylesheet_link('/css/960/uncompressed/text.css')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.javascript_link('/js/jquery-1.4.2.js')))
        __M_writer(u'\n\n')
        __M_writer(u'        ')
        __M_writer(escape(h.stylesheet_link('/css/main.css')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.stylesheet_link('/css/blitzer/jquery-ui-1.8.1.custom.css')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.javascript_link('/js/jquery.periodicalupdater.js')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.javascript_link('/js/jquery-ui-1.8.1.custom.min.js')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.javascript_link('/js/jquery.form.js')))
        __M_writer(u'\n        ')
        __M_writer(escape(h.javascript_link('/js/jquery.truncatable.js')))
        __M_writer(u'\n\n\n')
        __M_writer(u'        ')
        __M_writer(escape(self.head()))
        __M_writer(u'\n    </head>\n    <body>\n        <div class="container_12">\n            <div class="grid_12 header">\n                <h1>DARBS</h1>\n            </div>\n            ')
        __M_writer(escape(next.body()))
        __M_writer(u'\n        </div>\n    </body>\n</html>\n\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


"""
__M_BEGIN_METADATA
{"source_encoding": "utf-8", "line_map": {"17": 0, "25": 1, "26": 7, "27": 8, "28": 8, "29": 9, "30": 9, "31": 10, "32": 10, "33": 11, "34": 11, "35": 15, "36": 15, "37": 15, "38": 16, "39": 16, "40": 17, "41": 17, "42": 18, "43": 18, "44": 21, "45": 21, "46": 21, "47": 22, "48": 22, "49": 23, "50": 23, "51": 24, "52": 24, "53": 25, "54": 25, "55": 26, "56": 26, "57": 30, "58": 30, "59": 30, "60": 37, "61": 37, "67": 61}, "uri": "base.mako", "filename": "C:/Users/genna/OneDrive/Desktop/darbs/branches/xml/gui/gui/templates/base.mako"}
__M_END_METADATA
"""
