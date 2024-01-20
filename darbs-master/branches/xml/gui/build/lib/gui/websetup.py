"""Setup the gui application"""
import logging

from gui.config.environment import load_environment

log = logging.getLogger(__name__)

def setup_app(command, conf, vars):
    """Place any commands to setup gui here"""
    if not pylons.test.pylonsapp:
        load_environment(conf.global_conf, conf.local_conf)
