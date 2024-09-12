# Overview

Design and implement an LLVM pass that computes "anticipated expressions" using
dataflow analysis and performs code hoisting using the computed information to
reduce code size and repeated execution of expressions. The pass will be tested
by checking if anticipated expressions have been eliminated from the IR.
