
### CF-Crashy-app

A simple app that will open a socket and wait for connections.  When the app receivs a "crash" word through the socket, it will crash.

This is a testing app.

### Usage

To build and push the app you need gcc, headers and make.  You will also need the cf cli.

To build:
```
$ make build
```
To push the app:
```shell
$ make 
```
To build and push:
```
$ make all
```
or simply 
```
$ make
```
To test the app in cf-dev:
```
$ make test
```


