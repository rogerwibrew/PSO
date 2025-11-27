# Polymorphism in C++

## Virtual Destructors

### Why Use a Virtual Destructor?

When you have a polymorphic class hierarchy (classes with virtual functions that you'll inherit from), you need a **virtual destructor** to ensure proper cleanup.

**The problem without virtual destructor**:

```cpp
Problem* p = new SphereProblem(2);  // Pointer to base class
delete p;  // Without virtual destructor, only ~Problem() is called!
           // ~SphereProblem() is NEVER called - memory leak!
```

**With virtual destructor**:

```cpp
Problem* p = new SphereProblem(2);  // Pointer to base class
delete p;  // Calls ~SphereProblem() first, then ~Problem()
           // Proper cleanup of derived class resources
```

The virtual destructor ensures the **correct destructor is called** based on the actual object type, not just the pointer type.

### Why `= default`?

`= default` tells the compiler to generate the default implementation for you.

**For destructors specifically**:

```cpp
virtual ~Problem() = default;
```

This means:

- "This destructor is virtual" (for polymorphism)
- "But I don't need to do any special cleanup, so just use the compiler-generated version"

**Equivalent to**:

```cpp
virtual ~Problem() {}  // Empty destructor body
```

But `= default` is:

- **More explicit** - clearly communicates intent
- **More efficient** - compiler can optimize better
- **Safer** - maintains trivial/standard-layout properties when possible

### Why We Need It

Since `Problem` is an abstract base class with no member variables that need special cleanup, we don't need custom destructor logic. But we still need it to be **virtual** for polymorphic deletion to work correctly.

**Rule of thumb**: In any base class with virtual functions, always declare the destructor virtual (even if it's `= default`) to prevent resource leaks when deleting through base class pointers.

## Pure Virtual Functions

### What is a Pure Virtual Function?

A pure virtual function is declared with `= 0` at the end:

```cpp
virtual double evaluate(const std::vector<double>& x) const = 0;
```

This means:

- The function **must** be overridden in derived classes
- The base class cannot be instantiated (it's abstract)
- The derived class must provide an implementation

### Abstract Base Classes

A class with at least one pure virtual function is called an **abstract base class**:

```cpp
class Problem {
public:
    virtual ~Problem() = default;
    virtual double evaluate(const std::vector<double>& x) const = 0;  // Pure virtual
    virtual int getDimensions() const = 0;  // Pure virtual
    // ... more pure virtual functions
};

// This won't compile:
// Problem p;  // Error: cannot instantiate abstract class

// But this works:
class SphereProblem : public Problem {
public:
    // Must implement all pure virtual functions
    double evaluate(const std::vector<double>& x) const override { /* ... */ }
    int getDimensions() const override { /* ... */ }
    // ...
};

SphereProblem sp(2);  // OK - SphereProblem is concrete
```

### Benefits of Abstract Base Classes

1. **Interface definition** - Defines a contract that all derived classes must follow
2. **Polymorphism** - Allows working with different problem types through a common interface
3. **Type safety** - Compiler enforces that derived classes implement all required methods
4. **Extensibility** - Easy to add new problem types without changing existing code

## The `override` Keyword

When implementing virtual functions in derived classes, use the `override` keyword:

```cpp
class SphereProblem : public Problem {
public:
    double evaluate(const std::vector<double>& x) const override;  // Good
    // double evaluate(const std::vector<double>& x) const;  // Works but less safe
};
```

**Benefits of `override`**:

- **Catches typos** - Compiler error if the base class doesn't have this virtual function
- **Catches signature mismatches** - Error if parameters don't match exactly
- **Self-documenting** - Clearly shows this overrides a base class method
- **Refactoring safety** - If base class signature changes, you get a compile error

## Polymorphic Usage Example

```cpp
// Create different problem instances
SphereProblem sphere(2);
RosenbrockProblem rosenbrock(2);

// Store in base class pointer
Problem* problem = &sphere;

// Call virtual function - dispatched to SphereProblem::evaluate()
double fitness = problem->evaluate({1.0, 2.0});

// Can change which problem we're working with
problem = &rosenbrock;

// Same code, different behavior - that's polymorphism!
fitness = problem->evaluate({1.0, 2.0});
```

## Smart Pointers with Polymorphism

In modern C++, prefer smart pointers over raw pointers:

```cpp
#include <memory>

// Using unique_ptr
std::unique_ptr<Problem> problem = std::make_unique<SphereProblem>(2);
double fitness = problem->evaluate({1.0, 2.0});

// Using shared_ptr (for shared ownership)
std::shared_ptr<Problem> shared_problem = std::make_shared<SphereProblem>(2);

// Virtual destructor ensures proper cleanup when smart pointer goes out of scope
// No need for manual delete!
```

## Common Pitfalls

### Forgetting Virtual Destructor

```cpp
// BAD - no virtual destructor
class Base {
public:
    virtual void foo() = 0;
    // ~Base() = default;  // Should be virtual!
};

class Derived : public Base {
public:
    void foo() override {}
    ~Derived() { /* cleanup */ }  // This won't be called if deleting through Base*
};
```

### Slicing

```cpp
Problem problem = SphereProblem(2);  // BAD - object slicing!
// Only the Problem part is copied, derived class data is lost

Problem* problem = new SphereProblem(2);  // Good - pointer to derived
std::unique_ptr<Problem> problem = std::make_unique<SphereProblem>(2);  // Better
```

### Forgetting `const` in Override

```cpp
class Base {
public:
    virtual void foo() const = 0;
};

class Derived : public Base {
public:
    // BAD - missing const, doesn't override!
    void foo() override { }  // Compile error due to override keyword

    // Good
    void foo() const override { }
};
```

## Best Practices

1. **Always use `virtual` destructor** in base classes with virtual functions
2. **Use `= 0`** to make functions pure virtual when there's no sensible default implementation
3. **Use `override`** keyword in derived classes to catch errors
4. **Use `final`** keyword to prevent further overriding if needed
5. **Prefer smart pointers** over raw pointers for ownership
6. **Use `const` correctly** - especially in interface methods
7. **Keep interfaces minimal** - only make functions virtual if needed for polymorphism

## Additional Resources

- C++ Core Guidelines: [C.35](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-dtor-virtual) - Virtual destructors
- C++ Core Guidelines: [C.128](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override) - Use override
- Effective C++ by Scott Meyers - Items on inheritance and virtual functions