# C++ Delta Curriculum — Module Question Bank

> **For the AI agent quizzing me:** Ask one question at a time. For MCQs, show only the
> options (A–D), not the marked answer. For `[Code]` questions, show only the snippet and
> prompt. For `[Open]`, show only the prompt — never the "Key points." Wait for my answer,
> grade it, tell me right/wrong, and explain briefly. Let me pick a module or say "random."
> Track misses and re-ask them at the end. Don't reveal answers before I attempt.
>
> **Types:** `[MCQ]` single best answer · `[Open]` explain in my own words · `[Code]` predict
> the output or find the bug. **`(UB)`** tags an undefined-behavior question.

Questions map to each module's learning outcomes — a clean run on a module means I've got it.

---

## Module 0 — Orientation, toolchain & undefined behavior

**Q0.1 [Open]** What is a translation unit, and what does the One Definition Rule (ODR) require?
*Key points:* translation unit = one source file after preprocessing (includes expanded, macros applied); ODR = each entity has exactly one definition program-wide (inline functions/templates exempt). Violations are an error or UB.

**Q0.2 [MCQ]** Why do declarations usually go in headers while definitions go in `.cpp` files?
- A) Headers compile faster
- B) To avoid ODR violations when the header is included in multiple translation units ✅
- C) The compiler forbids code in `.cpp` files
- D) Headers can't contain functions

*Answer: B.* Caveat: `inline` functions and templates may be defined in headers because they're exempt.

**Q0.3 [Open] (UB)** Define undefined behavior, and contrast it with the safety net Java gave you.
*Key points:* UB = standard imposes no requirements; compiler may do anything (crash, wrong output, appear to work, optimize away). Java is memory-safe: bounds checks throw, no use-after-free, overflow wraps. C++ gives no net.

**Q0.4 [MCQ] (UB)** Which of these is undefined behavior?
- A) Unsigned integer overflow
- B) Signed integer overflow ✅
- C) Integer division by a non-zero value
- D) Comparing two pointers into the same array

*Answer: B.* Signed overflow is UB; unsigned wraps (modulo 2ⁿ).

**Q0.5 [Open] (UB)** Name two bug classes AddressSanitizer catches and two for UBSan.
*Key points:* ASan — use-after-free, heap/stack buffer overflow, leaks, use-after-return. UBSan — signed overflow, null deref, misaligned access, out-of-range shifts, invalid bool/enum.

**Q0.6 [MCQ] (UB)** A use-after-free is most reliably caught at runtime by:
- A) The type checker
- B) AddressSanitizer ✅
- C) clang-format
- D) The linker

*Answer: B.*

**Q0.7 [MCQ]** While learning, the recommended flags to catch memory/UB bugs early are:
- A) `-O3`
- B) `-fsanitize=address,undefined` ✅
- C) `-w`
- D) `-static`

*Answer: B.*

**Q0.8 [Open]** Name the high-level phases a source file passes through to become an executable.
*Key points:* preprocessing → compilation (each TU to object code) → (assembly) → linking (combine objects + libraries, resolve symbols). 3–4 steps acceptable.

**Q0.9 [MCQ] (UB)** Reading and using the value of an uninitialized local `int x;` is:
- A) Guaranteed to be 0
- B) Undefined behavior (indeterminate value) ✅
- C) A compile error
- D) Defined garbage

*Answer: B.*

**Q0.10 [Open]** Coming from C, why turn on `-Wall -Wextra`, and name one C idiom C++ rejects.
*Key points:* surfaces likely bugs (unused vars, sign-compare, missing returns); C++ rejects e.g. implicit `void*`→`T*` conversion (needs a cast), implicit-int, calling undeclared functions.

**Q0.11 [MCQ]** An `#include` directive is processed by:
- A) The linker
- B) The preprocessor ✅
- C) The runtime
- D) The optimizer

*Answer: B.*

**Q0.12 [MCQ] (UB)** Which is NOT undefined behavior?
- A) Dereferencing a null pointer
- B) Signed integer overflow
- C) `vector::operator[]` out of range
- D) `vector::at()` with an out-of-range index ✅

*Answer: D.* `.at()` throws `std::out_of_range`; `operator[]` out of range is UB.

---

## Module 1 — Value semantics, object model & views

**Q1.1 [MCQ]** Given `Widget a;`, what does `Widget b = a;` do?
- A) Makes `b` an alias to the same object (Java behavior)
- B) Copies `a` into an independent new object `b` ✅
- C) Moves `a`, emptying it
- D) Makes `b` a pointer to `a`

*Answer: B.*

**Q1.2 [Open]** Explain the differences between a reference (`T&`) and a pointer (`T*`).
*Key points:* reference = alias bound at init, not rebindable, not (legally) null, no arithmetic; pointer = address-holding object, nullable, reassignable, arithmetic, dereferenced.

**Q1.3 [MCQ]** When take a parameter by `const T&` instead of by value?
- A) Always
- B) When `T` is small/cheap to copy
- C) When `T` is expensive to copy and you only read it ✅
- D) When you'll modify the caller's object

*Answer: C.*

**Q1.4 [Open]** What ownership does `std::string_view` give, and what is the dangling-view trap?
*Key points:* none — non-owning (ptr + length) read-only borrow; trap: outliving the underlying buffer (e.g., view into a temporary) → dangling → UB.

**Q1.5 [MCQ]** Which is true of `std::span`?
- A) It owns and frees its elements
- B) Non-owning view over a contiguous sequence ✅
- C) Always heap-allocated
- D) Deep-copies the array

*Answer: B.*

**Q1.6 [Open]** Locals `a`, `b`, `c` declared in that order — destruction order and why?
*Key points:* `c`, `b`, `a` — reverse of construction, at scope exit.

