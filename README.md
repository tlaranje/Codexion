*This project has been created as part of the 42 curriculum by tlaranje.*

# Codexion

## Description

Codexion is a multithreaded simulation project that models a group of coders competing for limited shared resources (dongles) in order to compile their code. Each coder follows a cyclic workflow consisting of:

- Taking dongles
- Compiling
- Debugging
- Refactoring
- Cooling down

The main objective of the project is to implement a correct and efficient thread synchronization system that ensures:

- No data races
- No deadlocks
- No starvation (when parameters are feasible)
- Accurate burnout detection
- Correct scheduling behavior using FIFO or EDF policies

The simulation ends when a coder burns out or when all required compile cycles are completed.

This project focuses on concurrency, synchronization primitives, scheduling algorithms, and precise timing control using POSIX threads.

---

## Instructions

### Compilation

Compile the project using:

	make

This will generate the executable:

	./codexion

ThreadSanitizer is enabled to detect data races.

### Execution

	./codexion <num_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <num_compiles> <dongle_cooldown> <scheduler>

**Parameters (all times in milliseconds):**

| `Parameter`        | `Description`                                        |
|:-----------------|:---------------------------------------------------|
| num_coders       | Number of coder threads                            |
| time_to_burnout  | Maximum time without compiling before burnout (ms) |
| time_to_compile  | Duration of compile phase (ms)                     |
| time_to_debug    | Duration of debug phase (ms)                       |
| time_to_refactor | Duration of refactor phase (ms)                    |
| num_dongles      | Number of shared dongles available                 |
| cooldown_time    | Cooldown before requesting dongles again (ms)      |
| fifo / edf       | Scheduling policy                                  |

**Example:**

	./codexion 3 1000 400 300 200 5 100 edf
