SRCS += \
	$(srcdir)/utils/chain.c \
	$(srcdir)/utils/cqueue.c \
	$(srcdir)/utils/logger.c \
	
ifndef __WIN32
TSRCS += \
	$(srcdir)/utils/cqueue-test.c \


LIBS += -lm
endif

