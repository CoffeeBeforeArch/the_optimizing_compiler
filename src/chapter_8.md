# Chapter 8 - Link-Time Optimization (LTO)

A major challenge that compilers face is a lack of context about what happens at runtime. However, compilers also face the challenge of a lack of context of what is going on between translation units. When source files are compiled separately, interprocedural optimizations can not occur. However, modern link time optimization allows use to pay the price of extra build time for gaining back optimizations across translation units.

## Alias Analysis

