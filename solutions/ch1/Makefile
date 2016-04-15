m4: m4.c
	@gcc -Wall m4.c -o m4

a4_files:
	@python a4 < test6.input > test6.m4
	@python a4 < test7.input > test7.m4

test: a4_files m4
	@./m4 test6.m4 | python test.py test6.expected
	@./m4 test7.m4 | python test.py test7.expected

clean:
	@rm -f *.m4 m4