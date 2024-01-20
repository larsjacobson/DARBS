import logging

from pylons import request, response, session, tmpl_context as c
from pylons.controllers.util import abort, redirect

from gui.lib.base import BaseController, render

from pylons import app_globals

log = logging.getLogger(__name__)

class KsController(BaseController):

    def index(self):
        blackboard = app_globals.darbs.blackboard
        c.kss = []
        partition = blackboard.get_partition("_ks")
        if partition:
            ks_names = [x.text for x in partition]
            for ks_name in ks_names:
                info_partition_name = "_ks_" + ks_name + "_info"
                info_partition = blackboard.get_partition(info_partition_name)
                info = [x.text for x in info_partition]
                state = 0
                for item in info:
                    if item.find("State is") != -1:
                        state = int(item[-1])
                        info.remove(item)
                state = ["Running", "Waiting", "Stopped"][state]
                c.kss.append({"name":ks_name, "info":info, "state":state,
                              "is_local" : app_globals.darbs.is_local(ks_name)})
        return render('/ks.mako')

    def add(self):
        ks_file = request.params["ks"]
        app_globals.darbs.add_ks(ks_file)

    def start(self, id):
        blackboard = app_globals.darbs.blackboard
        partition_name = "_ks_" + id + "_info"
        partition = blackboard.get_partition(partition_name)
        state = ""
        for pattern in partition:
            if pattern.text.strip().find("State is") != -1:
                state = pattern.text
                break
        if state:
            blackboard.edit_pattern(state, "State is 0", partition_name)

    def stop(self, id):
        blackboard = app_globals.darbs.blackboard
        partition_name = "_ks_" + id + "_info"
        partition = blackboard.get_partition(partition_name)
        state = ""
        for pattern in partition:
            if pattern.text.strip().find("State is") != -1:
                state = pattern.text
                break
        if state:
            blackboard.edit_pattern(state, "State is 2", partition_name)

    def output(self, id):
        out = app_globals.darbs.get_log(id)
        out = out.split("\n")
        c.log = out
        return render('/log.mako')
