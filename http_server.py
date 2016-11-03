import os
import re
import threading
import sys
import socket
import time

statstr = {
    200: "OK",
    400: "Bad Request",
    403: "Forbidden",
    404: "Not Found",
    405: "Method Not Allowed",
    411: "Length Required",
}

def parse_request(request):
    # parse the request
    p = re.compile(r'(.*) (.*) HTTP\/1\.[0|1]\r\n((.*: .*\r\n)*)\r\n')
    m = p.match(request)
    if m is not None:
        message_type = m.group(1)
        url = m.group(2)
        headers_string = m.group(3)

        # message type checking
        if message_type != "GET":
            return 405, None

        # check URL
        url = url[1:] if url[0] == '/' else url
        if not os.path.exists(url):
            return 404, None

        if not os.path.isfile(url):
            return 403, None

        p = re.compile(r'(.*): (.*)\r\n')
        header_list = p.findall(headers_string)

        headers = {}
        for h in header_list:
            headers[h[0]] = h[1]

        body_length = headers.get('Content-Length', -1)
        # listen for body? Do we have to?

        return 200, url
    else:
        return 400, None

def handle_connection(clientsocket, address):
    # read the request
    chunks = []
    bytes_recd = 0
    status = 200
    i = 0

    # read until we know the Content-Length
    end_of_headers = -1
    request = ''
    while True:
        chunk = clientsocket.recv(4)
        if chunk == '':
            status = 500
            break
        chunks.append(chunk)
        bytes_recd = bytes_recd + len(chunk)

        request = ''.join(chunks)

        # parse request
        end_of_headers = request.find("\r\n\r\n")
        if end_of_headers >= 0:
            break

    status, url = parse_request(request)

    # return web page
    if status > 200:
        totalsent = 0
        msg = "HTTP/1.1 %i %s\r\nContent-Length: 0\r\n\r\n" % (status, statstr[status])
        while totalsent < len(msg):
            sent = clientsocket.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent

        clientsocket.close()
    else:
        totalsent = 0

        msg = "HTTP/1.1 200 OK\r\n\r\n"
        with open(url) as f:
            msg += "".join(f.readlines())

        while totalsent < len(msg):
            sent = clientsocket.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent

        clientsocket.close()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "Usage: python http_server.py port_number"
    else:
        port = int(sys.argv[1])

        #create an INET, STREAMing socket
        serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        #bind the socket to a public host, and a well-known port
        serversocket.bind(('', port))

        #become a server socket
        serversocket.listen(5)

        while True:
            #accept connections from outside
            (clientsocket, address) = serversocket.accept()

            # kick off new thread
            t = threading.Thread(target=handle_connection, args=(clientsocket, address,))
            t.start()
