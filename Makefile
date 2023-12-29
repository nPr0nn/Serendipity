all:
	g++ -lraylib -lm src/*.cpp main.cpp -o main
	./main

web:
	cd build_web
	emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html" -DCMAKE_CXX_FLAGS="--preload-file ../assets"
	emmake make
	
run:
	./main

clean:
	rm main
