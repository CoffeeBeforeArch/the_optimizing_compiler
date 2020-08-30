# Chapter 3 - Predication

Modern processors use branch prediction to keep superscalar pipelines saturated with instructions. However, some branches are difficult to predict, and cause serious performance problems.

However, there are situations where the compiler can remove a branch, and replace with with a predicated the instruction. While this is more expensive than a perfectly predicted branch, it is far less expensive than a frequently mis-predicted branch.

In this section, we'll be looking at how the compiler may replace branches with `cmov` instruction, and study the performance impacts.

## Costly Branches

Modern branch predictors can accurately predict branch outcomes for relatively complex patterns. However, they have a difficult time if the input data is random, and the branch outcomes are not bised (e.g., mostly takes or mostly not taken). Let's take a look at a simple function that conditionally adds an input based on a condition.

```cpp
void conditional_add(int &sum, int input, bool b) {
  if (b)
    sum += input;
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

This is not by any means bad code. However, it can perform poorly if our `je` (jump if equal) instruction is poorly predicted by our branch predictor.

## Predicated Move Instructions

Some conditional jumps can be transformed into branch-less equivilants. The optimization passes `-fif-conversion` and `-fif-conversion2` are the primary optimization flags for doing this with GCC. These are enabled at all optimization levels except `-Og`.

Our goal is for the compiler to use `cmov` (conditional move) instrution instead of a branch. Unlike a branch, a `cmov` instruction can not be mispredicted. We can re-write our `conditional_add` function as follows to coax the compiler into using a `cmov`: 

```cpp
void conditional_add(int &sum, int val, bool b) {
  int tmp = 0;
  if (b)
    tmp = val;
  sum += temp;
}
```

Instead of doing nothing when the condition is `false`, our compiler will now add `0` to our `sum`.

This generates the following assembly:

```assembly
0000000000000000 <conditional_add(int&, int, bool)>:
   0:	84 d2                	test   %dl,%dl
   2:	b8 00 00 00 00       	mov    $0x0,%eax
   7:	0f 44 f0             	cmove  %eax,%esi
   a:	01 37                	add    %esi,(%rdi)
   c:	c3                   	retq   
```

Our compiler now selects between `0` and `val` using a `cmove` instruction.

If we re-compile this with `-fno-if-conversion` (to disable the conversion to a branchless equivilant), we get the following assembly:

```assembly
0000000000000000 <conditional_add(int&, int, bool)>:
   0:	84 d2                	test   %dl,%dl
   2:	75 02                	jne    6 <conditional_add(int&, int, bool)+0x6>
   4:	31 f6                	xor    %esi,%esi
   6:	01 37                	add    %esi,(%rdi)
   8:	c3                   	retq
```

Now the compiler selects between adding `0` and `val` using a `jne` (jump if not equal) instruction.
