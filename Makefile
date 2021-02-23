#NAME: Emily Vainberg 
#EMAIL: emily.vainberg@gmail.com
#ID: 605341878

#NAME: Brian Kwon 
#EMAIL:
#ID:


default:
	gcc -o lab3a -g -Wextra -Wall lab3a.c 

dist:
	tar -czvf lab3a-605341878.tar.gz Makefile README lab3a.c ext2_fs.h

clean:
	rm -f lab3a-605341878.tar.gz lab3a