**Q1.7 [Code] (UB)** What's the problem?
```cpp
std::string_view sv;
{
    std::string s = "hello";
    sv = s;
}
std::cout << sv;
```
*Key points:* `sv` views `s`'s buffer, destroyed at block end → dangling → use-after-free / UB.

**Q1.8 [MCQ]** A `Widget` value member vs a `Widget*` member:
- A) Both store the object inline
- B) Value member embeds the whole object; pointer stores only an address to it elsewhere ✅
- C) Pointers are always larger
- D) Value members are heap-allocated

*Answer: B.*

**Q1.9 [Open]** In Java, mutating through one object variable is visible through another assigned from it. Why not in C++ value types?
*Key points:* C++ copies the whole object (independent); Java copies a reference (same heap object). Java-like aliasing in C++ needs a reference/pointer.

**Q1.10 [MCQ]** Passing a `std::vector<int>` by value:
- A) Passes a reference automatically
- B) Copies the entire vector ✅
- C) Moves it
- D) Compile error

*Answer: B.*

**Q1.11 [Code]** Output?
```cpp
int a = 5;
int& r = a;
r = 10;
std::cout << a;
```
*Answer: 10.* `r` aliases `a`; assigning through it changes `a`.

**Q1.12 [MCQ]** What can a pointer do that a reference cannot?
- A) Alias an existing object
- B) Be reseated to a different object later ✅
- C) Be passed to a function
- D) Be read from

*Answer: B.*

**Q1.13 [Open]** Why is returning a large object by value not the performance disaster it looks like?
*Key points:* (N)RVO / copy elision and move semantics — the result is typically constructed directly in the caller, no real copy. Return by value is idiomatic.

**Q1.14 [MCQ] (UB)** A dangling reference/pointer arises when:
- A) You take the address of a heap object
- B) The referent's lifetime ends but the reference/pointer is still used ✅
- C) You copy an object
- D) You pass by const reference

*Answer: B.*

---

## Module 2 — RAII, destruction & exception safety

**Q2.1 [Open]** Define RAII and explain why it makes Java's `try/finally` largely unnecessary.
*Key points:* bind a resource to an automatic object — acquire in ctor, release in dtor; dtor runs deterministically at scope exit (incl. exception unwinding), so cleanup is automatic.

**Q2.2 [MCQ]** A `std::lock_guard<std::mutex>` releases its mutex:
- A) When you call `.unlock()`
- B) When its destructor runs at scope exit ✅
- C) Only if no exception was thrown
- D) At program termination

*Answer: B.*

**Q2.3 [Open]** State the basic, strong, and nothrow exception-safety guarantees.
*Key points:* basic — no leaks, invariants hold, some valid state; strong — fully succeeds or no effect (rollback); nothrow — never throws.

**Q2.4 [MCQ]** Marking a move constructor `noexcept` matters because:
- A) It inlines the function
- B) `std::vector` will move (not copy) elements during reallocation ✅
- C) It makes the move thread-safe
- D) It returns nothing

*Answer: B.*

**Q2.5 [Open]** When return `std::expected<T,E>` instead of throwing?
*Key points:* common/expected failures or hot paths (no unwinding cost), or to force handling via the type; reserve exceptions for rare exceptional conditions.

**Q2.6 [MCQ]** An exception escaping a destructor while another is propagating:
- A) Is caught by the nearest handler
- B) Calls `std::terminate` ✅
- C) Is ignored
- D) Retries

*Answer: B.* Destructors are implicitly `noexcept`.

**Q2.7 [MCQ]** Which standard type is a RAII wrapper for heap memory?
- A) `std::vector` only
- B) `std::unique_ptr` ✅
- C) raw `new`
- D) `std::move`

*Answer: B.*

**Q2.8 [Open]** Describe the copy-and-swap idiom and which guarantee it gives assignment.
*Key points:* copy the source, swap (cheap, noexcept) internals with `*this`, let old data die with the copy; gives the strong guarantee and handles self-assignment.

**Q2.9 [Code]** What's wrong, and how do you fix it?
```cpp
std::mutex m;
void f() {
    m.lock();
    doWork();      // may throw
    m.unlock();
}
```
*Key points:* if `doWork()` throws, `unlock()` is skipped → mutex stuck locked. Fix: `std::lock_guard<std::mutex> g(m);`.

**Q2.10 [MCQ]** RAII releases resources:
- A) In acquisition order
- B) In reverse order of acquisition ✅
- C) Randomly
- D) Only at exit

*Answer: B.*

**Q2.11 [Open]** Why are throwing destructors dangerous, and what's the default?
*Key points:* a second in-flight exception during unwinding → `std::terminate`; destructors are implicitly `noexcept` since C++11. Keep them non-throwing.

**Q2.12 [MCQ]** `std::expected<T, E>` holds:
- A) Always a `T`
- B) Either a `T` (success) or an `E` (error), checkable without exceptions ✅
- C) A pointer to `T`
- D) A future

*Answer: B.*

**Q2.13 [Open]** Give a non-memory resource RAII handles well and sketch the class.
*Key points:* file/socket/lock/DB connection — acquire in ctor, release in dtor. e.g., `ScopedFile { FILE* f; ctor fopen; dtor fclose; }`.

---

## Module 3 — Copy, Rule of 0/3/5 & initialization

**Q3.1 [MCQ]** The Rule of Three's three members:
- A) Constructor, destructor, `operator==`
- B) Destructor, copy constructor, copy-assignment operator ✅
- C) Default ctor, copy ctor, destructor
- D) Move ctor, move assignment, destructor

*Answer: B.*

**Q3.2 [Open]** What is the Rule of Zero and how do you achieve it?
*Key points:* design so the class needs none of the special members — let members (smart pointers, containers) own resources, making the generated ones correct. Prefer this.

**Q3.3 [Open]** Why does a class owning a raw `char*` need a user-defined copy constructor?
*Key points:* implicit copy is shallow (copies the pointer) → two objects share one buffer → double-free and aliasing. Must deep-copy.

