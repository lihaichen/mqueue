BUILD ?= build
SRC_PATH := mqueue
C_SOURCES += $(SRC_PATH)/init.c \
$(SRC_PATH)/list.c \
$(SRC_PATH)/loop.c  \
$(SRC_PATH)/message.c \
$(SRC_PATH)/object.c \
$(SRC_PATH)/thread.c \
$(SRC_PATH)/timer.c \
$(SRC_PATH)/main.c

TEST_C_SOURCES += example/main.c

INC += -Imqueue

SRCS += $(wildcard $(SRC_PATH)/*.c)
OBJS := $(SRCS:.c=.o)

TEST_SRCS := $(wildcard example/*.c)
TEST_OBJS := $(TEST_SRCS:.c=.o)

TARGET = test
LIB = equeue.a

$(TARGET): $(OBJS) $(TEST_OBJS)
	$(CC) $(INC) -o $(TARGET) $(OBJS) $(TEST_OBJS)

$(LIB): $(OBJS)
	ar rc $(LIB) $(OBJS)

lib: $(LIB)


all: $(TARGET)

clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(TARGET) $(LIB)