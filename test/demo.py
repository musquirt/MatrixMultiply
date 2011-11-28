from subprocess import *;

str = check_output(["../multiply", "-a", "9", "18", "-b", "18", "6"])

print str,

