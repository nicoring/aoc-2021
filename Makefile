CXX := clang++
CXXFLAGS := --std=c++2a -stdlib=libc++ -I . #-DNDEBUG -g -O0
BIN_DIR := bin

day1: src/day1/*.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$@ $^
	bin/day1

day2: src/day2/*.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$@ $^
	bin/day2

day3: src/day3/*.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$@ $^
	bin/day3

.PHONY: format
format:
	for d in src/* ; do \
		clang-format -i $$d/* ; \
	done

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN_DIR)/*
