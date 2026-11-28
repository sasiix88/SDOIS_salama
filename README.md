# PPOIS Course

Laboratory assignments for the "Design of Intelligent Systems Software" course, by Salama Younis.

Language: **C++17**. Testing: **GoogleTest**. Build: **CMake** (works from the console, no IDE required).

## Repository structure

- `lab1/` - OOP fundamentals: two classes with unit tests, coverage, Doxygen docs and a console menu app
- `lab2/` - (in progress)
- `lab3/` - (in progress)
- `lab4/` - (in progress)

## Lab 1 variants

- **1.3 Rectangle** - an axis-aligned rectangle with integer vertex coordinates: coordinate access, moving, resizing, pre/post increment/decrement, bounding union (`+`, `+=`), intersection (`-`, `-=`), equality, stream I/O.
- **1.4.1 Set** - an unordered Cantor set whose elements may be nested sets: emptiness check, add/remove, cardinality, membership (`[]`), union/intersection/difference (`+`, `*`, `-` and their compound forms), power set, construction from a string like `{a, b, {a, b}, {}}`.

## Building and running (console only)

Each lab is self-contained:

```bash
cd lab1
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Run the console menu applications:

```bash
./build/rectangle/RectangleApp
./build/set/SetApp
```

## Tests and coverage

```bash
cd lab1
cmake -S . -B build -DENABLE_COVERAGE=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
cmake --build build --target coverage     # gcovr, fails below 90% line coverage
cmake --build build --target test_report  # HTML test report
cmake --build build --target docs         # Doxygen documentation
```

## CI/CD

GitHub Actions (`.github/workflows/`) automatically on every push and pull request:

- builds the project and runs the unit tests;
- checks that line coverage is over 90%;
- runs cppcheck static analysis;
- generates the Doxygen documentation and uploads it with the test and coverage reports;
- publishes a downloadable Linux release artifact (binaries, docs and sources) to the repository's Releases section.
