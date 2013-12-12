objects = main.o benchmark.o metric.o json.o
all : $(objects)
	g++ -o all $(objects)
main.o : benchmark.h basic.h
benchmark.o: metric.h basic.h json.h
metric.o: basic.h
json.o: json.h
.PHONY : clean
clean :
	rm -rf *o all
