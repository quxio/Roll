TARGET_EXEC = roll
ALIAS = cointoss

prefix ?= /usr/local
bindir ?= $(prefix)/bin

STRIP ?= strip

CXXFLAGS += -std=c++17 -O3

.PHONY: all install clean
all: $(TARGET_EXEC)

$(TARGET_EXEC): main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) "$<" -o "$@"
	$(STRIP) "$@"

install: $(TARGET_EXEC)
	install -d "$(DESTDIR)$(bindir)"
	install -m755 "$<" "$(DESTDIR)$(bindir)/$(TARGET_EXEC)"
	ln -sf "$(TARGET_EXEC)" "$(DESTDIR)$(bindir)/$(ALIAS)"

clean:
	rm -f "$(TARGET_EXEC)"