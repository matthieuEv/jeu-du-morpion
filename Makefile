all:
	echo "unused for now"

test_draw:
	gcc -DCONFIG_TEXTUI -DCONFIG_PLAYER_MANAGER_MOCK -DDEF_DRAW src/*.c -o main && ./main

test_circle:
	gcc -DCONFIG_TEXTUI -DCONFIG_PLAYER_MANAGER_MOCK -DDEF_CIRCLE_WINS src/*.c -o main && ./main

test_cross:
	gcc -DCONFIG_TEXTUI -DCONFIG_PLAYER_MANAGER_MOCK -DDEF_CROSS_WINS src/*.c -o main && ./main