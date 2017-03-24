CXX      ?= g++
CXXFLAGS := -std=c++14 -Iinclude -g

SRCDIR := src
BINDIR := bin
DEPDIR := .dep

SRCS     := $(wildcard $(SRCDIR)/*.cpp)
EXECS    := $(SRCS:$(SRCDIR)/%.cpp=$(BINDIR)/%)
DEPS     := $(SRCS:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)

all: $(EXECS)

.PHONY: clean all

bin/stat_table : src/stat_table.cpp $(DEPDIR)/stat_table.d | $(BINDIR)
	$(CXX) $(CXXFLAGS) -lboost_program_options $< -o $@

bin/kolmogorov_smirnov : src/kolmogorov_smirnov.cpp $(DEPDIR)/kolmogorov_smirnov.d | $(BINDIR)
	$(CXX) $(CXXFLAGS) -lboost_program_options $< -o $@

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp | $(DEPDIR)
	@$(CXX) $(CXXFLAGS) -MM -MP -MT $(BINDIR)/$(patsubst $(SRCDIR)/%.cpp,%,$<) -MF $@ $<

-include $(DEPS)

clean:
	rm -rf $(BINDIR) $(DEPDIR)

$(BINDIR):
	@mkdir $(BINDIR)

$(DEPDIR):
	@mkdir $(DEPDIR)
