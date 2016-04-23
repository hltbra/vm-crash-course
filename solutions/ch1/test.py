import sys

with open(sys.argv[1]) as f:
    expected = f.read()

result = sys.stdin.read()

assert expected == result, "FAILED: Got '{}', expected '{}'".format(result, expected)
