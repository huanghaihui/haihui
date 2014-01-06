
clang -o3 -emit-llvm -c a.c -o a.bc
opt -dot-callgraph a.bc
dot callgraph.dot > dot.callgraph.png


