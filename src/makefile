files := calc.c util.c
temfiles := number sign
para := '404'

run: compile
	@ ./calc $(para)

compile: remove
	@ gcc $(files) -o calc -g

remove:
ifneq ("$(wildcard $(temfiles))","")	
	@ rm $(temfiles)
endif

debug: 
	@ gdb calc
