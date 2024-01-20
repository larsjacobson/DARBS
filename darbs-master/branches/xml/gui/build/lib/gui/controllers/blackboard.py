import logging

from pylons import request, response, session, tmpl_context as c
from pylons.controllers.util import abort, redirect

from gui.lib.base import BaseController, render

from pylons import app_globals

log = logging.getLogger(__name__)

class BlackboardController(BaseController):

    def index(self):
        c.partitions = []
        blackboard = app_globals.darbs.blackboard
        partition_names = blackboard.get_partition_names()
        for partition_name in partition_names:
            if partition_name[0] != "_":
                np = blackboard.get_num_patterns(partition_name)
                revision = blackboard.get_revision(partition_name)
                c.partitions.append({"name":partition_name, "num_patterns": np,
                                     "revision":revision})
        return render('/blackboard.mako')

