CXX=clang++

src = $(wildcard src/*.cpp)
obj = $(src:.c=.o)

LDFLAGS = -lepoxy -lglfw

all: $(obj)
	$(CXX) -O0 -g -o build/program $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog
