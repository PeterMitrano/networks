import socket

BUFFER_SIZE = 128

statstr = {
    200: "OK",
    400: "Bad Request",
    403: "Forbidden",
    404: "Not Found",
    405: "Method Not Allowed",
    411: "Length Required",
    418: "I'm a Teapot",
}

def read_until_end_of_headers(socket):
    # read until we know the headers
    bytes_recd = 0
    chunks = []
    end_of_headers = -1
    request = ''
    while True:
        chunk = socket.recv(4)
        if chunk == '':
            return 500, None, 0
        chunks.append(chunk)
        bytes_recd = bytes_recd + len(chunk)

        request = ''.join(chunks)

        # parse request
        end_of_headers = request.find("\r\n\r\n")
        if end_of_headers >= 0:
            return 200, request, end_of_headers + 4

