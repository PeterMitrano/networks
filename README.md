## Testing Instructions

### examples of server:
python http_server.py 25546

### examples of client:
python http_client.py gnu.org 80 # response is 301 moved
python http_client.py www.cnn.com 80 # response is 200 OK and webpage
python http_client.py www.ccwork3.com/TMDG.html 25546 # response is 200 OK and webpage


## Easter egg!:
curl -v -H "Brew-Coffee: True" localhost:25546/TMDG.html # response is 418 I'm a teapot

## HTTP Client RTT Results

### Testing with real websites
www.httpbin.org:80/TMDG.html (from cccwork3)
RTT(ms)
48
49
49
49
49
51
50
50
50
51
Average: 49.6 ms

www.gnu.org:80 (from cccwork4)
RTT (ms)
70
42
47
47
44
31
53
59
46
42
Average: 48.1 ms


### Testing with my http_server
cccwork4.wpi.edu:22535/TMDG.html (from cccwork3)
RTT(ms)
9
3
3
3
3
3
3
3
3
3
Average: 3.6 ms

my-laptop-ip:22888/TMDG.html (from cccwork4)
RTT( ms)
11
8
6
9
9
6
16
15
16
13
Average: 10.9 ms
