all:
	g++ -c src/util.cpp src/graphics.cpp src/input.cpp src/net.cpp src/sound.cpp SDL2/lib/linux/libSDL2-2.0.so.0.2.1 SDL2/lib/linux/libSDL2_image-2.0.so.0.0.0 SDL2/lib/linux/libSDL2_net-2.0.so.0.0.0 SDL2/lib/linux/libSDL2_mixer-2.0.so.0.0.0 -fpic -std=c++0x && g++ -shared -o lib/linux/libSDL2W.so util.o graphics.o input.o net.o sound.o && rm util.o graphics.o input.o net.o sound.o 
