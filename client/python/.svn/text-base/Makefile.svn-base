JAVAFILES=$(shell ls *.java 2> /dev/null)

all: ../libclient.so

../libclient.so: 
	make -C ../ libclient.so

Main.class: $(JAVAFILES) ../libclient.so

clean:
	rm -f *.pyc
	rm -f ../libclient.so
