from gui.tests import *

class TestBlackboardController(TestController):

    def test_index(self):
        response = self.app.get(url(controller='blackboard', action='index'))
        # Test response...
