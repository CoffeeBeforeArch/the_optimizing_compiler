# Chapter 4 - Biased Branches

There are situations where the programmer knows that a branch is is mostly taken, or mostly not taken. However, the compiler is largely blind to this because it only has information available at compile time. To bridge this gap, we can use compiler intrinsics to give hints to our compiler about how biased a branch is so it can make better scheduling decisions.

## Compiler Hints

