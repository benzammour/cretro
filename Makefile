# project name (generate executable with this name)
TARGET	= cretro

CC	= gcc
# compiling flags here
CFLAGS   = -g -std=c17 -Wall -Wextra -Werror -Wunused -Wshadow -Wcast-align -Wconversion\
           -Wlogical-op -Wdouble-promotion -fstack-protector -Wstack-protector -Wredundant-decls -Wpacked -Wmissing-declarations\
           -Wmissing-prototypes -Wstrict-prototypes -Wno-aggressive-loop-optimizations -Wjump-misses-init -Wwrite-strings\
           -Wcast-qual -Wundef -funsafe-loop-optimizations -Wunsafe-loop-optimizations -Wfloat-equal -Wtrampolines -Warray-bounds=2\
           -Wsuggest-attribute=cold -Wsuggest-attribute=malloc -Wsuggest-attribute=pure -Wsuggest-attribute=const\
           -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wstrict-overflow=4 -Wswitch-enum -Wswitch-default\
           -Wmissing-include-dirs -Wformat=2 -O3

LINKER	= gcc
# linking flags here
LFLAGS	= -Wall -lm -lSDL2

# change these to proper directories where each file should be
SRCDIR	= src
OBJDIR	= obj
BINDIR	= .

INSTALL_PATH = /usr/bin/$(TARGET)

SOURCES		:= $(wildcard $(SRCDIR)/*.c)
INCLUDES	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS		:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm			:= rm -f
mkdir       := mkdir -p

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c | $(BINDIR) $(OBJDIR) 
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(BINDIR) $(OBJDIR):
	@$(mkdir) $@

.PHONY: clean
clean:
	@$(rm) -r $(OBJDIR)
	@$(rm) $(TARGET)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

.PHONY: install
install: $(BINDIR)/$(TARGET)
	@sudo cp $(BINDIR)/$(TARGET) $(INSTALL_PATH)
	@echo "Installed to "$(INSTALL_PATH)

.PHONY: uninstall
uninstall:
	@if [[ -f $(INSTALL_PATH) ]]; then sudo rm $(INSTALL_PATH); echo "Removed binary "$(INSTALL_PATH)"!"; else echo "Nothing to uninstall!"; fi

