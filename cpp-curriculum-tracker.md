# C++ Curriculum — Progress Tracker

Tick boxes as you go. Use this alongside **cpp-curriculum.md** (the full detail — videos,
readings, exercise specs) and **cpp-curriculum-quiz.md** (self-test). Each module here has a
box for watching, reading, each "Do" task, and the self-quiz. The task labels are short on
purpose — the real instructions live in the curriculum file.

> Clickable checkboxes that save back to the file work in **Obsidian**, **VS Code** (with a
> preview/Markdown extension), **Typora**, **Logseq**, and on **GitHub/GitLab**. See the note
> at the very bottom for picking one.

## Overall progress
- [ ] Module 0 — Orientation, toolchain & UB
- [ ] Module 1 — Value semantics, object model & views
- [ ] Module 2 — RAII, destruction & exception safety
- [ ] Module 3 — Copy, Rule of 0/3/5 & initialization
- [ ] Module 4 — Move semantics & value categories
- [ ] Module 5 — Polymorphism the C++ way
- [ ] Module 6 — Smart pointers
- [ ] Module 7 — const correctness
- [ ] Module 8 — Templates & generic programming
- [ ] Module 9 — STL + rebuilt containers
- [ ] Module 10 — Modern C++ glue
- [ ] Module 11 — Build systems & dependency management
- [ ] Module 12 — Capstone
- [ ] Module 13 — Concurrency (optional)

---

## Module 0 — Orientation, toolchain & UB
- [ ] Watch the videos
- [ ] Read: *Tour of C++* Ch. 3
- [ ] Do 1 — compile a C program of yours as C++, fix every warning
- [ ] Do 2 — trigger 3 classic UBs (use-after-free, signed overflow, OOB read) under the sanitizer
- [ ] Self-quiz: Module 0

## Module 1 — Value semantics, object model & views
- [x] Watch the videos
- [x] Read: *Tour of C++* Ch. 1–2 + learncpp references/pass-by sections
- [x] Do 1 — byValue / byRef / byConstRef: compare addresses (copy vs alias)
- [x] Do 2 — `Tracked` destruction-order prediction across nested scopes
- [x] Do 3 — dangling `string_view` demo + fix
- [x] Self-quiz: Module 1

## Module 2 — RAII, destruction & exception safety
- [ ] Watch the videos
- [x] Read: *Tour of C++* Ch. 5 + Ch. 4
- [x] Do 1 — `ScopedFile` that closes even when an exception is thrown
- [x] Do 2 — `parseInt` two ways (throw vs `std::expected`) + argue the tradeoff
- [ ] Self-quiz: Module 2

## Module 3 — Copy, Rule of 0/3/5 & initialization
- [ ] Watch the videos
- [ ] Read: *Tour of C++* Ch. 6 + Effective Modern C++ Items 17 & 7
- [ ] Do 1 — minimal `String` (ctor, dtor, copy ctor, copy assignment), sanitizer-clean
- [ ] Do 2 — initialization gotchas (narrowing + most-vexing-parse) + your default rule
- [ ] Self-quiz: Module 3

## Module 4 — Move semantics & value categories
- [ ] Watch the videos
- [ ] Read: *Tour of C++* Ch. 6 (move) + Effective Modern C++ Items 23–25
- [ ] Do — add move ctor + move assignment to `String`; trigger all five special members; verify moved-from is valid/empty
- [ ] Self-quiz: Module 4

## Module 5 — Polymorphism the C++ way
- [ ] Watch the videos
- [ ] Read: *Tour of C++* Ch. 5 (hierarchies) + Effective C++ virtual-destructor item
- [ ] Do 1 — `Shape` hierarchy, polymorphic `area()` via `vector<Shape*>` (raw pointers), `override`
- [ ] Do 2 — slicing demo + one-sentence explanation
- [ ] Do 3 — virtual-destructor leak demo + fix
- [ ] Do 4 — casting: `dynamic_cast` over the shapes (check `nullptr`) vs unsafe `static_cast`
- [ ] Self-quiz: Module 5

