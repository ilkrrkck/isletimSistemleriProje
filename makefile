hepsi: derle calistir

derle:
	gcc -o ./main.o -c ./main.c -pthread
	gcc -o ./main ./main.c -pthread

calistir:
	./main