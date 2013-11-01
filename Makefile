TARGET = epoll_socket_server

OBJECTS = socket_server.o s_socket.o epoll_reactor.o socket_address.o test.o

VPATH = common

CXXFLAGS = -std=c++11 -Wall -g

%.o : %.cpp
	@$(CXX) $(CXXFLAGS) -o $@ -c $<
	@echo $<

all : $(TARGET)
	@echo all done

clean :
	@rm -f $(OBJECTS) $(TARGET)
	@echo clean done


$(TARGET) : $(OBJECTS)
	@$(CXX) $(CXXFLAGS) -o $@ $^