**Q3.4 [MCQ]** What does `int x{3.9};` do?
- A) `x` = 3
- B) `x` = 4
- C) Ill-formed — brace-init forbids the narrowing conversion ✅
- D) UB

*Answer: C.* (`int x = 3.9;` would silently truncate to 3.)

**Q3.5 [MCQ]** At block scope, `Widget w();` declares:
- A) A default-constructed `Widget`
- B) A function `w` returning `Widget` (most vexing parse) ✅
- C) A syntax error
- D) A pointer

*Answer: B.* Fix: `Widget w;` or `Widget w{};`.

**Q3.6 [Open]** How do you make copy-assignment self-assignment-safe, and why?
*Key points:* `if (this == &other) return *this;`, or copy-and-swap. Otherwise releasing your own resource before copying from `other` (== `this`) corrupts the object.

**Q3.7 [MCQ]** The Rule of Five adds which two members to the Rule of Three?
- A) Default ctor and destructor
- B) Move constructor and move-assignment operator ✅
- C) `operator==` and `operator!=`
- D) Two more copy ctors

*Answer: B.*

**Q3.8 [Code]** How many times does the copy constructor run?
```cpp
struct S { S(){} S(const S&){ std::cout<<"copy\n"; } };
void take(S s) {}
S a;
take(a);
```
*Answer: once* — passing `a` by value copies it. (`S a;` uses the default ctor.)

**Q3.9 [MCQ]** If you declare a destructor but no copy operations, the implicit copy operations are:
- A) Deleted
- B) Still generated, but relying on them is deprecated ✅
- C) Never generated
- D) Made private

*Answer: B.* (Move operations, however, are NOT generated — exactly why the Rule of Three/Five matters.)

**Q3.10 [Open]** Difference between direct-init `T a(x);` and copy-init `T a = x;`?
*Key points:* direct considers all constructors incl. `explicit`; copy-init won't use `explicit` ctors. Often equivalent, but `explicit` is the dividing line.

**Q3.11 [MCQ]** `std::vector<int> v{3};` vs `std::vector<int> v(3);`:
- A) Identical
- B) `{3}` → one element (value 3); `(3)` → three zero-initialized ints ✅
- C) Both → three elements
- D) Both → one element

*Answer: B.* Braces prefer the `initializer_list` constructor.

**Q3.12 [Open]** What is `= default` on a special member and when is it useful?
*Key points:* request the compiler-generated version explicitly — documents intent, keeps it trivial/noexcept, and can reinstate a member another declaration suppressed (e.g., a user destructor suppresses moves; `= default` brings them back).

**Q3.13 [MCQ]** To explicitly forbid copying:
- A) Private copy ctor with no body (old style)
- B) `Widget(const Widget&) = delete;` ✅
- C) `Widget(const Widget&) = default;`
- D) Nothing — classes are non-copyable by default

*Answer: B.*

---

## Module 4 — Move semantics & value categories

**Q4.1 [MCQ]** What does `std::move(x)` actually do?
- A) Moves `x`'s resources on the spot
- B) Casts `x` to an rvalue reference, enabling a move overload ✅
- C) Destroys `x`
- D) Copies `x`

*Answer: B.* By itself it moves nothing.

**Q4.2 [Open]** Classify: `x` (named var), `x + 1`, `std::move(x)`, `foo()` returning by value.
*Key points:* lvalue; rvalue (prvalue); rvalue (xvalue); rvalue (prvalue).

**Q4.3 [MCQ]** After `String b = std::move(a);`, `a` is:
- A) Destroyed and unusable
- B) Valid but unspecified (typically empty) ✅
- C) Unchanged
- D) Dangling

*Answer: B.*

**Q4.4 [Open]** Which special member fires for `String b = a;`, `= std::move(a);`, `= makeString();`?
*Key points:* copy ctor; move ctor; for `makeString()` move is conceptually selected but elided by guaranteed copy elision (C++17) — usually nothing.

**Q4.5 [MCQ]** In `template<class T> void f(T&& x)`, `T&&` is:
- A) Always an rvalue reference
- B) A forwarding reference (binds lvalues or rvalues) ✅
- C) A const reference
- D) A pointer

*Answer: B.* Only in deduced context.

**Q4.6 [Code]** Output?
```cpp
struct S {
  S() = default;
  S(const S&){ std::cout<<"copy "; }
  S(S&&){ std::cout<<"move "; }
};
S a;
S b = a;
S c = std::move(a);
```
*Answer: `copy move `* — `S a;` default (silent); `b=a` copies (lvalue); `c=std::move(a)` moves.

**Q4.7 [MCQ]** After moving from a `std::unique_ptr`, the source is:
- A) Dangling
- B) Null ✅
- C) Pointing to freed memory
- D) Unchanged

*Answer: B.*

**Q4.8 [Open]** Why does a `noexcept` move constructor sometimes speed up `std::vector`?
*Key points:* reallocation uses move-if-`noexcept`; without it vector copies (to keep the strong guarantee). `noexcept` moves avoid copies on growth.

**Q4.9 [MCQ]** `std::forward<T>(x)` is used to:
- A) Always cast to rvalue
- B) Preserve the value category of a forwarding reference when passing it on ✅
- C) Move `x`
- D) Copy `x`

*Answer: B.*

**Q4.10 [Code]** What prints?
```cpp
void f(int&)  { std::cout<<"L "; }
void f(int&&) { std::cout<<"R "; }
int x = 0;
f(x);
f(5);
f(std::move(x));
```
*Answer: `L R R `* — lvalue; rvalue; rvalue.

**Q4.11 [MCQ]** Inside `void g(int&& x)`, the named parameter `x` is:
- A) An rvalue
- B) An lvalue ✅
- C) A prvalue
- D) Undefined

