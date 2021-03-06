HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11
ifeq ($(SYSTEM),Darwin)
	LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
			   -L/usr/lib64/mysql -lmysqlclient\
			   -I/usr/include/mysql\
			   -pthread\
			   -lgrpc++_reflection\
			   -ldl
	else
	LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
			   -L/usr/lib64/mysql  -lmysqlclient\
			   -I/usr/include/mysql\
			   -pthread\
			   -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
			   -ldl
	endif
	PROTOC = protoc
	GRPC_CPP_PLUGIN = grpc_cpp_plugin
	GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = ./protos

PROTO_CC :=  protoc
INCLUDE  :=  ./
PROTO_FLAGS := --cpp_out=$(INCLUDE)
PROTO__SOURCES := ./protos
obj-y          := $(PROTO__SOURCES:%.proto=$(INCLUDE)/%.grpc.pb.cc)
obj-y          += $(PROTO__SOURCES:%.proto=$(INCLUDE)/%.grpc.pb.h)


vpath %.proto $(PROTOS_PATH)

all: $(obj-y ) client  server 

# $(INCLUDE)/%.pb.cc: %.proto
#         $(PROTO_CC) $(PROTO_FLAGS) $^
#         @echo 'generate file' $@
        
# .PRECIOUS: %.grpc.pb.cc
# protoc -I ./protos --grpc_out=. --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin   user.proto 
%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

# .PRECIOUS: %.pb.cc
# protoc -I ./protos --cpp_out=.   user.proto 
%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

client: user.pb.o user.grpc.pb.o client.o   
	$(CXX) $^ $(LDFLAGS) -o $@
server: user.pb.o user.grpc.pb.o server.o 
	$(CXX) $^ $(LDFLAGS) -o $@

clean:
	rm -f *.o *.pb.cc *.pb.h client  server


# The following is to test your system and ensure a smoother experience.
# They are by no means necessary to actually compile a grpc-enabled software.

PROTOC_CMD = which $(PROTOC)
PROTOC_CHECK_CMD = $(PROTOC) --version | grep -q libprotoc.3
PLUGIN_CHECK_CMD = which $(GRPC_CPP_PLUGIN)
HAS_PROTOC = $(shell $(PROTOC_CMD) > /dev/null && echo true || echo false)
ifeq ($(HAS_PROTOC),true)
HAS_VALID_PROTOC = $(shell $(PROTOC_CHECK_CMD) 2> /dev/null && echo true || echo false)
endif
HAS_PLUGIN = $(shell $(PLUGIN_CHECK_CMD) > /dev/null && echo true || echo false)

SYSTEM_OK = false
ifeq ($(HAS_VALID_PROTOC),true)
ifeq ($(HAS_PLUGIN),true)
SYSTEM_OK = true
endif
endif

system-check:
ifneq ($(HAS_VALID_PROTOC),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have protoc 3.0.0 installed in your path."
	@echo "Please install Google protocol buffers 3.0.0 and its compiler."
	@echo "You can find it here:"
	@echo
	@echo "   https://github.com/google/protobuf/releases/tag/v3.0.0"
	@echo
	@echo "Here is what I get when trying to evaluate your version of protoc:"
	@echo
	-$(PROTOC) --version
	@echo
	@echo
endif
ifneq ($(HAS_PLUGIN),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have the grpc c++ protobuf plugin installed in your path."
	@echo "Please install grpc. You can find it here:"
	@echo
	@echo "   https://github.com/grpc/grpc"
	@echo
	@echo "Here is what I get when trying to detect if you have the plugin:"
	@echo
	-which $(GRPC_CPP_PLUGIN)
	@echo
	@echo
endif
ifneq ($(SYSTEM_OK),true)
	@false
endif

