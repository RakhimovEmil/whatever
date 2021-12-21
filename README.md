This project develops a non-template whatever class whose object can store a value of an arbitrary type (with a small caveat to the requirement 
to have a copy constructor). An object of the whatever class must allow to "safely" provide a value of that type upon request. "Safely" in this
case implies that if whatever contains an object of type T, then when queried:

 * of type T, the value will be provided;
 * of a type other than T, the user of whatever class will be informed of the error.

```
struct A
{
    A() = default;
    A(A const & a) : x(a.x + 1) {}
    A(A && a) = delete;

    int x = 0;
};
```

```
A a;
assert(a.x == 0);

whatever w1(a);
assert(whatever_cast<A>(&w1)->x == 1);

whatever w2;
w2 = w1;
assert(whatever_cast<A>(&w1)->x == 1);
assert(whatever_cast<A>(&w2)->x == 2);
```

The value is obtained by analogy with dynamic_cast().
