# Chapter 1 - Introduction

The role of the compiler is to translate code written in one language (like C or C++) to another (like x86 assembly) while maintaining functional correctness. Modern optimizing compilers go even further, and perform numerous optimizations that dramatically increase the performance of our applications. Understanding some of these optimizations is core goal of this book.

## Why Should You Care About Optimizing Compilers?

If you care about performance, then you care about the code your compiler generates. Understanding the capabilities and limitations of your compiler can (and should) influence design decisions in high-level software.

## Background

This book assumes that the reader is familiar with C++ and the basic steps of compilation. Additionally, the reader should have a basic understanding of how assembly looks/functions.

However, this book takes a practical apprach to reading and understanding assembly. We will not be examining every line of assembly in every example. What we will be doing is extracting the important snippets, and analyzing them in detail, similar to who you would approach trying to understand a large codebase you are unfamiliar with.