*Answer: B.* A named rvalue reference is an lvalue — hence you `std::move(x)` again to forward it as an rvalue.

**Q4.12 [Open]** Why should you NOT write `return std::move(localVar);`?
*Key points:* it turns the return into an xvalue and disables (N)RVO/copy elision; the implicit move of a local in `return` already happens. So it can pessimize — return the local directly.

**Q4.13 [MCQ]** A moved-from `std::string` is guaranteed to be:
- A) Empty
- B) Valid but unspecified ✅
- C) Equal to its prior value
- D) Null

*Answer: B.*

---

## Module 5 — Polymorphism the C++ way

**Q5.1 [MCQ]** A member function dispatches virtually when:
- A) Always, as in Java
- B) Only if declared `virtual` (directly or inherited) ✅
- C) Only if the class has no constructor
- D) Never

*Answer: B.* Non-virtual by default — opposite of Java.

**Q5.2 [Open] (UB)** Why must a polymorphic base have a virtual destructor, and what happens without one?
*Key points:* deleting a derived object through `Base*` with non-virtual `~Base` is UB — only `~Base` runs, leaking the derived part. Virtual destructor runs the full chain.

**Q5.3 [MCQ]** Object slicing occurs when:
- A) You delete a derived object twice
- B) You copy a derived object into a base-class *value*, dropping the derived part ✅
- C) You omit a virtual destructor
- D) You call `std::move`

*Answer: B.*

**Q5.4 [Open]** What do `override` and `final` do, and why write `override`?
*Key points:* `override` asserts it really overrides — compiler errors on signature mismatch (catches typos creating a new function); `final` blocks further overriding/derivation.

**Q5.5 [MCQ]** To call `area()` polymorphically over mixed shapes, use:
- A) `std::vector<Shape>`
- B) `std::vector<std::unique_ptr<Shape>>` ✅
- C) `std::array<Shape, N>`
- D) `Shape[]`

*Answer: B.* By-value storage slices.

**Q5.6 [Code] (UB)** What's the bug?
```cpp
struct Base { ~Base(){ std::cout<<"~Base "; } };
struct Derived : Base {
  int* data = new int[100];
  ~Derived(){ delete[] data; }
};
Base* p = new Derived;
delete p;
```
*Key points:* `~Base` non-virtual → `delete p` runs only `~Base`, `~Derived` never runs → `data` leaks (UB). Fix: `virtual ~Base()`.

**Q5.7 [MCQ]** A pure virtual function is declared:
- A) `virtual void f();`
- B) `virtual void f() = 0;` ✅
- C) `void f() final;`
- D) `static void f();`

*Answer: B.* Makes the class abstract.

**Q5.8 [Open]** What is a vtable, and what's the cost of a virtual vs normal call?
*Key points:* per-class table of function pointers; each polymorphic object holds a vptr to it; a virtual call adds an indirection and generally isn't inlined — small but nonzero overhead.

**Q5.9 [MCQ]** Calling a virtual function inside a base-class constructor dispatches to:
- A) The derived override
- B) The base version (object isn't a Derived yet) ✅
- C) UB
- D) Compile error

*Answer: B.* During Base construction the dynamic type is Base — opposite of Java.

**Q5.10 [Open]** When prefer composition over inheritance?
*Key points:* "has-a"/reuse without is-a, avoiding fragile base classes, slicing, tight coupling; modern C++ favors value types + composition, reserving inheritance for genuine runtime polymorphism behind a stable interface.

**Q5.11 [MCQ]** `final` on a class means:
- A) Can't be copied
- B) Can't be used as a base class ✅
- C) Immutable
- D) All members const

*Answer: B.*

**Q5.12 [Code]** Output?
```cpp
struct A { virtual void who(){ std::cout<<"A"; } };
struct B : A { void who() override { std::cout<<"B"; } };
A* p = new B;
p->who();
A obj = B{};
obj.who();
```
*Answer: `BA`* — `p->who()` dispatches to B; `obj` slices to A so `obj.who()` prints A.

**Q5.13 [MCQ]** To keep polymorphic behavior across derived types in a collection, store:
- A) Values in a vector
- B) (Smart) pointers to the base ✅
- C) Copies cast to base
- D) References (can't be stored in a vector)

*Answer: B.*

**Q5.14 [MCQ]** To safely downcast a `Base*` to a `Derived*` in a polymorphic hierarchy, use:
- A) `static_cast`
- B) `dynamic_cast` ✅
- C) `const_cast`
- D) `reinterpret_cast`

*Answer: B.* It checks at runtime (via RTTI), returning `nullptr` (pointers) or throwing `std::bad_cast` (references) on a mismatch.

**Q5.15 [Code]** What does this print?
```cpp
struct Base { virtual ~Base() = default; };
struct A : Base {};
struct B : Base {};
Base* p = new A;
std::cout << (dynamic_cast<B*>(p) ? "B" : "null") << " ";
std::cout << (dynamic_cast<A*>(p) ? "A" : "null");
```
*Answer: `null A`* — `p` really points to an `A`, so the cast to `B*` fails (→ `nullptr`) and the cast to `A*` succeeds. (Works because the type is polymorphic — note the virtual destructor.)

**Q5.16 [Open]** Name C++'s four named casts and when each is appropriate, and say why they're preferred over a C-style `(T)x`.
*Key points:* `static_cast` — well-defined compile-time conversions (numeric, up-cast, `void*`↔`T*`); `dynamic_cast` — runtime-checked downcast in polymorphic hierarchies; `const_cast` — add/remove `const` (rare, a smell); `reinterpret_cast` — bit-level reinterpretation (dangerous, low-level). Preferred because each states intent, is greppable, and is narrower — a C-style cast silently does whatever conversion works, including unsafe ones, with no checking.

