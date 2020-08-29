# Chapter 3 - Predication

Modern processors use branch prediction to keep superscalar pipelines saturated with instructions. However, some branches are difficult to predict, and cause serious performance problems.

However, there are situations where the compiler can remove a branch, and replace with with a predicated the instruction. While this is more expensive than a perfectly predicted branch, it is far less expensive than a frequently mis-predicted branch.

In this section, we'll be looking at how the compiler may replace branches with `cmov` instruction, and study the performance impacts.

## Costly Branches

Modern branch predictors can accurately predict branch outcomes for relatively complex patterns. However, they have a difficult time if the input data is random, and the branch outcomes are not bised (e.g., mostly takes or mostly not taken). Let's take a look at a simple function that conditionally adds an input based on a condition.

```cpp
void conditional_add(int &sum, int val, bool b) {
  if (b)
    return sum += val;
}
```

Compiling this, we get the following assembly:

```assembly
0000000000000000 <conditional_add(int&, int, bool)>:
   0:	84 d2                	test   %dl,%dl
   2:	74 02                	je     6 <conditional_add(int&, int, bool)+0x6>
   4:	01 37                	add    %esi,(%rdi)
   6:	c3                   	retq
```

## Predicated Move Instructions

Some conditional jumps can be transformed into branch-less equivilants. The optimization passes `-fif-conversion` and `-fif-conversion2` are the primary optimization flags for doing this with GCC. These are enabled at all optimization levels except `-Og`.

What we're looking for is the compiler to use `cmov` (conditional move) instrution instead of a breach. Unlike a branch, a `cmov` instruction cano not be mispredicted. We can re-write our `conditional_add` function to coax the compiler into using a `cmov`: 

```cpp
void conditional_add(int &sum, int val, bool b) {
  sum += (b) ? val : 0;
}
```

This generates the following assembly:

```assembly
0000000000000000 <conditional_add(int&, int, bool)>:
   0:	8b 07                	mov    (%rdi),%eax
   2:	01 c6                	add    %eax,%esi
   4:	84 d2                	test   %dl,%dl
   6:	0f 45 c6             	cmovne %esi,%eax
   9:	89 07                	mov    %eax,(%rdi)
   b:	c3                   	retq
```

If we re-compile this with `-fno-if-conversion`, we get the following assembly:

```assembly
0000000000000000 <conditional_add(int&, int, bool)>:
   0:	8b 07                	mov    (%rdi),%eax
   2:	84 d2                	test   %dl,%dl
   4:	74 02                	je     8 <conditional_add(int&, int, bool)+0x8>
   6:	01 f0                	add    %esi,%eax
   8:	89 07                	mov    %eax,(%rdi)
   a:	c3                   	retq   
```