## Module 6 — Smart pointers
- [ ] Watch the videos
- [ ] Read: *Tour of C++* Ch. 15 + Effective Modern C++ Items 18–21 + learncpp smart pointers
- [ ] Do 1 — implement a simplified move-only `UniquePtr<T>`
- [ ] Do 2 — convert a raw `new`/`delete` program (reuse `Shape`) to `std::unique_ptr`, zero leaks
- [ ] Self-quiz: Module 6

## Module 7 — const correctness
- [ ] Watch the videos
- [ ] Read: learncpp const + const member-functions chapters
- [ ] Do — add `const` member functions to `String`; make a `const String` and confirm mutation is blocked
- [ ] Self-quiz: Module 7

## Module 8 — Templates & generic programming
- [ ] Watch the videos
- [ ] Read: *Tour of C++* Ch. 7–8 + Effective Modern C++ Items 1–4 + learncpp templates
- [ ] Do — STL rebuild #1: `Vector<T>` (push_back, operator[], copy, move) + a movable `concept`
- [ ] Self-quiz: Module 8

## Module 9 — STL + rebuilt containers
- [ ] Watch the videos
- [ ] Read: *Tour of C++* Ch. 12–14
- [ ] Do 1 — STL rebuild #2: `List<T>` + a real forward iterator (works in range-for)
- [ ] Do 2 — STL rebuild #3: `Stack<T>` adaptor over your `Vector<T>`
- [ ] Do 3 — STL rebuild #4: `HashMap<K,V>` (chaining, rehash)
- [ ] Do 4 — word-frequency top-10 with `unordered_map` + `sort`/`partial_sort`; diff vs your `HashMap`
- [ ] Self-quiz: Module 9

## Module 10 — Modern C++ glue
- [ ] Watch the videos
- [ ] Read: *Tour of C++* Ch. 16 + Effective Modern C++ Items 5–6 & 30–31
- [ ] Do 1 — lambda + standard algorithm; then a dangling-capture bug + by-value fix
- [ ] Do 2 — STL rebuild #5 (stretch): `Optional<T>` via aligned storage + placement `new`
- [ ] Self-quiz: Module 10

## Module 11 — Build systems & dependency management
- [ ] Watch (context): Cherno static/dynamic library videos
- [ ] Read: CMake Tutorial + *Modern CMake* + vcpkg getting-started
- [ ] Do 1 — split `Shape` into header/source, build with `CMakeLists.txt` (cxx_std_20)
- [ ] Do 2 — add `fmt` via vcpkg and call it
- [ ] Do 3 — Catch2 test target over `Vector`/`HashMap` + run clang-tidy (cppcoreguidelines) and fix one warning
- [ ] Self-quiz: Module 11

## Module 12 — Capstone
- [ ] Chose a project (calculator / key–value store / ECS)
- [ ] Core functionality working
- [ ] All resources owned via RAII / smart pointers (no raw owning pointers)
- [ ] Builds via CMake with at least one external dependency
- [ ] Has a unit-test target
- [ ] Sanitizer-clean (address + undefined)
- [ ] Self-quiz: Module 12 (reflective)

## Module 13 — Concurrency (optional)
- [ ] Watch the videos
- [ ] Read: *Tour of C++* Ch. 18 + Effective Modern C++ Items 35–40
- [ ] Do 1 — thread-safe queue (mutex + condition_variable) with producer/consumer threads
- [ ] Do 2 — parallel array sum with `std::async`, timed vs single-threaded, checked with `-fsanitize=thread`
- [ ] Self-quiz: Module 13

---

### Where to edit and tick this off

- **Obsidian** (free, recommended for this) — point a vault at the folder with these three files;
  checkboxes are clickable and save straight back to the `.md`. Best "click to tick" experience.
- **VS Code** — install *Markdown All in One*; toggle a checkbox with a shortcut, or click it in
  the preview pane. Good if you already live in VS Code.
- **GitHub/GitLab repo** — renders task lists and gives you version history + access anywhere; tick
  by editing the file (or via rendered checklists). Best if you want it backed up and synced.
- **Typora / Mark Text / Logseq** — all render and toggle task lists fine too.

Avoid pasting into Google Docs/Notion if you care about the Markdown — they'll mangle the checkbox
syntax on import. Keep it as a plain `.md` file in one of the tools above.