**Q5.17 [MCQ] (UB)** Using `static_cast` to downcast a `Base*` that does **not** actually point to a `Derived`, then using the result, is:
- A) Safe — `static_cast` checks the type
- B) Undefined behavior — `static_cast` does no runtime check ✅
- C) A compile error
- D) Returns `nullptr`

*Answer: B.* That runtime check is exactly what `dynamic_cast` adds.

---

## Module 6 — Smart pointers

**Q6.1 [MCQ]** `std::unique_ptr` is:
- A) Copyable and movable
- B) Move-only ✅
- C) Reference-counted
- D) Required to be on the heap

*Answer: B.*

**Q6.2 [Open]** Cost of `std::shared_ptr` vs `unique_ptr`, and when do you actually need it?
*Key points:* control block with an **atomic** refcount (synchronized on copy/destroy) + larger size; use only for genuinely shared ownership with no single bounding scope.

**Q6.3 [MCQ] (UB-adjacent)** Two `shared_ptr`s in a cycle:
- A) Cleaned up automatically
- B) Leak — neither refcount hits zero ✅
- C) Fail to compile
- D) Flagged at compile time

*Answer: B.* Break with `weak_ptr`.

**Q6.4 [Open]** Why prefer `make_unique` over `unique_ptr<T>(new T(...))`?
*Key points:* avoids a potential leak if another arg's evaluation throws, removes raw `new`, concise; `make_shared` also fuses object + control-block allocation.

**Q6.5 [MCQ]** `std::weak_ptr` exists mainly to:
- A) Speed up `shared_ptr`
- B) Break reference cycles ✅
- C) Replace `unique_ptr`
- D) Make a pointer thread-safe

*Answer: B.*

**Q6.6 [Code] (UB)** What's wrong?
```cpp
int* raw = new int(42);
std::shared_ptr<int> a(raw);
std::shared_ptr<int> b(raw);
```
*Key points:* two independent control blocks own the same pointer → double-free when both die (UB). Fix: `b = a;` or use `make_shared` and never build two shared_ptrs from one raw pointer.

**Q6.7 [MCQ]** `make_shared<T>()` vs `shared_ptr<T>(new T())` — make_shared typically:
- A) Is slower
- B) Does a single combined allocation for object + control block ✅
- C) Doesn't refcount
- D) Returns a unique_ptr

*Answer: B.*

**Q6.8 [Open]** How do you transfer ownership out of a `unique_ptr`?
*Key points:* `std::move` it into another `unique_ptr`; or `.release()` to take the raw pointer and give up ownership (then delete manually); `.reset()` replaces/destroys.

**Q6.9 [MCQ]** To safely get a `shared_ptr` from a `weak_ptr`:
- A) Cast it
- B) Call `.lock()` (returns a possibly-empty shared_ptr) ✅
- C) Dereference directly
- D) Call `.get()`

*Answer: B.*

**Q6.10 [Code]** Output?
```cpp
auto sp = std::make_shared<int>(7);
std::weak_ptr<int> wp = sp;
std::cout << sp.use_count() << " ";
sp.reset();
std::cout << (wp.expired() ? "expired" : "alive");
```
*Answer: `1 expired`* — weak_ptr doesn't raise the strong count; after reset the object is gone.

**Q6.11 [MCQ]** A custom deleter on a `unique_ptr` is useful for:
- A) Reference counting
- B) Releasing a non-`new` resource (e.g., `fclose` on a `FILE*`) ✅
- C) Making it copyable
- D) Thread safety

*Answer: B.*

**Q6.12 [Open]** When is a raw pointer still appropriate in modern C++?
*Key points:* non-owning observation/parameters where null is meaningful — ownership stays with a smart pointer/container; raw = "observe, don't own." Prefer a reference when non-null.

---

## Module 7 — const correctness

**Q7.1 [MCQ]** Difference between `const int* p` and `int* const p`?
- A) None
- B) Pointer-to-const-int (can't change `*p`) vs const-pointer-to-int (can't change `p`) ✅
- C) First is a const pointer; second a pointer to const
- D) Both forbid changing `p`

*Answer: B.*

**Q7.2 [Open]** What does a `const` member function guarantee and what may it touch?
*Key points:* won't modify observable state; callable on const objects; `this` is pointer-to-const; can't modify non-`mutable` members or call non-const members.

**Q7.3 [MCQ]** `mutable` lets you:
- A) Reassign a const object
- B) Modify a specific member inside a const member function ✅
- C) Strip const off a pointer
- D) Make access thread-safe

*Answer: B.*

**Q7.4 [Open]** Why provide both `T& operator[](size_t)` and `const T& operator[](size_t) const`?
*Key points:* const overload lets a const object be read; non-const lets a mutable object be written. Without the const version you can't index a const container.

**Q7.5 [Code]** Which line fails to compile?
```cpp
void f(const std::string& s) {
    s += "x";        // (1)
    std::cout << s;  // (2)
}
```
*Answer: (1)* — can't modify through a `const` reference; (2) (read) is fine.

**Q7.6 [MCQ]** const correctness mainly helps by:
- A) Running faster
- B) Letting the compiler enforce that read-only data isn't mutated — catching bugs at compile time ✅
- C) Reducing memory
- D) Auto-multithreading

*Answer: B.*

**Q7.7 [Open]** Why can't you call a non-const member function on a `const` object?
*Key points:* a const object's `this` is pointer-to-const; a non-const member might mutate state, violating const — only const members are callable.

**Q7.8 [MCQ]** `int const x = 5;` vs `const int x = 5;`:
- A) Different
- B) Identical ✅
- C) First is an error
- D) Second is an error

*Answer: B.*

