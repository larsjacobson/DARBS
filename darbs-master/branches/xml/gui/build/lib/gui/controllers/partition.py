import logging

from pylons import request, response, session, tmpl_context as c
from pylons.controllers.util import abort, redirect

from gui.lib.base import BaseController, render

from pylons import app_globals

log = logging.getLogger(__name__)



class PartitionController(BaseController):

    def index(self, id):
       partition = app_globals.darbs.blackboard.get_partition(id)
       # Create list of patterns for display
       c.patterns = []
       for pattern in partition:
           c.patterns.append(pattern.text)
       c.partition_name = id
       c.revision = app_globals.darbs.blackboard.get_revision(id)
       return render('/partition.mako') 

    def delete(self, id):
        app_globals.darbs.blackboard.delete_partition(id)
        return "Done"

    def add(self):
        app_globals.darbs.blackboard.add_partition(request.params["name"])

    def select(self):
        return render('/select_partition.mako')
