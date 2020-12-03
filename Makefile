CXX      := g++
CXXFLAGS := -std=c++17 -ltbb


%: code/%.cpp utils
	$(CXX) -g build/utils.o $< -o build/$@ $(CXXFLAGS)

utils:
	$(CXX) -c code/utils.cpp -o build/utils.o $(CXXFLAGS)