**Q7.9 [Open]** A `const` member function makes a pointer member const — but const in what sense? (the subtlety)
*Key points:* the *pointer* becomes const (can't reseat), but NOT the pointee — you can still modify `*ptr` from a const method. (Hence care with "deep const" / `propagate_const`.)

**Q7.10 [MCQ]** Returning `const T` *by value* (e.g., `const int foo()`):
- A) Is good practice
- B) Is mostly pointless and can inhibit moves ✅
- C) Is required for const correctness
- D) Prevents copies

*Answer: B.*

**Q7.11 [Open]** How does `const&` let one parameter accept both lvalues and temporaries?
*Key points:* a const lvalue reference can bind to an rvalue/temporary (extending its lifetime to the reference's scope), so a single `const T&` param works for named objects and temporaries without extra overloads.

---

## Module 8 — Templates & generic programming

**Q8.1 [Open]** How do C++ templates differ from Java generics at the implementation level?
*Key points:* C++ monomorphizes — a distinct concrete type/function per instantiation, no type erasure, "duck typed" on used operations; Java erases to `Object`+casts (one runtime version). C++: zero runtime cost, but code bloat + cryptic errors.

**Q8.2 [MCQ]** `std::vector<int>` and `std::vector<std::string>` are:
- A) One type with a runtime tag
- B) Two distinct, separately compiled types ✅
- C) Subclasses of a vector base
- D) Identical after erasure

*Answer: B.*

**Q8.3 [MCQ]** A C++20 `concept` is for:
- A) Faster runtime dispatch
- B) Constraining template parameters and clearer errors ✅
- C) Replacing inheritance
- D) Allocating memory

*Answer: B.*

**Q8.4 [Open]** Why can a template error mention a type you never wrote?
*Key points:* the error surfaces at instantiation deep in library code, with the concrete type the compiler substituted — far from your call site.

**Q8.5 [Code]** Does this compile, and for which call?
```cpp
template<class T> T max3(T a, T b, T c){ return std::max({a,b,c}); }
max3(1, 2, 3);
max3(1, 2.0, 3);   // (1)
```
*Key points:* first call is fine (all int). (1) fails — `T` deduced inconsistently (int/double/int), no match. Fix: `max3<double>(...)` or unify the argument types.

**Q8.6 [MCQ]** Type-dependent template code is fully type-checked:
- A) When the template is defined
- B) When instantiated with concrete types ✅
- C) At runtime
- D) Never

*Answer: B.*

**Q8.7 [Open]** What is template specialization, with a use case?
*Key points:* a different implementation for specific arguments (full/partial); e.g., a generic `Serializer<T>` with a special `bool`/`std::string` case, or pointer optimizations.

**Q8.8 [MCQ]** A non-type template parameter is:
- A) A type like `T`
- B) A compile-time constant value, e.g. `template<std::size_t N>` ✅
- C) A runtime argument
- D) A concept

*Answer: B.* (e.g., `std::array<T, N>`.)

**Q8.9 [Code]** Output?
```cpp
template<class T> void f(T)  { std::cout<<"value "; }
template<class T> void f(T*) { std::cout<<"ptr "; }
int x = 0;
f(x);
f(&x);
```
*Answer: `value ptr `* — overload resolution prefers the pointer overload for `&x`.

**Q8.10 [Open]** Why are most templates defined entirely in headers?
*Key points:* the full definition must be visible at each instantiation point (each TU); no separate compilation by default — a `.cpp`-only definition causes unresolved symbols unless explicitly instantiated.

**Q8.11 [MCQ]** `auto x = foo();` deduces the type using rules most like:
- A) Manual casting
- B) Template type deduction ✅
- C) Runtime reflection
- D) Always `decltype`

*Answer: B.* (Drops top-level const/ref unless `&`/`&&` written.)

**Q8.12 [Open]** What do C++20 concepts give you over raw SFINAE/`enable_if`?
*Key points:* readable constraints, far better errors ("constraint not satisfied" vs substitution-failure walls), constrained overloading/ordering, self-documenting interfaces.

---

## Module 9 — STL + rebuilt containers

**Q9.1 [MCQ]** Fast lookup by key, ordering not needed — best container:
- A) `std::vector`
- B) `std::map`
- C) `std::unordered_map` ✅
- D) `std::list`

*Answer: C.*

**Q9.2 [Open]** Explain `[begin, end)` and why `end()` is one-past-the-last.
*Key points:* `begin`=first, `end`=one-past-last; empty when `begin==end`; gives `size = end-begin`, clean loops, and a valid sentinel without a magic value.

**Q9.3 [MCQ]** Minimum to use a custom type in a range-based `for`:
- A) `operator[]` and `size()`
- B) `begin()`/`end()` returning iterators with `operator*`, `operator++`, `operator!=` ✅
- C) An `iterator` typedef only
- D) A virtual `next()`

*Answer: B.*

**Q9.4 [Open] (UB)** Name an operation that invalidates `vector` iterators, and the consequence of using them after.
*Key points:* a reallocating `push_back`/`insert` invalidates all iterators/pointers/references (`erase` invalidates from the point onward); using an invalidated one is UB.

**Q9.5 [MCQ]** To find the first element matching a predicate:
- A) `std::sort`
- B) `std::find_if` ✅
- C) `std::accumulate`
- D) `std::transform`

*Answer: B.*

**Q9.6 [MCQ]** Average lookup complexity, `std::map` vs `std::unordered_map`:
- A) O(1) vs O(log n)
- B) O(log n) vs O(1) average ✅
- C) Both O(n)
- D) Both O(1)

*Answer: B.*

**Q9.7 [Code] (UB)** What's the bug?
```cpp
std::vector<int> v{1,2,3};
for (auto it = v.begin(); it != v.end(); ++it)
    if (*it == 2) v.push_back(99);
```
*Key points:* `push_back` may reallocate, invalidating `it`/`end` mid-loop → UB. Fix: reserve up front, use indices, or collect then append.

