m3: m3.c
	@gcc m3.c -o m3

a3_files:
	@python a3 < test1.input > test1.m3
	@python a3 < test2.input > test2.m3
	@python a3 < test3.input > test3.m3
	@python a3 < test4.input > test4.m3
	@python a3 < test5.input > test5.m3

test: a3_files m3
	@./m3 test1.m3 | python test.py test1.expected
	@./m3 test2.m3 | python test.py test2.expected
	@./m3 test3.m3 | python test.py test3.expected
	@./m3 test4.m3 | python test.py test4.expected
	@./m3 test5.m3 | python test.py test5.expected
	@rm *.m3
