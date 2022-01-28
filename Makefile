CXX=clang++

src = $(wildcard src/*.cpp)
obj = $(src:.c=.o)

LDFLAGS = -lepoxy -lglfw

build: $(obj)
	$(CXX) -o build/program $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog
