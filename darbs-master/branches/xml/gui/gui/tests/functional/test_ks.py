from gui.tests import *

class TestKsController(TestController):

    def test_index(self):
        response = self.app.get(url(controller='ks', action='index'))
        # Test response...
