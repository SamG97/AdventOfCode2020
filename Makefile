CXX      := g++
CXXFLAGS := -std=c++17


%: code/%.cpp utils
	$(CXX) $(CXXFLAGS) build/utils.o $< -o build/$@

utils:
	$(CXX) $(CXXFLAGS) -c code/utils.cpp -o build/utils.o
