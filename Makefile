all:
	${CC} -framework ApplicationServices -o vanisher vanisher.c

clean:
	rm vanisher

.PHONY: all clean
