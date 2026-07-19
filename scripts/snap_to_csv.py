import sys


with open(sys.argv[1]) as inp, open(sys.argv[2], "w") as out:
    out.write("from,to\n")
    for line in inp:
        if line.startswith("#"):
            continue
        u, v = line.split()
        out.write(f"{u},{v}\n")