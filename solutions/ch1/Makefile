m6: m6.c
	@gcc -Wall m6.c -o m6

a6_files:
	@python a6 < test6.input > test6.m6
	@python a6 < test7.input > test7.m6
	@python a6 < test8.input > test8.m6
	@python a6 < test9.input > test9.m6

test: a6_files m6
	@./m6 test6.m6 | python test.py test6.expected
	@./m6 test7.m6 | python test.py test7.expected
	@./m6 test8.m6 | python test.py test8.expected
	@./m6 test9.m6 | python test.py test9.expected

clean:
	@rm -f *.m6 m6