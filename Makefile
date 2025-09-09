main: main.c alloc.c mask.c entity.c components.c entity_manager.c components_container.c
	gcc -std=c2x -O3 -g -fsanitize=address -fsanitize=leak -o $@ $^

main-valgrind: main.c alloc.c mask.c entity.c components.c entity_manager.c components_container.c
	gcc -std=c2x -O0 -g -o $@ $^

clean:
	rm -f main main-valgrind

run:
	./main
