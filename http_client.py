import time
import getopt
import sys
import socket

class mysocket:
    '''demonstration class only
      - coded for clarity, not efficiency
    '''

    def __init__(self, sock=None):
        if sock is None:
            self.sock = socket.socket(
                socket.AF_INET, socket.SOCK_STREAM)
        else:
            self.sock = sock

    def connect(self, host, port):
        self.sock.connect((host, port))

    def mysend(self, msg):
        totalsent = 0
        while totalsent < len(msg):
            sent = self.sock.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent

    def myreceive(self):
        chunks = []
        bytes_recd = 0
        i = 0
        while True:
            chunk = self.sock.recv(256)
            if chunk == '':
                break;
            chunks.append(chunk)
            bytes_recd = bytes_recd + len(chunk)
        return ''.join(chunks)

def format_get_request(url="", headers="", body=""):
    req_line = "GET %s HTTP/1.1\r\n" % url

    headers['Content: Length'] = len(body)

    header_lines = ""
    for key, value in headers.iteritems():
        header_lines += "%s: %s\r\n" % (key, value)

    return req_line + header_lines + "\r\n" + body


if __name__ == "__main__":
    optlist, args = getopt.getopt(sys.argv[1:], "p")
    if len(args) != 2:
        print "Usage: python http_client.py [-p] server_url port_number"
    else:
        print_rtt = len(optlist) > 0
        server_url = args[0]
        port = int(args[1])

        if '/' in server_url:
            domain = server_url[0:server_url.index('/')]
            url = server_url[server_url.index('/'):]
        else:
            domain = server_url
            url = '/'

        s1 = mysocket()
        t0 = time.time() # START TIMING
        s1.connect(domain, port)
        headers = {
            'Connection': 'close',
            'Host': domain
        }
        get_msg = format_get_request(url=url, headers=headers)
        print get_msg
        s1.mysend(get_msg)
        response = s1.myreceive()
        t1 = time.time() # END TIMING
        s1.sock.close()

        # compute RTT
        rtt = t1 - t0

        if print_rtt:
            print "\033[1;94mRTT: %d\033[0m" % rtt

        print response
