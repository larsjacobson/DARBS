import logging

from pylons import request, response, session, tmpl_context as c
from pylons.controllers.util import abort, redirect

from gui.lib.base import BaseController, render

import gui.lib.helpers as h

import sys
import os
import os.path

log = logging.getLogger(__name__)

class MainController(BaseController):

    def index(self):
        c.kss = []
        ks_path = os.path.join(sys.path[0], "gui", "lib", "ks")
        ks_paths = find_kss(ks_path)
        for path in ks_paths:
            name = os.path.split(path)[-1][:-4]
            c.kss.append({"path":path, "name":name})
        return render("main.mako")

def find_kss(dir):
    kss = []
    paths = os.listdir(dir)
    for path in paths:
        path = os.path.join(dir, path)
        if os.path.isfile(path):
            if path[-3:] == "dkf":
                kss.append(path)
        elif os.path.isdir(path):
            kss.extend(find_kss(path))
    return kss
