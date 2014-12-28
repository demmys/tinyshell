CC     = clang
CFLAGS = -Wextra -MMD -MP
TARGET = tinyshell
SRCDIR = src
OBJDIR = obj
SRCS   = $(wildcard $(SRCDIR)/*.c)
OBJS   = $(subst $(SRCDIR),$(OBJDIR),$(SRCS:.c=.o))
DEPS   = $(OBJS:.o=.d)

.PHONY: all clean

$(TARGET): $(OBJS)
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

all: clean $(TARGET)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
