# Exception Handling in C++

## What Are Exceptions?

Exceptions are a mechanism for handling errors and exceptional conditions in C++. Instead of returning error codes or using special sentinel values, you can **throw** an exception when something goes wrong, and **catch** it where you can handle it appropriately.

## Basic Exception Syntax

### Throwing an Exception

```cpp
#include <stdexcept>

void setAge(int age) {
    if (age < 0) {
        throw std::invalid_argument("Age cannot be negative");
    }
    // ... rest of function
}
```

### Catching an Exception

```cpp
try {
    setAge(-5);  // This will throw
    // Code here won't execute if exception is thrown
} catch (const std::invalid_argument& e) {
    std::cout << "Error: " << e.what() << std::endl;
    // Handle the error
}
```

### The Flow

1. When `throw` is executed, the function immediately stops
2. The stack **unwinds** (destructors are called for local objects)
3. Control jumps to the nearest matching `catch` block
4. If no `catch` block is found, the program terminates

## Standard Exception Classes

C++ provides a hierarchy of standard exception classes in `<stdexcept>`:

```text
std::exception (base class)
├── std::logic_error
│   ├── std::invalid_argument    // Invalid argument passed to function
│   ├── std::domain_error        // Mathematical domain error
│   ├── std::length_error        // Attempted to exceed maximum allowed size
│   ├── std::out_of_range        // Access outside valid range
│   └── std::future_error        // Future/promise error
└── std::runtime_error
    ├── std::range_error         // Range error in internal computation
    ├── std::overflow_error      // Arithmetic overflow
    ├── std::underflow_error     // Arithmetic underflow
    └── std::system_error        // Operating system error
```

### When to Use Which Exception

**`std::invalid_argument`** - Parameter validation fails

```cpp
SphereProblem::SphereProblem(int dimensions) {
    if (dimensions <= 0) {
        throw std::invalid_argument("Dimensions must be greater than 0");
    }
}
```

**`std::out_of_range`** - Index or access out of bounds

```cpp
double getElement(const std::vector<double>& vec, size_t index) {
    if (index >= vec.size()) {
        throw std::out_of_range("Index out of range");
    }
    return vec[index];
}
```

**`std::logic_error`** - Programming logic error (preventable)

```cpp
void setConfiguration(bool initialized) {
    if (!initialized) {
        throw std::logic_error("Must initialize before configuration");
    }
}
```

**`std::runtime_error`** - Runtime error (not preventable by programmer)

```cpp
void openFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}
```

## Complete Exception Handling Example

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>

class SphereProblem {
public:
    SphereProblem(int dimensions) : dimensions_(dimensions) {
        if (dimensions <= 0) {
            throw std::invalid_argument("Dimensions must be greater than 0");
        }
    }

    double evaluate(const std::vector<double>& x) const {
        if (static_cast<int>(x.size()) != dimensions_) {
            throw std::invalid_argument(
                "Position vector size must match problem dimensions");
        }

        double sum = 0.0;
        for (double val : x) {
            sum += val * val;
        }
        return sum;
    }

private:
    int dimensions_;
};

