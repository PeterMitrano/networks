import sys
import socket
import time

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "Usage: python http_server.py port_number"
    else:
        port = int(sys.argv[1])

        #create an INET, STREAMing socket
        serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        #bind the socket to a public host, and a well-known port
        serversocket.bind((socket.gethostname(), port))

        #become a server socket
        serversocket.listen(5)

        while True:
            #accept connections from outside
            (clientsocket, address) = serversocket.accept()

            # kick off new thread
            #ct = client_thread(clientsocket)
            #ct.run()
