all:
	@echo "-----------------------------------------------"
	@echo "You can use -init-, -run-, -valgrind- or -clean- parameters"
	@echo "sample: make clean"
	@echo "-----------------------------------------------"

	$(MAKE) init

init:
	gcc 121044029_main.c -o exe -lm
	@echo "You can run exe file"

run:
	./exe -N 4 -X file.dat -M 100

valgrind:
	valgrind -v --log-file=LOGVAL.txt --tool=memcheck  --leak-check=yes --show-reachable=yes --track-origins=yes ./exe -N 4 -X file.dat -M 100

clean:
	rm exe *.dat *.log
