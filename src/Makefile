CC = gcc        # Define the compiler
CFLAGS = -g -Wall  # Flags for debugging and warnings

# List of source and object files
SRCS = main.c prompt.c input.c hop.c reveal.c log.c fg.c bg.c
OBJS = $(SRCS:.c=.o)

# Target executable name
TARGET = a.out

# Create obj directory if it doesn't exist
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to create object files in obj folder
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure obj directory exists
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Rule to create object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove the executable and object files 
clean:
	rm -f $(TARGET)
	rm -f $(OBJDIR)/*.o