int main() {
    try {
        // This will throw - invalid dimensions
        SphereProblem bad_problem(-5);

    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid argument: " << e.what() << std::endl;
    }

    try {
        SphereProblem problem(2);

        // This will throw - wrong size vector
        std::vector<double> pos = {1.0, 2.0, 3.0};  // 3D, but problem is 2D
        double fitness = problem.evaluate(pos);

    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid argument: " << e.what() << std::endl;
    }

    // This works fine
    try {
        SphereProblem problem(2);
        std::vector<double> pos = {1.0, 2.0};
        double fitness = problem.evaluate(pos);
        std::cout << "Fitness: " << fitness << std::endl;  // Prints: Fitness: 5.0

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

## Catching Multiple Exception Types

```cpp
try {
    // ... code that might throw

} catch (const std::invalid_argument& e) {
    // Handle invalid argument specifically
    std::cerr << "Invalid argument: " << e.what() << std::endl;

} catch (const std::out_of_range& e) {
    // Handle out of range specifically
    std::cerr << "Out of range: " << e.what() << std::endl;

} catch (const std::exception& e) {
    // Catch all other standard exceptions
    std::cerr << "Error: " << e.what() << std::endl;

} catch (...) {
    // Catch anything else (not recommended)
    std::cerr << "Unknown error occurred" << std::endl;
}
```

**Order matters!** Catch more specific exceptions first, then more general ones.

## The `what()` Method

All standard exceptions inherit from `std::exception` and provide a `what()` method:

```cpp
try {
    throw std::invalid_argument("Something went wrong");
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;  // Prints: Something went wrong
}
```

## Exception Safety Guarantees

When writing exception-safe code, aim for these guarantees:

### 1. Basic Guarantee

- No memory leaks
- Objects remain in valid state (even if changed)

```cpp
void process() {
    std::vector<int> data;
    data.push_back(1);
    data.push_back(2);
    riskyOperation();  // If this throws, data is still valid
}
```

### 2. Strong Guarantee (Commit or Rollback)

- Either the operation succeeds completely, or has no effect
- Original state is preserved if exception is thrown

```cpp
void updateBoth(std::vector<int>& v1, std::vector<int>& v2) {
    auto temp1 = v1;  // Copy
    auto temp2 = v2;  // Copy

    temp1.push_back(1);  // Modify copies
    temp2.push_back(2);

    v1 = std::move(temp1);  // Only commit if all succeeded
    v2 = std::move(temp2);
}
```

### 3. No-throw Guarantee

- Operation never throws exceptions
- Mark with `noexcept` keyword

```cpp
int getValue() noexcept {
    return value_;
}

void swap(MyClass& other) noexcept {
    std::swap(data_, other.data_);
}
```

## RAII and Exception Safety

**RAII (Resource Acquisition Is Initialization)** ensures resources are properly cleaned up even when exceptions are thrown:

```cpp
void processFile() {
    std::ifstream file("data.txt");  // Resource acquired in constructor

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    // ... process file ...

    // File automatically closed when 'file' goes out of scope
    // Even if an exception is thrown above!
}
```

**Smart pointers** provide automatic memory management:

```cpp
void processData() {
    std::unique_ptr<int[]> data(new int[1000]);  // RAII

    riskyOperation();  // If this throws, data is automatically deleted

    // No need for explicit delete
}
```

## Best Practices

### 1. Throw by Value, Catch by Const Reference

```cpp
// Good
throw std::invalid_argument("Error message");

// Good
catch (const std::invalid_argument& e) { }

// Bad - can cause slicing
catch (std::exception e) { }  // Missing reference!

// Bad - unnecessary copy
catch (std::exception& e) { }  // Should be const&
```

### 2. Don't Throw in Destructors

```cpp
class MyClass {
public:
    ~MyClass() noexcept {  // Destructors should be noexcept
        // Don't throw exceptions here!
        // If destructor runs during stack unwinding and throws,
        // std::terminate() is called
    }
};
```

### 3. Use Standard Exceptions When Possible

```cpp
// Good - use standard exception
throw std::invalid_argument("Invalid dimensions");

// Less good - custom exception (only if you need special behavior)
class InvalidDimensionsError : public std::invalid_argument {
public:
    InvalidDimensionsError(int dims)
        : std::invalid_argument("Invalid dimensions"), dimensions_(dims) {}
    int getDimensions() const { return dimensions_; }
private:
    int dimensions_;
};
```

### 4. Document Exceptions in Comments

```cpp
/**
 * @brief Construct a sphere problem
 * @param dimensions Number of dimensions (must be > 0)
 * @throws std::invalid_argument if dimensions <= 0
 */
SphereProblem(int dimensions);
```

### 5. Let Exceptions Propagate

Don't catch exceptions unless you can actually handle them:

```cpp
// Bad - catching and ignoring
try {
    problem.evaluate(x);
} catch (...) {
    // Silently ignore - BAD!
}

// Good - let it propagate to caller who can handle it
double fitness = problem.evaluate(x);  // No try/catch here

// Good - catch only to add context, then rethrow
try {
    problem.evaluate(x);
} catch (const std::exception& e) {
    std::cerr << "Error in iteration " << i << ": " << e.what() << std::endl;
    throw;  // Rethrow the same exception
}
```

## When NOT to Use Exceptions

### 1. Expected Conditions

```cpp
// Bad - exceptions for normal flow control
try {
    int value = findValue(key);
} catch (const std::out_of_range&) {
    value = defaultValue;
}

// Good - use optional or special return value
std::optional<int> findValue(const std::string& key);
```

### 2. Performance-Critical Code

Exceptions have overhead. For tight loops, consider alternative error handling:

```cpp
// In tight loop - validate once before loop
if (!isValid(input)) {
    throw std::invalid_argument("Invalid input");
}

for (int i = 0; i < 1000000; ++i) {
    // Don't throw exceptions here
}
```

## Testing Exception Behavior

With GoogleTest, you can test that exceptions are thrown correctly:

```cpp
#include <gtest/gtest.h>

TEST(SphereProblemTest, InvalidDimensionsThrows) {
    EXPECT_THROW(SphereProblem(-1), std::invalid_argument);
    EXPECT_THROW(SphereProblem(0), std::invalid_argument);
}

TEST(SphereProblemTest, ValidDimensionsDoesNotThrow) {
    EXPECT_NO_THROW(SphereProblem(2));
    EXPECT_NO_THROW(SphereProblem(10));
}

TEST(SphereProblemTest, ThrowsCorrectMessage) {
    try {
        SphereProblem(-1);
        FAIL() << "Expected std::invalid_argument";
    } catch (const std::invalid_argument& e) {
        EXPECT_STREQ("Dimensions must be greater than 0", e.what());
    }
}
```

## Summary

**Key points to remember:**

1. Use exceptions for **exceptional conditions**, not normal flow control
2. Throw **specific** exception types that match the error
3. Catch by **const reference**: `catch (const std::exception& e)`
4. Always use **RAII** for resource management
5. Destructors should **never throw** (mark them `noexcept`)
6. Document what exceptions your functions can throw
7. Let exceptions **propagate** unless you can actually handle them
8. Use **standard exceptions** when possible

## Additional Resources

- [C++ Core Guidelines: Error Handling](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-errors)
- [cppreference: std::exception](https://en.cppreference.com/w/cpp/error/exception)
- Effective C++ by Scott Meyers - Items on exception safety