
.PHONY: all

all: static dynamic

static:
	g++ -fPIC -shared mul.cpp -o libmul.so
	g++ static.cpp -L. -lmul -Wl,-rpath,. -o static

dynamic:
	g++ -ldl dynamic.cpp -o dynamic

clean:
	rm static dynamic libmul.so

