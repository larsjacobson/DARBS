from gui.tests import *

class TestPartitionController(TestController):

    def test_index(self):
        response = self.app.get(url(controller='partition', action='index'))
        # Test response...
