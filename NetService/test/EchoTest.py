#!/usr/bin/env python

import socket
import sys
import subprocess

address= ('localhost', 12345)
p=subprocess.Popen('../bin/Server')
try: 
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(address)

    s.send('1234567890\n')
    data = s.recv(100)
    if data != '1234567890\n':
        print "failed"
        sys.exit(1)
    print "pass"
except socket.error, msg:
    sys.stderr.write("error: %s\n" % msg[1])
    print "failed"
    sys.exit(1)
p.kill()
sys.exit(0)

