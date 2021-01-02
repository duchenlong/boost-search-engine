export BUILD_ROOT = $(shell pwd)
export INCLUDE_PATH = $(BUILD_ROOT)/include

export BUILD_PATH = $(BUILD_ROOT)/src

all:
	@for dir in $(BUILD_PATH); \
	do \
		make -C $$dir; \
	done

clean:
	rm -rf $(BUILD_ROOT)/link_obj
	rm -rf $(BUILD_ROOT)/dep
	rm -rf main
