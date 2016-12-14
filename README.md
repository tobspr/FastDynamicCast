# Fast dynamic cast

This is a dynamic cast implementation which outperforms the regular `dynamic_cast` by up to 25 times.


# Performance

All performance tests ran on 2 Xeons E5-2623, but should be representative for lower end systems too.
I ran each iteration count multiple times, and took the average.
Notice that all performance tests are more or less best-case scenarios.

## Simple class hierarchy

<img src="img/chart_simple.png" />


## Complex class hierarchy

<img src="img/chart_complex.png" />


## Speedup vs regular dynamic_cast

<img src="img/chart_speedup.png" />


# Usage

Copy the `fast_dynamic_cast.h` header to your project, and change your `dynamic_cast` calls to `fast_dynamic_cast`, e.g.:

```cpp
A a;
B& a_as_b = a;
A& a_as_a = fast_dynamic_cast<A&>(a_as_b);
```

The syntax is identical to the regular dynamic cast.

The dynamic cast implementation only works on MSVC 2013 or newer. For all other compilers,
it will fallback to the default dynamic cast.

