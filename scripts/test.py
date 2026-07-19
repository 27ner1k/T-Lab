import networkx as nx
import csv
import sys

# Данный скрипт - уже реализованный PageRank в python через библиотеку
# нужен для проверки правильности работы моего кода,
# немного уменьшил порог сходимости в pr, так как иначе этот код не дает той же точности, что мой

edges = []
with open(sys.argv[1]) as f:
    reader = csv.reader(f)
    next(reader)
    for row in reader:
        fromm = int(row[0])
        to = int(row[1])
        edges.append((fromm, to))

G = nx.DiGraph()
G.add_edges_from(edges)
pr = nx.pagerank(G, alpha=0.85, tol=1e-12, max_iter=200)

with open(sys.argv[2], "w") as f:
     f.write(f"vertex,rank\n")
     for i in sorted(pr):
         f.write(f"{i},{pr[i]:.10f}\n")