**Q9.8 [MCQ]** To remove all elements equal to `x` from a `vector` (pre-C++20):
- A) A manual loop erasing each
- B) Erase–remove: `v.erase(std::remove(v.begin(),v.end(),x), v.end())` ✅
- C) `v.clear()`
- D) `std::find`

*Answer: B.* (C++20: `std::erase(v, x)`.)

**Q9.9 [Open]** What does `std::accumulate(begin, end, init)` do, and a subtle pitfall?
*Key points:* folds the range from `init` (sum by default); pitfall — the accumulator type is `init`'s type, so `accumulate(d.begin(), d.end(), 0)` over doubles truncates (use `0.0`).

**Q9.10 [MCQ]** `std::sort` requires iterators that are:
- A) Forward
- B) Random-access ✅
- C) Input
- D) Output

*Answer: B.* (Hence `std::list` has its own `.sort()`.)

**Q9.11 [Code]** Output?
```cpp
std::vector<int> v{5,3,1,4,2};
std::sort(v.begin(), v.end());
std::cout << v[0] << v[4];
```
*Answer: `15`* — sorted ascending → v[0]=1, v[4]=5.

**Q9.12 [Open]** Why is `std::vector` usually preferred over `std::list` even with "lots of inserts"?
*Key points:* contiguous memory → cache-friendly iteration; list's O(1) insert is undercut by pointer-chasing cache misses and per-node allocation; vector wins unless you need stable references or frequent middle splices in huge lists.

**Q9.13 [MCQ]** `std::map` keeps elements:
- A) In insertion order
- B) Sorted by key ✅
- C) In hash order
- D) Unordered

*Answer: B.*

---

## Module 10 — Modern C++ glue

**Q10.1 [MCQ] (UB)** A lambda captures a local by reference and is stored past that local's life. Invoking it later is:
- A) A harmless copy
- B) A dangling reference → UB ✅
- C) A compile error
- D) Reference counting

*Answer: B.*

**Q10.2 [Open]** When does `[=]` vs `[&]` capture matter for *correctness*?
*Key points:* by-reference dangles if the lambda outlives the captured object; by-value snapshots (safe to outlive, frozen value + copy cost); `[this]` captures a pointer that dangles if the object dies.

**Q10.3 [MCQ]** `std::optional<T>` models:
- A) A possibly-null pointer
- B) A value that may or may not be present, no heap allocation ✅
- C) A thread-safe wrapper
- D) A two-type union

*Answer: B.*

**Q10.4 [Open]** What does `auto [x, y] = somePair;` do, and one gotcha?
*Key points:* structured binding decomposing a pair/tuple/struct; gotcha — `auto` copies while `auto&` binds references (watch dangling/unintended copies).

**Q10.5 [Code] (UB)** Spot the danger:
```cpp
auto makeCounter() {
    int count = 0;
    return [&]() { return ++count; };
}
auto c = makeCounter();
c();
```
*Key points:* captures `count` by reference, but `count` dies when `makeCounter` returns → dangling, UB on call. Fix: `[count]() mutable`.

**Q10.6 [MCQ]** `[x]` vs `[&x]` in a lambda:
- A) Both by reference
- B) `[x]` copies; `[&x]` references ✅
- C) Both copy
- D) `[x]` is illegal

*Answer: B.*

**Q10.7 [Open]** What does `mutable` on a lambda do?
*Key points:* a lambda's `operator()` is const by default, so by-value captures can't be modified; `mutable` removes the const, letting it modify its own copies (originals untouched).

**Q10.8 [MCQ]** `std::variant<int, std::string>` is:
- A) A pointer to either
- B) A type-safe union holding exactly one of the listed types ✅
- C) A tuple of both
- D) An optional

*Answer: B.*

**Q10.9 [Code]** Output?
```cpp
std::optional<int> o;
std::cout << o.value_or(-1) << " ";
o = 42;
std::cout << o.value_or(-1);
```
*Answer: `-1 42`* — empty returns the fallback; then holds 42.

**Q10.10 [MCQ]** `for (auto x : c)` vs `for (auto& x : c)`:
- A) Identical
- B) First copies each element; second references them (use `&` to modify or avoid copies) ✅
- C) First always faster
- D) Second copies

*Answer: B.*

**Q10.11 [Open]** What problem does `std::visit` solve for a `variant`?
*Key points:* dispatches on the currently-held alternative, calling the matching visitor overload — exhaustive, type-safe handling without manual `index()`/`get_if` chains.

**Q10.12 [MCQ]** `for (auto& [k, v] : someMap)` gives you:
- A) Copies of key and value
- B) References to each pair's key and mapped value (structured binding) ✅
- C) Iterators
- D) A compile error

*Answer: B.*

---

## Module 11 — Build systems & dependency management

**Q11.1 [MCQ]** Modern CMake way to require C++20 for a target:
- A) Hand-edit the Makefile
- B) `target_compile_features(mytarget PRIVATE cxx_std_20)` ✅
- C) Pass the flag only on the command line
- D) Rename files to `.cpp20`

*Answer: B.*

**Q11.2 [Open]** What do vcpkg/Conan solve that CMake alone does not?
*Key points:* dependency acquisition — fetch/build/version third-party libs (the Maven/Gradle role) instead of vendoring source; CMake builds, the package manager supplies.

**Q11.3 [MCQ]** clang-tidy is:
- A) A compiler
- B) A static-analysis/linter flagging bug-prone or non-idiomatic code ✅
- C) A formatter
- D) A debugger

*Answer: B.* (clang-format is the formatter.)

**Q11.4 [Open]** Why a Catch2/doctest test target instead of testing in `main()`?
*Key points:* isolated, repeatable, automatable assertions; localized failures; CI-friendly; components tested independently without polluting the app.

**Q11.5 [MCQ]** `target_link_libraries(app PRIVATE fmt::fmt)` does what?
- A) Compiles fmt from scratch
- B) Links `app` against fmt and propagates its usage requirements ✅
- C) Downloads fmt
- D) Formats the code

