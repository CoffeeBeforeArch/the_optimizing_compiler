# Chapter 3 - Predication

Modern processors use branch prediction to keep superscalar pipelines saturated with instructions. However, some branches are difficult to predict, and cause serious performance problems.

However, there are situations where the compiler can remove a branch, and replace with with a predicated the instruction. While this is more expensive than a perfectly predicted branch, it is far less expensive than a frequently mis-predicted branch.

In this section, we'll be looking at how the compiler may replace branches with `cmov` instruction, and study the performance impacts.

## Costly Branches

A branch is costly if it is difficult to predict the outcome or target.

## Predicated Move Instructions

Some conditional jumps can be transformed into branch-less equivilants. The optimization passes `-fif-conversion` and `-fif-conversion2` are the primary optimization flags for doing this with GCC. These are enabled at all optimization levels except `-Og`.

