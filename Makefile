all: build push

build:
	gcc -g crashy.c -o crashy

push:
	cf push crashy -c './crashy' -b binary_buildpack

debug:
	gdb ./crashy 

crash:
	(./putcrash.sh | telnet 192.168.11.11 80;echo )

test:
	curl -v -k crashy.local.pcfdev.io
