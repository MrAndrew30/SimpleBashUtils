import sys

from random import randrange

path = "../../data-samples/cat"

with open(path + "/bytes.txt", "w") as file:
    for i in range(0, 300):
        file.write(f"{chr(i)} is {i}\n")

with open(path + "/spaces.txt", "w") as file:
    for i in range(50):
        file.write("\n" * randrange(0, 6))
        for i in range(50):
            file.write(chr(randrange(1, 300)))
    file.write("\n" * randrange(0, 4))

with open(path + "/test0.txt", "w") as file:
    for i in range(20):
        file.write("\t" * randrange(0, 6))
        for i in range(50):
            file.write(chr(randrange(1, 300)))

