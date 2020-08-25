# Chapter 3 - Predication

Modern processors use branch prediction to keep superscalar pipelines saturated with instructions. However, some branches are difficult to predict, and cause serious performance problems.

However, there are situations where the compiler can remove a branch, and replace with with a predicated the instruction. While this is more expensive than a perfectly predicted branch, it is far less expensive than a frequently mis-predicted branch.

In this section, we'll be looking at how the compiler may replace branches with `cmov` instruction, and study the performance impacts.

## Costly Branches

## Predicated Move Instructions

