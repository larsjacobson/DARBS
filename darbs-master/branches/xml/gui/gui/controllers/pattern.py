import logging

from pylons import request, response, session, tmpl_context as c
from pylons.controllers.util import abort, redirect

from gui.lib.base import BaseController, render

from pylons import app_globals

log = logging.getLogger(__name__)

class PatternController(BaseController):

    def index(self):
        # Return a rendered template
        #return render('/pattern.mako')
        # or, return a response
        return 'Hello World'

    def add(self):
        app_globals.darbs.blackboard.add_pattern(request.params["pattern"],
                                                 request.params["partition"])

    def delete(self):
        app_globals.darbs.blackboard.delete_pattern(request.params["pattern"],
                                                    request.params["partition"])

    def edit(self):
        app_globals.darbs.blackboard.edit_pattern(request.params["old-pattern"],
                                                  request.params["new-pattern"],
                                                  request.params["partition"])
