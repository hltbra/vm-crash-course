m5: m5.c
	@gcc -Wall m5.c -o m5

a5_files:
	@python a5 < test6.input > test6.m5
	@python a5 < test7.input > test7.m5
	@python a5 < test8.input > test8.m5

test: a5_files m5
	@./m5 test6.m5 | python test.py test6.expected
	@./m5 test7.m5 | python test.py test7.expected
	@./m5 test8.m5 | python test.py test8.expected

clean:
	@rm -f *.m5 m5