# Fixing clangd C++ Standard Library Resolution

## Problem

clangd (the C++ language server) cannot resolve standard library symbols like `std::vector`, showing errors for both `std` namespace and standard headers like `<vector>`.

## Root Cause

clangd may look for the wrong GCC version's C++ headers. For example, clangd 14 might default to looking for GCC 12 headers when you actually have GCC 11 installed, causing it to fail to find standard library headers.

## Solution

### Step 1: Enable compile_commands.json Generation

Add this to your `CMakeLists.txt` after setting the C++ standard:

```cmake
# Export compile commands for language servers (clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

### Step 2: Generate compile_commands.json

```bash
cmake -B build -S .
```

### Step 3: Create Symlink in Project Root

```bash
ln -sf build/compile_commands.json compile_commands.json
```

### Step 4: Find Your System's C++ Include Paths

```bash
g++ -E -x c++ - -v < /dev/null 2>&1 | grep -A 20 "search starts here"
```

This will output something like:
```
#include <...> search starts here:
 /usr/include/c++/11
 /usr/include/x86_64-linux-gnu/c++/11
 /usr/include/c++/11/backward
 ...
```

Note the C++ version number (e.g., `11`).

### Step 5: Create .clangd Configuration File

Create a `.clangd` file in your project root with the following content, adjusting the version number to match your system:

```yaml
CompileFlags:
  Add:
    - "-std=c++17"
    - "-I/path/to/your/project/include"
    - "--gcc-toolchain=/usr"
    - "-isystem/usr/include/c++/11"
    - "-isystem/usr/include/x86_64-linux-gnu/c++/11"
    - "-isystem/usr/include/c++/11/backward"
  CompilationDatabase: build

Diagnostics:
  UnusedIncludes: Strict
```

Replace:
- `/path/to/your/project/include` with your actual include directory path
- `11` with your GCC version number from Step 4

### Step 6: Clear clangd Cache

```bash
rm -rf ~/.cache/clangd/index/*
```

### Step 7: Restart Your Editor

Completely restart nvim (or your editor) to reload the language server.

## Verification

Test that clangd can now parse your files correctly:

```bash
clangd --check=path/to/your/header.h
```

You should see `All checks completed, 0 errors` at the end of the output.

## Additional Notes

- For header files (`.h`/`.hpp`), ensure they are included by at least one `.cpp` file in your project so clangd knows how to compile them
- Add proper header guards to all header files
- The `.clangd` file should be added to your `.gitignore` if paths are system-specific, or committed if the paths are portable

## Example .clangd for Different Systems

### Ubuntu 22.04 with GCC 11:
```yaml
CompileFlags:
  Add:
    - "-std=c++17"
    - "--gcc-toolchain=/usr"
    - "-isystem/usr/include/c++/11"
    - "-isystem/usr/include/x86_64-linux-gnu/c++/11"
    - "-isystem/usr/include/c++/11/backward"
  CompilationDatabase: build
```

### Newer Systems with GCC 12:
```yaml
CompileFlags:
  Add:
    - "-std=c++17"
    - "--gcc-toolchain=/usr"
    - "-isystem/usr/include/c++/12"
    - "-isystem/usr/include/x86_64-linux-gnu/c++/12"
    - "-isystem/usr/include/c++/12/backward"
  CompilationDatabase: build
```

### macOS with Clang:
On macOS, clangd usually works out of the box with Xcode command line tools. If you still have issues, you may need:

```yaml
CompileFlags:
  Add:
    - "-std=c++17"
  CompilationDatabase: build
```
