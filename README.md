# HTTP Server
A simple HTTP server written in C using UNIX sockets. Supports GET and HEAD requests.

##Instructions
1. Install `make`.
2. Clone this repository using this command:
```
git clone https://github.com/ketanhwr/http-server.git
```
3. Run `make`:
```
make
```
4. Run the server:
```
[sudo] ./http-server
```
5. Open your web browser and head on to
```
localhost:10000
```

##Features
- Supports GET and HEAD requests.
- Looks for the file `index.html` in the current directory and serves it if found.
- Returns Status Codes `200 (OK)`, `400 (Bad Request)` and `404 (Not Found)`

##TODOs
- Serving multiple clients at once using multi-threading.
- Adding option to change ROOT directory and PORT number by passing them as arguments.

