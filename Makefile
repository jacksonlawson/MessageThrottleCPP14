all:
	g++ -g -std=gnu++14 Main.cpp -I./ -o throttle

clean:
	rm throttle

test:
	./throttle