.PHONY: all clean install dir $(TARGETS)

all clean install dir: $(TARGETS)

$(TARGETS):
	@echo "### make $(MAKECMDGOALS) in $@"
	@$(MAKE) $(MAKECMDGOALS) -C $@ --no-print-directory
	@echo

