# 🍝 Philosophers: Concurrency & Threading in C

![C](https://img.shields.io/badge/language-C-blue.svg)
![Multithreading](https://img.shields.io/badge/concept-Multithreading-orange.svg)
![Concurrency](https://img.shields.io/badge/concept-Concurrency-lightgrey.svg)

## 📌 Overview
The "Philosophers" project is a classic synchronization problem (The Dining Philosophers) implemented in C. The main objective of this project is to understand the core principles of threading and shared memory management while strictly avoiding **deadlocks** and **data races**.

In this simulation, philosophers sit at a round table, alternating between eating, thinking, and sleeping. To eat, a philosopher must pick up two forks (one on the left, one on the right). The challenge lies in coordinating these actions perfectly across multiple threads without allowing them to block each other or cause inconsistent states in shared data.

## 🧠 What I Learned (The Technical Core)
This project provided deep, practical experience in concurrent programming:
* **Multithreading:** Mastered the creation and management of independent threads using the POSIX `pthread` library.
* **Mutexes:** Implemented `pthread_mutex` locks to protect shared resources (forks and logging) from concurrent access.
* **Time Management:** Developed a custom sleep function using `gettimeofday()` to achieve higher precision than standard `usleep()`, ensuring death checks are accurate within a 10ms margin.
* **Deadlock Prevention:** Designed a resource acquisition hierarchy to ensure philosophers never enter a state of circular waiting.
* **Global Variable Constraints:** Successfully managed all simulation data without the use of global variables, ensuring a clean and modular architecture.

## ✨ Project Architecture
* **Entity Representation:** Each philosopher is represented as a separate **thread**.
* **Resource Management:** Each fork is a shared resource protected by a **Mutex**.
* **Thread Safety:** All state changes (eating, sleeping, thinking) and the final "death" message are synchronized to prevent overlapping or garbled output.

## 🛠️ Installation & Usage

### Prerequisites
* `gcc` or `clang` compiler
* `make`
* `pthread` library

### Compiling
Navigate to the `philo` directory and run `make`:

```bash
git clone git@github.com:menosgrande00/philo.git
cd philo
make
```

### Execution
Run the executable with the following arguments:
```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

### Arguments

* number_of_philosophers: Total number of philosophers and forks.
* time_to_die (ms): If a philosopher doesn't start eating within this time, they die.
* time_to_eat (ms): Time it takes to eat (holding two forks).
* time_to_sleep (ms): Time spent sleeping.
* [number_of_times_each_philosopher_must_eat] (optional): Simulation stops if everyone eats this many times.

### Example
```bash
# 5 philosophers, 800ms to die, 200ms to eat, 200ms to sleep.
./philo 5 800 200 200
```
