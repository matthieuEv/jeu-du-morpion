# This is needed for the project to compile with OpenGL
ifeq ($(shell uname), Darwin)
    CFLAGS := -w $(shell pkg-config --cflags glfw3 glew) $(shell pkg-config --libs glfw3 glew) -framework OpenGL -lm -Wno-implicit-function-declaration
else
    CFLAGS := -w -lglfw -lGL -lGLEW -lm -Wno-implicit-function-declaration # not tested on linux or windows
endif

all:
	gcc -DCONFIG_PLAYER_MANAGER_OPENGL -DCONFIG_OPENGLUI $(CFLAGS) src/*.c -o ./main && ./main

etape2:
	gcc -DCONFIG_TEXTUI -DCONFIG_PLAYER_MANAGER_SCANF $(CFLAGS) src/*.c -o ./main && ./main

test_draw:
	gcc -DCONFIG_TEXTUI -DCONFIG_PLAYER_MANAGER_MOCK -DDEF_DRAW $(CFLAGS) src/*.c -o ./main && ./main

test_circle:
	gcc -DCONFIG_TEXTUI -DCONFIG_PLAYER_MANAGER_MOCK -DDEF_CIRCLE_WINS $(CFLAGS) src/*.c -o ./main && ./main

test_cross:
	gcc -DCONFIG_TEXTUI -DCONFIG_PLAYER_MANAGER_MOCK -DDEF_CROSS_WINS $(CFLAGS) src/*.c -o ./main && ./main