import os
import re
import threading
import sys
import socket
import time
import http_common

def parse_request(request):
    # parse the request
    p = re.compile(r'(.*) (.*) HTTP\/1\.[0|1]\r\n((.*: .*\r\n)*)\r\n')
    m = p.match(request)
    if m is not None:
        message_type = m.group(1)
        url = m.group(2)
        headers_string = m.group(3)
        print "url [%s] requested" % url

        # message type checking
        if message_type != "GET":
            return 405, None

        # check URL
        if url[0] == '/':
            url = url[1:]
        else:
            url = url

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
        brew_coffee = headers.get('Brew-Coffee', False)

        if brew_coffee:
            return 418, None
        # TODO: listen for body? Do we have to?

        return 200, url
    else:
        return 400, None

def format_response(headers={}, body=""):
    req_line = "HTTP/1.1 200 OK\r\n"

    headers['Content-Length'] = len(body)

    header_lines = ""
    for key, value in headers.iteritems():
        header_lines += "%s: %s\r\n" % (key, value)

    return req_line + header_lines + "\r\n" + body

def handle_connection(clientsocket, address):
    # read the request
    status, request, start_of_body = http_common.read_until_end_of_headers(clientsocket)
    excess = request[start_of_body:]

    if status == 200:
        status, url = parse_request(request)

    # return web page
    if status > 200:
        totalsent = 0
        msg = "HTTP/1.1 %i %s\r\nContent-Length: 0\r\n\r\n" \
                    % (status, http_common.statstr[status])
        while totalsent < len(msg):
            sent = clientsocket.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent

        clientsocket.close()
    else:
        totalsent = 0

        body = ""
        f = open(url, 'r')
        body += "".join(f.readlines())
        msg = format_response(body=body)

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
