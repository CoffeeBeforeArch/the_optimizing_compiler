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

It would be east to assume that this function always returns the value `9`. However, both `9` and `10` are valid results. To understand why, let's consider two different calls to our `set_and_add` function. In the first, we will create integers, and pass them to `set_and_inc`.

```cpp
// Create two integers
int a = 0;
int b = 0;

// Set 'c' using set_and_inc
int c = set_and_inc(a, b);
```

After the function call, we would observe that `a == 4`, `b == 5`, and `c == 9`. However, consider this equally valid way to call `set_and_inc` where we pass in the same integer for both function arguments.

```cpp
// Create an integers
int a = 0;

// Set 'c' using set_and_inc
int c = set_and_inc(a, a);
```

After this function call, we would observe that `a == 5`, and `c == 10`.

```assembly
0000000000000000 <inc_and_add(int&, int&)>:
   0:	c7 07 04 00 00 00    	movl   $0x4,(%rdi)
   6:	c7 06 05 00 00 00    	movl   $0x5,(%rsi)
   c:	8b 07                	mov    (%rdi),%eax
   e:	83 c0 05             	add    $0x5,%eax
  11:	c3                   	retq   
```