*Answer: B.*

**Q11.6 [Open]** Difference between a static and a dynamic/shared library?
*Key points:* static (.a/.lib) copied into the executable at link time (bigger binary, no runtime dep); dynamic (.so/.dll/.dylib) loaded at runtime (smaller, shared, must be present/locatable).

**Q11.7 [MCQ]** clang-format is for:
- A) Finding bugs
- B) Enforcing consistent code style/layout ✅
- C) Linking
- D) Running tests

*Answer: B.*

**Q11.8 [Open]** Why prefer target-based ("modern") CMake over directory-wide commands?
*Key points:* usage requirements scoped per target with PUBLIC/PRIVATE/INTERFACE and propagated to consumers — avoids global leakage, makes dependencies composable.

**Q11.9 [MCQ]** A CI pipeline for a C++ project typically:
- A) Only formats code
- B) Configures, builds, and runs tests on each push ✅
- C) Replaces CMake
- D) Only runs clang-tidy

*Answer: B.*

**Q11.10 [Open]** What does a package manager give you that copying a library's headers into your repo does not?
*Key points:* versioned, reproducible resolution; correct build for your toolchain/platform; transitive deps; easy upgrades — vs stale hand-maintained vendored copies.

---

## Module 12 — Capstone (reflective)

**Q12.1 [Open]** List every resource in your capstone (memory, files, sockets…) and which RAII type/smart pointer owns each.
*Key points:* one clear owner per resource; no raw owning pointers; files via RAII/`fstream`; sanitizer-clean. *(Reflective — grade the reasoning, not a fixed key.)*

**Q12.2 [Open]** Where did you use value semantics vs `unique_ptr` vs `shared_ptr` — justify one of each.
*Key points:* single-owner trees → `unique_ptr`; value types by `const&`; `shared_ptr` only for genuinely shared lifetime. *(Reflective.)*

**Q12.3 [Open]** Describe a template- or move-related compiler error you hit and how you diagnosed it.
*Key points:* reading the instantiation stack, checking value categories / which special member was chosen. *(Reflective.)*

**Q12.4 [Open]** Which parts of the capstone most deserve unit tests, and one edge case you'd assert?
*Key points:* pure logic (parser, container ops, computation); edge cases — empty input, capacity boundaries, error paths returning `std::expected`.

**Q12.5 [Open]** Explain one design tradeoff you made (e.g., `unique_ptr` vs value, exceptions vs `expected`) and why.
*Key points:* weigh ownership clarity, performance, safety. *(Reflective.)*

---

## Module 13 — Concurrency (optional)

**Q13.1 [MCQ] (UB)** Two threads writing the same non-atomic variable without synchronization is:
- A) Fine if fast enough
- B) A data race → undefined behavior ✅
- C) Automatically serialized
- D) A compile error

*Answer: B.*

**Q13.2 [Open]** How does RAII apply to mutexes, and which lock-guard types exist?
*Key points:* lock in ctor, unlock in dtor → exception-safe; `std::lock_guard` (simple scope), `std::unique_lock` (movable/deferrable, works with condition variables), `std::scoped_lock` (multiple mutexes, deadlock-avoiding).

**Q13.3 [MCQ]** `std::atomic<int>` provides:
- A) A mutex
- B) Usually lock-free atomic read-modify-write without a data race ✅
- C) Thread creation
- D) A condition variable

*Answer: B.*

**Q13.4 [Open]** What does `std::async` return, and how do you get the result?
*Key points:* a `std::future<T>`; call `.get()` to block for the value (or rethrown exception); launch policy controls async vs deferred.

**Q13.5 [MCQ] (UB)** To detect data races at runtime, build with:
- A) `-fsanitize=address`
- B) `-fsanitize=thread` ✅
- C) `-O2`
- D) clang-format

*Answer: B.*

**Q13.6 [MCQ]** A `std::thread` destroyed without `join()` or `detach()`:
- A) Joins automatically
- B) Calls `std::terminate` ✅
- C) Detaches automatically
- D) Is fine

*Answer: B.*

**Q13.7 [Open]** Define a data race precisely, and the minimum needed to avoid one.
*Key points:* two threads access the same location, ≥1 writes, no synchronization/happens-before between them → UB. Avoid via mutex/atomic/other ordering.

**Q13.8 [Code] (UB)** Why is this wrong, and the fix?
```cpp
int counter = 0;
auto inc = [&]{ for (int i=0;i<100000;++i) ++counter; };
std::thread t1(inc), t2(inc);
t1.join(); t2.join();
// counter == 200000 ?
```
*Key points:* `++counter` on a shared non-atomic int from two threads is a data race → UB; result unpredictable (often < 200000). Fix: `std::atomic<int>` or a mutex.

**Q13.9 [MCQ]** A `std::condition_variable` lets a thread:
- A) Lock a mutex
- B) Wait until notified of a condition, releasing the lock while waiting ✅
- C) Create threads
- D) Count references

*Answer: B.*

**Q13.10 [Open]** Why pass a predicate to `cv.wait(lock, predicate)`?
*Key points:* guards against spurious wakeups and lost/early notifications — the predicate is rechecked on wake, so the thread proceeds only when the condition truly holds.

**Q13.11 [MCQ]** `std::lock_guard` vs `std::unique_lock`:
- A) Identical
- B) `unique_lock` is more flexible (deferred lock, early unlock, movable, condition-variable-compatible) at slightly higher cost ✅
- C) `lock_guard` is movable
- D) `unique_lock` doesn't unlock

*Answer: B.*

**Q13.12 [Open]** What does `std::future::get()` do if the async task threw?
*Key points:* the exception is stored and rethrown in the calling thread at `.get()` — exceptions cross the async boundary.
