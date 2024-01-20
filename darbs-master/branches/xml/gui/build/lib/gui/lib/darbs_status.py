import tcp_client
import xml.etree.ElementTree
import threading
import copy
import subprocess
import os.path
import sys
import uuid

class DarbsStatus(object):
    def __init__(self):
        self.client = tcp_client.TcpClient()
        self.client.add_callback(self.notification_callback)

        self.blackboard = Blackboard(self.client)
        self.acquire_blackboard()

        self.lock = threading.RLock()
        self.kss = {}
        
    def acquire_blackboard(self):
        self.client.send("<request type=\"get\" />",
                         self.blackboard.set_blackboard)

    def acquire_partitions(self, partition_names):
        request = "<request type=\"get\">"
        for partition_name in partition_names:
            request += "<partition name=\"" + partition_name + "\" />"
        request += "</request>"
        self.client.send(request, self.blackboard.set_partitions) 

    def notification_callback(self, notification):
        partition_names = [partition.attrib["name"] for partition in notification]
        # We know which partitions have changed, so only get them
        self.acquire_partitions(partition_names)

    def add_ks(self, ks_file):
        ks_name = os.path.split(ks_file)[1][:-4]
        # Bit ugly - as Popen doesn't support non-blocking reading at the moment
        # we write to a file, and read back from that.  See PEP 3145.
        log_file_path = os.path.join(sys.path[0], "gui", "lib", "tmp",
                                     ks_name + str(uuid.uuid4()))
        log_file = open(log_file_path, "w")
        process = subprocess.Popen(["ksclient", ks_file, "-l", "3"],
                                   stdout=log_file,
                                   stderr=subprocess.STDOUT)
        self.lock.acquire()
        self.kss[ks_name] = {"file" : ks_file, "process" : process,
                             "log_file_path" : log_file_path,
                             "log_file" : log_file}
        self.lock.release()

    def get_log(self, ks_name):
        ks = self.kss[ks_name]
        log_file = open(ks["log_file_path"], "r") 
        log = log_file.read()
        log_file.close()
        return log

    def is_local(self, ks_name):
        self.lock.acquire()
        is_local = ks_name in self.kss
        self.lock.release()
        return is_local


class Blackboard:
    def __init__(self, client):
        self.blackboard = None
        self.lock = threading.RLock()
        self.client = client

    def set_blackboard(self, blackboard):
        blackboard.tag = "blackboard"
        self.lock.acquire()
        self.blackboard = blackboard
        for partition in self.blackboard:
            partition.attrib["revision"] = 0
        self.lock.release()

    def set_partitions(self, partitions):
        self.lock.acquire()
        for partition in partitions:
            # Try to find the partition with the same name, and remove it
            bb_partition = self.find_partition(partition.attrib["name"])

            revision = None

            if bb_partition != None:
                revision = bb_partition.attrib["revision"]
                self.blackboard.remove(bb_partition)

            # Append the new, correct version of the partition
            if partition.text != "false":
                self.blackboard.append(partition)
                if revision:
                    partition.attrib["revision"] = revision + 1
                else:
                    partition.attrib["revision"] = 0
        self.lock.release()

    def find_partition(self, partition_name):
        for partition in self.blackboard:
            if partition.attrib["name"] == partition_name:
                return partition
        return None

    def get_partition_names(self):
        self.lock.acquire()
        names = [partition.attrib["name"] for partition in self.blackboard]
        self.lock.release()
        return names

    def get_num_patterns(self, partition_name):
        self.lock.acquire()
        partition = self.find_partition(partition_name)
        num_patterns = len(partition)
        self.lock.release()
        return num_patterns

    def get_revision(self, partition_name):
        self.lock.acquire()
        partition = self.find_partition(partition_name)
        revision = partition.attrib["revision"]
        self.lock.release()
        return revision

    def get_partition(self, partition_name):
        self.lock.acquire()
        partition = self.find_partition(partition_name)
        partition = copy.deepcopy(partition)
        self.lock.release()
        return partition

    def add_partition(self, partition_name):
        request = "<request type=\"add\">"
        request += "<partition name=\"" + partition_name + "\" />"
        request += "</request>"
        self.lock.acquire()
        if (self.find_partition(partition_name) == None):
            self.client.send(request, self.send_callback)
            xml.etree.ElementTree.SubElement(self.blackboard, "partition",
                                             {"name" : partition_name,
                                              "revision" : 0})
        self.lock.release()

    def delete_partition(self, partition_name):
        request = "<request type=\"delete\">"
        request += "<partition name=\"" + partition_name + "\" />"
        request += "</request>"
        self.lock.acquire()
        self.client.send(request, self.send_callback)
        partition = self.find_partition(partition_name)
        if partition != None:
            self.blackboard.remove(partition)
        self.lock.release()

    def add_pattern(self, pattern, partition_name):
        request = "<request type=\"add\">"
        request += "<partition name=\"" + partition_name + "\">"
        request += "<pattern>"
        request += pattern
        request += "</pattern>"
        request += "</partition>"
        request += "</request>"
        self.lock.acquire()
        self.client.send(request, self.send_callback)
        partition = self.find_partition(partition_name)
        bbPattern = xml.etree.ElementTree.SubElement(partition, "pattern")
        bbPattern.text = pattern
        partition.attrib["revision"] += 1
        self.lock.release()

    def delete_pattern(self, pattern, partition_name):
        request = "<request type=\"delete\" limit=\"1\">"
        request += "<partition name=\"" + partition_name + "\">"
        request += "<pattern>"
        request += pattern
        request += "</pattern>"
        request += "</partition>"
        request += "</request>"
        self.lock.acquire()
        self.client.send(request, self.send_callback)
        partition = self.find_partition(partition_name)
        for bbPattern in partition:
            if bbPattern.text.strip() == pattern.strip():
                partition.remove(bbPattern)
                break
        partition.attrib["revision"] += 1
        self.lock.release()

    def edit_pattern(self, old_pattern, new_pattern, partition_name):
        request = "<request type=\"replace\" limit=\"1\">"
        request += "<partition name=\"" + partition_name + "\">"
        request += "<pattern>"
        request += old_pattern
        request += "</pattern>"
        request += "<pattern>"
        request += new_pattern
        request += "</pattern>"
        request += "</partition>"
        request += "</request>"

        self.lock.acquire()
        self.client.send(request, self.send_callback)
        partition = self.find_partition(partition_name)
        for pattern in partition:
            if pattern.text.strip() == old_pattern.strip():
                pattern.text = new_pattern
                break
        partition.attrib["revision"] += 1
        self.lock.release()


        
                

    def send_callback(self, response):
        pass

if __name__ == "__main__":
    import asyncore
    darbs_status = DarbsStatus()
    asyncore.loop()
