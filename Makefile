CXX := clang++
CXXFLAGS := --std=c++2a -stdlib=libc++ -DNDEBUG -O3#-g -O0
BIN_DIR := bin

%:
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/day$@ src/day$@/*.cpp
	bin/day$@

.PHONY: format
format:
	for d in src/* ; do \
		clang-format -i $$d/*.cpp ; \
	done

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN_DIR)/*
