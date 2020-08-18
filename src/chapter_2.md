# Chapter 2 - Aliasing

When the compiler can not determine if two pointers (or references) refer to the same piece of memory, they are said to alias each other. When this happens, the compiler will be limited on the optimizations it can perform on parts of the code using those pointers. In this chapter, we'll look at how aliasing can lead to sub-optimal assembly, and then use benchmarks to quantify the impact on performance.

## The Canonical Aliasing Example

We'll start our discussion on aliasing by examining the canonical example of a `set_and_add` function. This function takes two pointers (or references) to values, sets the values to different constants, then returns the sum of the two new values. Let's look at an implementation that uses integers.

```cpp
int inc_and_add(int &a, int &b) {
  // Set the values of a and b
  a = 4;
  b = 5;

  // Return their sum
  return a + b;
}
```

It would be east to assume that this function always returns the value `9`. However, both `9` and `10` are valid results. To understand why, let's consider two different calls to our `set_and_add` function. In the first, we will create two integers, and pass them to `set_and_inc`.

```cpp
// Create two integers
int a = 0;
int b = 0;

// Set 'c' using set_and_inc
int c = set_and_inc(a, b);
```

After the function call, we would observe that `a == 4`, `b == 5`, and `c == 9`.

However, consider this equally valid way to call `set_and_inc` where we pass in the same integer for both function arguments.

```cpp
// Create an integers
int a = 0;

// Set 'c' using set_and_inc
int c = set_and_inc(a, a);
```

After this function call, we would observe that `a == 5`, and `c == 10`.

Our compiler doesn't know which way we intended to call our function. While the function author may never have intended `set_and_inc` to be called with the same integer for both arguments, our compiler must be conservative, and generate code assuming that it is a possibility.

Let's take a look at the generated assembly for out `set_and_inc` function.

```assembly
0000000000000000 <inc_and_add(int&, int&)>:
   0:	c7 07 04 00 00 00    	movl   $0x4,(%rdi)
   6:	c7 06 05 00 00 00    	movl   $0x5,(%rsi)
   c:	8b 07                	mov    (%rdi),%eax
   e:	83 c0 05             	add    $0x5,%eax
  11:	c3                   	retq   
```

Our first two instructions are `movl`, used to store the values to `a` and `b` respectively. Because the compiler does not know if the store to `b` has overwritten the value stored in `a` (if they are aliases), it inserts a third `mov` instruction to re-read the contents of `a`. Finally, the compiler adds `5` to `b`, and returns the result through the `eax` register.

We pay multiple prices for aliasing in this function. First, we have an extra instruction to re-load the value stored in `a`. Second, our compiler is unable to pre-compute the return result, because it doesn't know if it will be `9` or `10`.

In order to help our conservative compiler, we need to inform it that `a` and `b` will never alias each other. This can be done using the `__restrict` compiler intrinsic.

```cpp
int inc_and_add(int & __restrict a, int & __restrict b) {
  // Set the values of a and b
  a = 4;
  b = 5;

  // Return their sum
  return a + b;
}
```

Now our compiler will be able to optimize this function assuming that writes to `b` have no effect on `a` (and vice versa). Let's look at the assembly generated from our new `inc_and_add`.

```assembly
0000000000000000 <inc_and_add(int&, int&)>:
   0:	f3 0f 1e fa          	endbr64 
   4:	c7 07 04 00 00 00    	movl   $0x4,(%rdi)
   a:	b8 09 00 00 00       	mov    $0x9,%eax
   f:	c7 06 05 00 00 00    	movl   $0x5,(%rsi)
  15:	c3                   	retq   
```

Our compiler no longer re-loads the value stored in `a`, and it pre-computes the return value (`9`). `10` is no longer a valid return value from this function, because we have already specified that `a` and `b` can not alias each other.

## Type-Based Alias Analysis

## Additional Notes

