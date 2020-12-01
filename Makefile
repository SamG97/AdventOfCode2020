%.o: code/%.cpp
	g++ $< -o build/$@