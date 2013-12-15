objects = main.o benchmark.o metric.o json.o metric_luxand.o
all : $(objects)
	g++ -o all $(objects) -lfsdk -ldl
main.o : benchmark.h basic.h
benchmark.o: metric.h basic.h json.h
metric_luxand.o: metric.h SDK/Luxand/include/LuxandFaceSDK.h
metric.o: basic.h
json.o: json.h
.PHONY : clean
clean :
	rm -rf *o all
