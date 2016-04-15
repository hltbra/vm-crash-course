m2: m2.c
	gcc m2.c -o m2

a2_files:
	@python a2 < test1.input > test1.m2
	@python a2 < test2.input > test2.m2
	@python a2 < test3.input > test3.m2
	@python a2 < test4.input > test4.m2

test: a2_files m2
	@./m2 test1.m2 | python test.py test1.expected
	@./m2 test2.m2 | python test.py test2.expected
	@./m2 test3.m2 | python test.py test3.expected
	@./m2 test4.m2 | python test.py test4.expected
