# C++ for the C/Java Programmer — A Delta Curriculum

You already have pointers and manual memory (C), and OOP, generics, and exceptions (Java).
This curriculum deliberately **skips** what you know and drills the parts of C++ that are
genuinely new or behave differently. Roughly 12–15 weeks at one module/week (the last is optional).

## Resources used (get these once)

- **Video:** The Cherno — [*C++* playlist on YouTube](https://www.youtube.com/playlist?list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb). Individual videos are linked per module below as direct watch links (the one exception is `std::string_view`, which has no dedicated Cherno video). The playlist isn't ordered like this curriculum, so follow the module links, not the playlist sequence.
- **Standard reading — primary:** Bjarne Stroustrup, *A Tour of C++*, **3rd ed. (covers C++20)**. Written for experienced programmers — ~250 pages, chapter-per-topic. Read by chapter, never cover-to-cover in one go.
- **Standard reading — depth:** Scott Meyers, *Effective Modern C++* (C++11/14), and a few items from the classic *Effective C++*. Read by **Item** where flagged.
- **Free deep-dive reading:** learncpp.com — referenced by topic (use its table of contents; section numbers shift between site updates).
- **Optional "what the standard actually says":** **cppreference.com**. Pages flagged per module are linked directly. It's a wiki, so no page numbers — each topic is one page, and every page ends with a **References** section citing the exact ISO clause, so you get the spec pointer for free. Optional precision reading, not where understanding comes from.
- **Standing style reference:** the **C++ Core Guidelines** (Stroustrup & Sutter, [`isocpp.github.io/CppCoreGuidelines`](https://isocpp.github.io/CppCoreGuidelines)) — consult for "what's idiomatic." You'll lint against it in Module 11.

Compile everything with a modern standard from day one: `g++ -std=c++20 -Wall -Wextra file.cpp` (or clang). Use `-fsanitize=address,undefined` while learning — it catches your C-habit memory bugs instantly.

### The "rebuild the STL" thread

A spine that runs through the curriculum: you reimplement a standard container, then diff your version against the real one. Order:

1. **`Vector<T>`** — Module 8 (growth, templates, move).
2. **`List<T>`** doubly linked — Module 9 (node ownership + writing a real iterator).
3. **`Stack<T>`** adaptor on top of your own `Vector<T>` — Module 9 (the container-adaptor pattern, cheaply).
4. **`HashMap<K,V>`** with chaining — Module 9 capstone (buckets, load factor; bridges into the final project).
5. *Stretch:* **`Optional<T>`** via aligned storage + placement `new` — Module 10 (manual lifetime management).

---

## Module 0 — Orientation, toolchain & undefined behavior (½ week, mostly skim)

You know compilation and linking from C. Goal here is C++'s spin on it — plus naming the hazard Java protected you from.

**Learning outcomes**

- Explain translation units, the One Definition Rule (ODR), and why declarations go in headers.
- Set up a build with a chosen standard flag and run a sanitizer build.
- Define undefined behavior and recognize that, unlike Java, the language gives you no safety net.

**Watch:** [How C++ Works](https://www.youtube.com/watch?v=SfGuIVzE_Os), [How the C++ Compiler Works](https://www.youtube.com/watch?v=3tIqpEmWMLI), [How the C++ Linker Works](https://www.youtube.com/watch?v=H4s55GgAg0I), [C++ Header Files](https://www.youtube.com/watch?v=9RJTQmK0YPI).

**Read:** *Tour of C++* Ch. 3 (Modularity).

**Optional (cppreference):** ODR — [`/w/cpp/language/definition`](https://en.cppreference.com/w/cpp/language/definition); translation phases — [`/w/cpp/language/translation_phases`](https://en.cppreference.com/w/cpp/language/translation_phases); undefined behavior — [`/w/cpp/language/ub`](https://en.cppreference.com/w/cpp/language/ub).

**Do:**

1. Take a small C program of yours, compile it as C++ with `-std=c++20 -Wall -Wextra`, fix every warning, and note which C idioms C++ rejects.
2. Deliberately write three classic undefined behaviors in separate tiny programs — a use-after-free, a signed-integer overflow, and an out-of-bounds array read — build each with `-fsanitize=address,undefined`, and confirm the sanitizer pinpoints each. This builds the reflex you'll rely on all curriculum.

---

## Module 1 — Value semantics, the object model & views (the #1 mental shift from Java)

In Java almost everything is a reference to a heap object. In C++ objects are **values** with a location and lifetime. This is where Java programmers stumble most — spend real time here.

**Learning outcomes**

- Predict whether an object lives on the stack or heap and when it is destroyed.
- Explain copy vs alias, and reference (`T&`) vs pointer (`T*`).
- Pass by value vs by `const&` correctly and say why.
- Use a non-owning **view** (`string_view`, `span`) and explain the dangling-view trap it introduces.

**Watch:** [Pointers in C++](https://www.youtube.com/watch?v=DTxHyVn0ODg), [References in C++](https://www.youtube.com/watch?v=IzoFn3dfsPA), [Object Lifetime in C++ (Stack/Scope Lifetimes)](https://www.youtube.com/watch?v=iNuTwvD6ciI), [Stack vs Heap Memory in C++](https://www.youtube.com/watch?v=wJ1L2nSIV1s), [std::string_view](https://www.youtube.com/results?search_query=The+Cherno+std%3A%3Astring_view).

**Read:** *Tour of C++* Ch. 1 (The Basics) + Ch. 2 (User-Defined Types). learncpp.com: [lvalue references](https://www.learncpp.com/cpp-tutorial/lvalue-references/) and pass by [reference](https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/) / [address](https://www.learncpp.com/cpp-tutorial/pass-by-address/).

**Optional (cppreference):** value categories — [`/w/cpp/language/value_category`](https://en.cppreference.com/w/cpp/language/value_category); references — [`/w/cpp/language/reference`](https://en.cppreference.com/w/cpp/language/reference); pointers — [`/w/cpp/language/pointer`](https://en.cppreference.com/w/cpp/language/pointer); storage duration — [`/w/cpp/language/storage_duration`](https://en.cppreference.com/w/cpp/language/storage_duration); string_view — [`/w/cpp/string/basic_string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view); span — [`/w/cpp/container/span`](https://en.cppreference.com/w/cpp/container/span).

**Do:**

1. Define a small `struct BigStruct`, then write `void byValue(BigStruct)`, `void byRef(BigStruct&)`, and `void byConstRef(const BigStruct&)`. Inside each, print the parameter's address with `&param`, and print the argument's address at the call site. The by-value parameter prints a *different* address (it's a separate copy); both reference parameters share the argument's address (they're aliases) — no copy constructor needed, just addresses you already know from C. (In Module 3 you'll instrument the copy constructor itself to literally count the copies.)
2. Make a small `Tracked` struct that prints its name in its destructor. In `main`, declare two `Tracked` objects, then a third inside a nested `{ }` block, then a fourth after the block. **Write the destruction order down before running**, then run and compare — the prediction is the exercise, and any mismatch is the lesson. (The destructor here is just a one-line lifetime *probe* — the "Object Lifetime" video shows exactly this; you'll cover destructors fully in Module 2.)
3. **Views:** write a function taking `std::string_view`, then deliberately build a dangling view (return a `string_view` into a temporary `std::string`), watch the sanitizer catch the use-after-free, and fix it. State the ownership rule a view does *not* give you.

**You know this when:** you can read a function signature and immediately know whether it copies, aliases, borrows (view), or takes ownership.

---

## Module 2 — RAII, deterministic destruction & exception safety (the defining idea of C++)

No garbage collector. Cleanup is tied to scope — and it generalizes to *all* resources (files, locks, sockets), not just memory. Exception safety is the discipline that makes it bulletproof.

**Learning outcomes**

- Define RAII and implement a class whose destructor releases a resource.
- Explain why Java's `try/finally` is largely unnecessary in C++.
- State the basic / strong / nothrow exception-safety guarantees and what `noexcept` promises.
- Compare exceptions with the error-as-value style (`std::expected`, C++23).

**Watch:** [Destructors in C++](https://www.youtube.com/watch?v=D8cWquReFqw), [Constructors in C++](https://www.youtube.com/watch?v=FXhALMsHwEY), [The new Keyword in C++](https://www.youtube.com/watch?v=NUZdUSqsCs4).

**Read:** *Tour of C++* Ch. 5 (Classes) + Ch. 4 (Error Handling — note RAII vs exceptions, and contrast with Java's checked exceptions).

**Optional (cppreference):** RAII — [`/w/cpp/language/raii`](https://en.cppreference.com/w/cpp/language/raii); destructors — [`/w/cpp/language/destructor`](https://en.cppreference.com/w/cpp/language/destructor); object lifetime — [`/w/cpp/language/lifetime`](https://en.cppreference.com/w/cpp/language/lifetime); exceptions — [`/w/cpp/language/exceptions`](https://en.cppreference.com/w/cpp/language/exceptions); `noexcept` specifier — [`/w/cpp/language/noexcept_spec`](https://en.cppreference.com/w/cpp/language/noexcept_spec); std::expected — [`/w/cpp/utility/expected`](https://en.cppreference.com/w/cpp/utility/expected).

**Do:**

1. Write a `ScopedFile` that opens a file in its constructor and closes it in its destructor. Prove (with prints) it closes even when an exception is thrown mid-scope.
2. **Exception safety:** write a `parseInt(std::string_view)` two ways — one that throws on bad input, one that returns `std::expected<int, ParseError>`. Write a caller for each and argue which fits a hot loop vs a one-shot validation.

---

## Module 3 — Copy semantics, the Rule of 0/3/5 & initialization

When a class owns a raw resource, the compiler-generated copy is wrong (shallow). You learn to control copying — and to sidestep C++'s notorious initialization traps.

**Learning outcomes**

- State the Rule of 3, Rule of 5, and Rule of 0, and pick the right one for a class.
- Implement a correct copy constructor and copy-assignment operator with self-assignment safety.
- Navigate copy/direct/list/aggregate initialization, narrowing, and the "most vexing parse."

**Watch:** [Copying and Copy Constructors in C++](https://www.youtube.com/watch?v=BvR1Pgzzr38), [How to Write a C++ Class](https://www.youtube.com/watch?v=3dHBFBw13E0), [The "this" keyword in C++](https://www.youtube.com/watch?v=Z_hPJ_EhceI).

**Read:** *Tour of C++* Ch. 6 (Essential Operations). *Effective Modern C++* **Item 17** (special-member generation) and **Item 7** (braced initialization).

**Optional (cppreference):** rule of three/five/zero — [`/w/cpp/language/rule_of_three`](https://en.cppreference.com/w/cpp/language/rule_of_three); copy constructor — [`/w/cpp/language/copy_constructor`](https://en.cppreference.com/w/cpp/language/copy_constructor); copy assignment — [`/w/cpp/language/copy_assignment`](https://en.cppreference.com/w/cpp/language/copy_assignment); list initialization — [`/w/cpp/language/list_initialization`](https://en.cppreference.com/w/cpp/language/list_initialization); aggregate initialization — [`/w/cpp/language/aggregate_initialization`](https://en.cppreference.com/w/cpp/language/aggregate_initialization).

**Do:**

1. Build a minimal `String` wrapping a `char*` (what Cherno builds): constructor, destructor, copy constructor, copy assignment. Run under `-fsanitize=address` to confirm no leaks or double-frees.
2. **Initialization:** trigger a narrowing conversion two ways — `int x = 3.9;` (allowed, silent) vs `int x{3.9};` (rejected) — and write a line that hits the most-vexing-parse (`Widget w();` declaring a function), then fix both with braces. State your one-line rule for which initializer to reach for by default.

---

## Module 4 — Move semantics & value categories (the hardest modern topic)

The big one with no C or Java equivalent. Budget extra time.

**Learning outcomes**

- Classify expressions as lvalues vs rvalues and explain what `std::move` actually does (it's a cast).
- Implement a move constructor and move-assignment operator and explain when each fires.
- Distinguish rvalue references from forwarding ("universal") references.

**Watch:** [lvalues and rvalues in C++](https://www.youtube.com/watch?v=fbYknr-HPYE), [Move Semantics in C++](https://www.youtube.com/watch?v=ehMg6zvXuMY), [std::move and the Move Assignment Operator in C++](https://www.youtube.com/watch?v=OWNeCTd7yQE).

**Read:** *Tour of C++* Ch. 6 (move sections). *Effective Modern C++* **Items 23, 24, 25**.

**Optional (cppreference):** move constructor — [`/w/cpp/language/move_constructor`](https://en.cppreference.com/w/cpp/language/move_constructor); move assignment — [`/w/cpp/language/move_assignment`](https://en.cppreference.com/w/cpp/language/move_assignment); std::move — [`/w/cpp/utility/move`](https://en.cppreference.com/w/cpp/utility/move); value categories — [`/w/cpp/language/value_category`](https://en.cppreference.com/w/cpp/language/value_category).

**Do:** Extend your `String` with a move constructor and move-assignment operator. Add prints to all five special members, then trigger each (construct, copy, move, assign, move-assign). Confirm a moved-from object is a valid empty state.

**You know this when:** you can predict which special member fires for `String b = a;` vs `String b = std::move(a);` vs `String b = makeString();`.

---

## Module 5 — Polymorphism the C++ way (NEW — the inheritance deltas that bite Java devs)

You know OOP, so this skips the concept and drills only what's different — and dangerous — in C++.

**Learning outcomes**

- Explain why methods are **non-virtual by default** (the opposite of Java) and what `virtual`, `override`, and `final` do.
- State why a polymorphic base class needs a **virtual destructor**, and connect it to deleting through `unique_ptr<Base>` (next module).
- Reproduce and explain **object slicing** — a hazard with no Java equivalent, because Java has no value semantics for objects.
- Sketch the vtable cost model and say when to prefer composition/value types over a hierarchy.
- Use the four named casts (`static_cast`, `dynamic_cast`, `const_cast`, `reinterpret_cast`) appropriately — and reach for `dynamic_cast` to downcast safely within a hierarchy, instead of C's single unchecked `(T)x`.

**Watch:** [Virtual Functions in C++](https://www.youtube.com/watch?v=oIV2KchSyGQ), [Interfaces in C++ (Pure Virtual Functions)](https://www.youtube.com/watch?v=UWAdd13EfM8), [Virtual Destructors in C++](https://www.youtube.com/watch?v=jELbKhGkEi0), [Visibility in C++](https://www.youtube.com/watch?v=6OVQ8nh3KP0), [Casting in C++](https://www.youtube.com/watch?v=pWZS1MtxI-A), [Dynamic Casting in C++](https://www.youtube.com/watch?v=CiHfz6pTolQ).

**Read:** *Tour of C++* Ch. 5 (the class-hierarchy and abstract-types sections). *Effective C++* (classic), the item *"Declare destructors virtual in polymorphic base classes."*

**Optional (cppreference):** virtual functions — [`/w/cpp/language/virtual`](https://en.cppreference.com/w/cpp/language/virtual); abstract class — [`/w/cpp/language/abstract_class`](https://en.cppreference.com/w/cpp/language/abstract_class); `override`/`final` — [`/w/cpp/language/override`](https://en.cppreference.com/w/cpp/language/override); derived classes (slicing) — [`/w/cpp/language/derived_class`](https://en.cppreference.com/w/cpp/language/derived_class); the four casts — [`/w/cpp/language/static_cast`](https://en.cppreference.com/w/cpp/language/static_cast), [`/w/cpp/language/dynamic_cast`](https://en.cppreference.com/w/cpp/language/dynamic_cast), [`/w/cpp/language/const_cast`](https://en.cppreference.com/w/cpp/language/const_cast), [`/w/cpp/language/reinterpret_cast`](https://en.cppreference.com/w/cpp/language/reinterpret_cast).

**Do:** Build a tiny `Shape` hierarchy (`Circle`, `Rectangle`) with a pure-virtual `area()`. Then:
1. Store them as `std::vector<Shape*>` — raw pointers for now (you'll retire these for smart pointers in Module 6) — and call `area()` polymorphically; mark overrides with `override`. Remember to `delete` each at the end.
2. **Slicing demo:** copy a `Circle` into a `Shape` *value* and show `area()` now returns the wrong answer; explain why in one sentence.
3. **Virtual-destructor demo:** give `Shape` a non-virtual destructor and a `Circle` that owns a resource, delete a `Circle` through a `Shape*`, watch the leak under the sanitizer, then make the destructor `virtual` and confirm it's fixed.
4. **Casting:** over your `vector<Shape*>`, use `dynamic_cast<Circle*>` to act only on the circles — it returns `nullptr` for anything that isn't one, so check it. Then try the same downcast with `static_cast` and explain in one sentence why it's unsafe here. Note in a comment what `const_cast` and `reinterpret_cast` are for, and why both are red flags in everyday code.

---

## Module 6 — Smart pointers (retire `new`/`delete`)

Apply RAII to heap ownership so you rarely write raw `new`/`delete` again. (The virtual destructor you just learned is exactly what makes `unique_ptr<Base>` safe.)

**Learning outcomes**

- Choose between `unique_ptr`, `shared_ptr`, and `weak_ptr` for a given ownership model.
- Explain the cost of `shared_ptr` (control block, atomic refcount) and how `weak_ptr` breaks cycles.

**Watch:** [Smart Pointers in C++](https://www.youtube.com/watch?v=UOB7-B2MfwA), [make_shared vs make_unique](https://www.youtube.com/results?search_query=The+Cherno+make_shared+vs+make_unique) (or [Track Memory Allocations](https://www.youtube.com/watch?v=sLlGEUO_EGE)).

**Read:** *Tour of C++* Ch. 15 (Pointers and Containers). *Effective Modern C++* **Items 18, 19, 20, 21**. learncpp.com: [move semantics and smart pointers](https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/).

**Optional (cppreference):** unique_ptr — [`/w/cpp/memory/unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr); shared_ptr — [`/w/cpp/memory/shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr); weak_ptr — [`/w/cpp/memory/weak_ptr`](https://en.cppreference.com/w/cpp/memory/weak_ptr).

**Do:** Implement a simplified `UniquePtr<T>`: a move-only class that deletes its pointee in the destructor. Then convert a raw-`new`/`delete` program (reuse the `Shape` hierarchy from Module 5) to `std::unique_ptr` and confirm zero leaks under the sanitizer.

---

## Module 7 — const correctness

`const` is pervasive in idiomatic C++ in a way it never was in your C or Java code. Getting it right early saves pain.

**Learning outcomes**

- Apply `const` to parameters, returns, member functions, and pointers/references correctly.
- Explain `const` pointer vs pointer-to-`const`, and what `mutable` is for.

**Watch:** [const in C++](https://www.youtube.com/watch?v=4fJBrditnJU), [Mutable Keyword in C++](https://www.youtube.com/watch?v=bP9z3H3cVMY).

**Read:** learncpp.com: [const](https://www.learncpp.com/cpp-tutorial/constant-variables-named-constants/) and [const class objects and member functions](https://www.learncpp.com/cpp-tutorial/const-class-objects-and-const-member-functions/).

**Optional (cppreference):** cv-qualifiers — [`/w/cpp/language/cv`](https://en.cppreference.com/w/cpp/language/cv).

**Do:** Add proper `const` member functions to your `String` (`size()`, `operator[] const`, …). Make a `const String` and verify the compiler stops mutation.

---

## Module 8 — Templates & generic programming (not Java generics)

C++ templates are compile-time code generation (monomorphization), not type erasure. They're "duck typed" at instantiation. C++20 adds **concepts** to constrain them.

**Learning outcomes**

- Write function and class templates and explain how they differ from Java generics.
- Read a basic template error and a `requires`/concept constraint.

**Watch:** [Templates in C++](https://www.youtube.com/watch?v=I-hZkUa9mIs), [auto in C++](https://www.youtube.com/watch?v=2vOPEuiGXVo).

**Read:** *Tour of C++* Ch. 7 (Templates) + Ch. 8 (Concepts and Generic Programming). *Effective Modern C++* **Items 1–4**. learncpp.com: [function templates](https://www.learncpp.com/cpp-tutorial/function-templates/), [class templates](https://www.learncpp.com/cpp-tutorial/class-templates/).

**Optional (cppreference):** templates — [`/w/cpp/language/templates`](https://en.cppreference.com/w/cpp/language/templates); constraints & concepts — [`/w/cpp/language/constraints`](https://en.cppreference.com/w/cpp/language/constraints).
**Do — STL rebuild #1:** Build `Vector<T>` — a growable array with `push_back`, `operator[]`, copy and move. Make it work for `int` and for your `String`. Add a C++20 `concept` constraining the element type to be movable.

**You know this when:** you can explain why a template error mentions a type you never named, and why `std::vector<int>` and `std::vector<std::string>` are genuinely different compiled types.

---

## Module 9 — The STL + rebuild your own containers

The payoff. Learn the algorithmic style (containers + iterators + `<algorithm>`) *and* reimplement containers to understand what's underneath.

**Learning outcomes**

- Pick the right container (`vector`, `map`, `unordered_map`, `set`, …) and justify it by complexity.
- Replace raw loops with standard algorithms (`sort`, `find_if`, `transform`, `accumulate`).
- Explain what an iterator is and the half-open `[begin, end)` convention — by having written one.

**Watch:** [Dynamic Arrays in C++ (std::vector)](https://www.youtube.com/watch?v=PocJ5jXv8No), [Optimizing the usage of std::vector](https://www.youtube.com/watch?v=HcESuwmlHEY), [Sorting in C++](https://www.youtube.com/watch?v=x0uUKWJzSO4).

**Read:** *Tour of C++* Ch. 12 (Containers) + Ch. 13 (Algorithms) + Ch. 14 (Ranges).

**Optional (cppreference):** containers library index — [`/w/cpp/container`](https://en.cppreference.com/w/cpp/container); iterators — [`/w/cpp/iterator`](https://en.cppreference.com/w/cpp/iterator); algorithms library index — [`/w/cpp/algorithm`](https://en.cppreference.com/w/cpp/algorithm).
**Do — STL rebuilds #2–4 (the core of this module):**
1. **`List<T>`** — doubly linked list. Write a real forward iterator (`begin()`/`end()`, `operator++`, `operator*`, `operator!=`) and prove it works in a range-based `for`. This is the iterator-protocol exercise.
2. **`Stack<T>`** — a container *adaptor* built on top of your own `Vector<T>` from Module 8. Tiny on purpose: it teaches that adaptors reuse an underlying container rather than reimplement storage.
3. **`HashMap<K,V>`** (chaining) — buckets as a `Vector` of `List`s, a hash functor, `insert`/`find`/`erase`, and a load-factor-triggered rehash. Module capstone and your bridge into the final project.

Then, the algorithm-style exercise: given a text file, count word frequencies with `std::unordered_map`, then produce the top 10 via `std::sort`/`std::partial_sort` on a vector of pairs — no manual index loops where an algorithm exists. Diff your `HashMap` against `std::unordered_map`.

---

## Module 10 — Modern C++ glue (the stuff that makes code pleasant)

**Learning outcomes**

- Use `auto`, range-based `for`, structured bindings, lambdas, and `std::optional`/`std::variant` idiomatically.
- Explain lambda capture by value vs by reference and the lifetime trap that comes with it.

**Watch:** [Lambdas in C++](https://www.youtube.com/watch?v=mWgmBBz0y8c), [Structured Bindings in C++](https://www.youtube.com/watch?v=eUsTO5BO3WI), [How to Deal with OPTIONAL Data (std::optional)](https://www.youtube.com/watch?v=UAAiwObNhQ0), [Multiple TYPES of Data in a SINGLE VARIABLE (std::variant)](https://www.youtube.com/watch?v=qCc_Vqg3hJk).

**Read:** *Tour of C++* Ch. 16 (Utilities). *Effective Modern C++* **Items 5–6** and **Items 30–31**.

**Optional (cppreference):** lambdas — [`/w/cpp/language/lambda`](https://en.cppreference.com/w/cpp/language/lambda); structured bindings — [`/w/cpp/language/structured_binding`](https://en.cppreference.com/w/cpp/language/structured_binding); auto — [`/w/cpp/language/auto`](https://en.cppreference.com/w/cpp/language/auto); std::optional — [`/w/cpp/utility/optional`](https://en.cppreference.com/w/cpp/utility/optional); std::variant — [`/w/cpp/utility/variant`](https://en.cppreference.com/w/cpp/utility/variant).

**Do:**

1. Take the Module 9 word-frequency data (or any `std::vector<int>`) and filter or reorder it with a lambda passed to a standard algorithm — e.g., `std::sort` with a custom comparator, or `std::count_if` with a predicate. Then, separately, write a function that builds a lambda capturing a local **by reference** and returns it; call the returned lambda after the function has exited, watch the sanitizer flag the dangling capture, and fix it by capturing **by value**.
2. **STL rebuild #5 (stretch):** Implement `Optional<T>` using aligned storage + placement `new` and an explicit `bool` engaged flag, with a destructor that only destroys the contained object when engaged. This is the manual-lifetime-management exercise.

---

## Module 11 — Build systems & dependency management (NEW — the practical gap)

Right now you've only run `g++ file.cpp`. Real C++ — and your capstone — needs a build tool and a way to pull in libraries. Coming from Maven/Gradle, the C++ answer is **CMake** plus **vcpkg** (or Conan).

**Learning outcomes**

- Write a `CMakeLists.txt` that builds a multi-file project and links a library.
- Pull in an external dependency through a package manager instead of vendoring source.
- Wire up a test target and run a linter/formatter over your code.

**Watch (context):** Cherno's [static libraries](https://www.youtube.com/watch?v=or1dAmUO8k0) and [dynamic libraries](https://www.youtube.com/watch?v=pLy69V2F_8M) videos (for what CMake automates under the hood).

**Read:** the official **CMake Tutorial** ([`cmake.org/cmake/help/latest/guide/tutorial`](https://cmake.org/cmake/help/latest/guide/tutorial)) and Henry Schreiner's **An Introduction to Modern CMake** ([`cliutils.gitlab.io/modern-cmake`](https://cliutils.gitlab.io/modern-cmake)). Skim **vcpkg** getting-started docs.

**Optional (reference):** **Catch2** or **doctest** docs (unit testing); **clang-format** and **clang-tidy** docs (formatting/linting).

**Do:**

1. Split one of your earlier multi-class exercises (e.g., the `Shape` hierarchy) into header/source files and build it with a `CMakeLists.txt` using `target_compile_features(... cxx_std_20)`.
2. Add an external library via vcpkg — `fmt` is a good first one — and call it from your project.
3. Add a **Catch2** test target with a handful of assertions over your rebuilt `Vector<T>`/`HashMap<K,V>`, then run `clang-tidy` with the `cppcoreguidelines-*` checks and fix one warning it raises.

---

## Module 12 — Capstone (1–2 weeks)

Pick one and build it end to end with CMake + a dependency, using smart pointers for ownership, RAII for resources, templates where they earn their keep, STL algorithms over hand loops — and where natural, your own rebuilt containers:

- A small **expression evaluator / calculator** (tokenizer → parser → evaluator) — `variant`, ownership trees, `std::expected` error handling.
- A **mini key–value store** with an in-memory index (reuse your `HashMap`) and file persistence — containers, RAII file handling, move semantics.
- A **tiny ECS or scene graph** (nods to Cherno's game-engine bent) — templates, ownership, polymorphism.

**Final learning outcomes**
- Manage all resources via RAII/smart pointers with zero sanitizer errors.
- Justify each design choice in value-semantics and ownership terms.
- Read and fix a template or move-related compiler error without panic.
- Build and test the project through CMake, with at least one external dependency.

---

## Module 13 — Concurrency (OPTIONAL — a natural next step given your OS background)

You already know threads and processes from OS basics; this is the C++ surface for them.

**Learning outcomes**

- Launch and join threads, protect shared state with a mutex (via RAII lock guards), and use `std::atomic`.
- Run work asynchronously with `std::async`/`std::future` and reason about the memory model at a high level.

**Watch:** [Threads in C++](https://www.youtube.com/watch?v=wXBcwHwIt_I), [Timing in C++](https://www.youtube.com/watch?v=oEx5vGNFrLk) (for measuring the speedup).

**Read:** *Tour of C++* Ch. 18 (Concurrency). *Effective Modern C++* **Items 35–40** (the concurrency chapter).

**Optional (cppreference):** thread — [`/w/cpp/thread/thread`](https://en.cppreference.com/w/cpp/thread/thread); mutex — [`/w/cpp/thread/mutex`](https://en.cppreference.com/w/cpp/thread/mutex); atomic — [`/w/cpp/atomic/atomic`](https://en.cppreference.com/w/cpp/atomic/atomic); async — [`/w/cpp/thread/async`](https://en.cppreference.com/w/cpp/thread/async); condition_variable — [`/w/cpp/thread/condition_variable`](https://en.cppreference.com/w/cpp/thread/condition_variable).

**Do:** Build a thread-safe queue (mutex + `condition_variable`) and drive it with a producer-and-consumer pair of threads. Then parallelize a large array sum with `std::async`, time it against the single-threaded version, and confirm correctness with `-fsanitize=thread`.

---

## How to study each module

1. Watch the videos first for intuition (Cherno is great for the "why").
2. Read the *Tour of C++* chapter for the precise, standard picture; hit the Effective C++ / Effective Modern C++ items where flagged.
3. Do the exercise **under `-fsanitize=address,undefined`** (and `thread` in Module 13) — coming from C/Java, your bugs will be lifetime and ownership bugs, and the sanitizer turns them from mysteries into instant feedback.
4. Reach for the flagged **cppreference** page only when you want the exact rule; scroll to its **References** section for the matching standard clause. Check a design choice against the **C++ Core Guidelines** when unsure what's idiomatic.

A note worth internalizing (the Cherno community repeats it): past a point, reading and writing *real* code beats more tutorials. Treat Modules 9–12 as the moment to start reading an open-source C++ codebase you find interesting.
