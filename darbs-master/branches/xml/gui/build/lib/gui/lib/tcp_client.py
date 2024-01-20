import asynchat
import asyncore
import socket
import threading
import xml.etree.ElementTree

class TcpClient(asynchat.async_chat):
    def __init__(self, host="127.0.0.1", port=9734):
        asynchat.async_chat.__init__(self)

        self.set_terminator(chr(23))
        self.data = ""

        self.request_queue = []
        self.request = None
        
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connect((host, port))

        self.notification_callback = None

    def collect_incoming_data(self, data):
        self.data += data

    def found_terminator(self):
        response = xml.etree.ElementTree.fromstring(self.data[:-1])
        if response.tag == "response":
            self.request["callback"](response, *self.request["args"],
                                     **self.request["kwargs"])
            self.request = None
            self.do_send()
        elif response.tag == "notification":
            if self.notification_callback:
                self.notification_callback(response)
        self.data = ""

    def send(self, request, callback, *args, **kwargs):
        self.request_queue.append({"request": request, "callback": callback,
                                   "args": args, "kwargs": kwargs})
        self.do_send()

    def do_send(self):
        if not self.request and self.request_queue:
            self.request = self.request_queue.pop()
            asynchat.async_chat.send(self, self.request["request"] + chr(23))

    def add_callback(self, callback):
        self.notification_callback = callback

class AsyncoreThread(threading.Thread):
    def __init__(self, timeout=30.0, use_poll=False, map=None):
        super(AsyncoreThread, self).__init__()
        self.exit_flag = False
        self.timeout = timeout
        self.use_poll = use_poll
        self.map = map
        
    def run(self):
        self.loop()

    def loop(self):
        if self.map is None:
            self.map = asyncore.socket_map

        while self.map and not self.exit_flag:
            asyncore.loop(self.timeout, self.use_poll, self.map, 1)
            
    def end(self):
        self.exit_flag = True
    

if __name__ == "__main__":
    import asyncore
    client = TcpClient()
    asyncore.loop()
        
