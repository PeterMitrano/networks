import re
import threading
import sys
import socket
import time

def handle_connection(clientsocket, address):
    # read the request
    chunks = []
    bytes_recd = 0
    bad_request = False
    i = 0

    # read until we know the Content-Length
    end_of_headers = -1
    request = ''
    while True:
        chunk = clientsocket.recv(4)
        if chunk == '':
            bad_request = True
            break;
        chunks.append(chunk)
        bytes_recd = bytes_recd + len(chunk)

        request = ''.join(chunks)

        # parse request
        end_of_headers = request.find("\r\n\r\n")
        if end_of_headers >= 0:
            break;

    # parse the request
    p = re.compile(r'GET (.*) HTTP\/1\.[0|1]\r\n((.*: .*\r\n)*)\r\n')
    m = p.match(request)
    if m is not None:
        url = m.group(1)
        headers_string = m.group(2)

        p = re.compile(r'(.*): (.*)\r\n')
        header_list = p.findall(headers_string)

        headers = {}
        for h in header_list:
            headers[h[0]] = h[1]

        print headers
    else:
        bad_request = True

    # return web page
    if bad_request:

        totalsent = 0
        msg = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n"
        while totalsent < len(msg):
            sent = clientsocket.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent

        clientsocket.close()
    else:
        totalsent = 0

        msg = "HTTP/1.1 200 OK\r\n\r\n"
        with open("TMDG.html") as f:
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
