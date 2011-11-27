includes = distance_vector.h link_state.h extra_credit.h
objects = distance_vector.o link_state.o extra_credit.o
sources = distance_vector.c link_state.c extra_credit.c
out = distance_vector link_state extra_credit

all: $(objects) link distance extra

$(objects): $(includes)

link: 
	cc -o link_state link_state.o -lm

distance: 
	cc -o distance_vector distance_vector.o -lm

extra:
	cc -o extra_credit extra_credit.o -lm
.PHONY: clean 
clean:
	rm -f $(out) $(objects)
