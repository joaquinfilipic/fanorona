OUTPUT_FILE=fanorona
FILES=fanoronaFront.c fanoronaBack.h fanoronaBack.c getnum.h getnum2.c

all:
	gcc -o $(OUTPUT_FILE) $(FILES) -pedantic

clean:
	rm $(OUTPUT_FILE)
