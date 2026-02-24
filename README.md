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

The simulation ends when a coder burns out or when all required compile cycles are completed. This project focuses on concurrency, synchronization primitives, scheduling algorithms, and precise timing control using POSIX threads.

---

## Instructions

### Compilation

Compile the project using:

```bash
make
```

This will generate the executable:

```bash
./codexion
```

### Execution

```bash
./codexion <num_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <num_dongles> <cooldown_time> <fifo|edf>
```

**Parameters (all times in milliseconds):**

| Parameter | Description |
|:---|:---|
| num_coders | Number of coder threads |
| time_to_burnout | Maximum time without compiling before burnout |
| time_to_compile | Duration of compile phase |
| time_to_debug | Duration of debug phase |
| time_to_refactor | Duration of refactor phase |
| num_dongles | Number of shared dongles available |
| cooldown_time | Cooldown before requesting dongles again |
| fifo / edf | Scheduling policy |

**Example:**

```bash
./codexion 3 1500 400 300 200 5 100 edf
```

## Blocking Cases Handled

This project carefully handles several concurrency issues to ensure the simulation runs correctly and coders never starve:

- **Deadlock prevention and Coffman's conditions:** Dongles are always requested in a consistent order (lower ID first) to prevent circular waits. This guarantees that the system cannot reach a deadlock state, satisfying Coffman's conditions.

- **Starvation prevention:** With the EDF (Earliest Deadline First) scheduler, coders with the closest burnout deadline are prioritized when acquiring dongles. This ensures no coder waits indefinitely and all coders eventually get to compile.

- **Cooldown handling:** After releasing dongles, a cooldown period is enforced before a coder can request them again. This prevents fast re-acquisition that could cause unfair resource monopolization.

- **Precise burnout detection:** A separate monitor thread continuously checks each coder's last compile time. Burnout is detected precisely when a coder exceeds `time_to_burnout` milliseconds without starting a compile, and the simulation stops immediately.

- **Log serialization:** All console outputs (logs) are protected by mutexes to ensure that messages from multiple threads do not interleave. This guarantees readable and consistent logs.

## Thread Synchronization Mechanisms

In **Codexion**, several threading primitives are used to ensure safe access to shared resources and proper coordination between coders and the monitor:

1. **`pthread_mutex_t` (mutexes)** - Protects **shared data structures** such as:
   - `dongle->in_use` - whether a dongle is currently held
   - `coder->last_compile_start` - timestamp of the last compile
   - `monitor->stop` - global stop flag for the simulation
   - `log_mutex` - ensures logs are printed sequentially without overlap

   **Example:** Before reading or updating a coder's `last_compile_start`, the thread locks the coder's mutex to prevent simultaneous reads/writes.

2. **`pthread_cond_t` (condition variables)** - Implements **waiting queues** for dongles.
   - Coders unable to acquire dongles immediately wait on the dongle's condition variable.
   - When a dongle is released, `pthread_cond_broadcast` wakes waiting coders.

   **Example:** A coder finishing compiling releases the dongle and signals others to try acquiring it.

3. **Custom timing and events** - Manages temporal constraints:
   - Each dongle has a **cooldown timer** after being released to prevent immediate reacquisition.
   - The monitor thread checks for **burnout** by reading coders' `last_compile_start` under mutex protection.
   - Timed waits (`pthread_cond_timedwait`) allow coders to sleep until cooldown expires or a burnout stop occurs.

4. **Preventing race conditions** - Ensures data consistency:
   - Every access to shared data is **guarded by the appropriate mutex**.
   - Logging uses both `monitor_mutex` and `log_mutex` to prevent inconsistent output.
   - This ensures no two threads modify or read shared resources simultaneously.

5. **Thread-safe communication** - Coordinates between threads:
   - Coders and the monitor communicate via shared variables (`last_compile_start`, `stop`) and condition variables.
   - When burnout is detected, the monitor sets `monitor->stop = true` under mutex and broadcasts to all dongle condition variables.
   - This ensures coders are notified promptly and exit gracefully, keeping the simulation state consistent.

## Resources

**References and Documentation:**

- [POSIX Threads (Pthreads) Documentation](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html) – Official reference for thread management, mutexes, and condition variables.
- [GeeksForGeeks Thread Functions Tutorial](https://www.geeksforgeeks.org/c/thread-functions-in-c-c/) – Tutorial with practical examples of thread creation, synchronization, and management.

**AI Usage:**

AI assisted with project organization and code structure planning and helped to understand how some things work.
