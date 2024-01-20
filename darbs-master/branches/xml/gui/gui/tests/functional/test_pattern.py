from gui.tests import *

class TestPatternController(TestController):

    def test_index(self):
        response = self.app.get(url(controller='pattern', action='index'))
        # Test response...
