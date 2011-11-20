includes = distance_vector.h link_state.h
objects = distance_vector.o link_state.o
sources = distance_vector.c link_state.c
out = distance_vector link_state

all: $(objects) link distance

$(objects): $(includes)

link: 
	cc -o link_state link_state.o -lm

distance: 
	cc -o distance_vector distance_vector.o -lm

.PHONY: clean 
clean:
	rm -f $(out) $(objects)
