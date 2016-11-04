import re
import time
import getopt
import sys
import socket
import http_common

def send(socket, msg):
    totalsent = 0
    while totalsent < len(msg):
        sent = socket.send(msg[totalsent:])
        if sent == 0:
            raise RuntimeError("socket connection broken")
        totalsent = totalsent + sent

def receive(socket):
    status, response, start_of_body = http_common.read_until_end_of_headers(socket)
    body = response[start_of_body:]

    # parse out the response line
    p = re.compile(r'HTTP\/1\.[0|1] (\d*) (.*)\r\n((.*: .*\r\n)*)\r\n')
    m = p.match(response)
    status = int(m.group(1))
    message = m.group(2)
    headers_string = m.group(3)

    # check status
    if status != 200:
        return "ERROR %d: %s" % (status, message)

    # parse headers
    p = re.compile(r'(.*): (.*)\r\n')
    header_list = p.findall(headers_string)

    headers = {}
    for h in header_list:
        headers[h[0]] = h[1]

    # 4.5.5 says chunked is checked first
    if headers.get('Transfer-Encoding', None) == "chunked":
        # read body as chunked
        print "chunky!"
        chunk = socket.recv(256)
        print repr(body + chunk)
    elif 'Content-Length' in headers:
        # read body until specified length
        chunks = [body]
        bytes_recd = len(body)
        while bytes_recd < headers['Content-Length']:
            chunk = socket.recv(256)
            if chunk == '':
                # socket closed by server
                break;
            chunks.append(chunk)
            bytes_recd += + len(chunk)

        response = ''.join(chunks)
        return response

def format_get_request(url="", headers={}, body=""):
    req_line = "GET %s HTTP/1.1\r\n" % url

    headers['Content-Length'] = len(body)

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

        s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        headers = {
            'Connection': 'close',
            'Host': domain
        }

        t0 = time.time() * 1000.0 # START TIMING

        s1.connect((domain, port))
        get_msg = format_get_request(url=url, headers=headers)

        # send and recieve
        send(s1, get_msg)
        response = receive(s1)

        s1.close()

        t1 = time.time() * 1000.0 # END TIMING

        # compute RTT
        rtt = t1 - t0

        if print_rtt:
            print "\033[1;94mRTT: %d\033[0m" % rtt

        print response
