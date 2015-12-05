---
output: pdf_document
---
# Real Time Simulator 

This is a project for Real Time Systems course. The main goal of this project is to create set real time tasks (with configurable execution times and relative periods) mesure worst case responce time and compare this responce time with theoretical responce time computed using RTA.


## Requirments
* libgonfig (I instaled it like this sudo apt-get install libconfig++8-dev)
* gnuplot (Only if run with --plot (-p) option)
* astyle (Only for code formating e.g. make indent)

## Usage
Usage: simulator [\-f file] [\-o directory] [\-s number] [\-plc]

* \-f, \-\-file      (file with tasks parameters (default task_spec.cfg))
* \-o, \-\-output    (directory to which put simulated responce times of the tasks (default no output))
* \-s, \-\-simtime   (time of simulation in seconds (default 10s))
* \-p, \-\-plot      (plot task CDF (usign gnuplot))
* \-k, \-\-latency   (run tasks with hight kernel latency and estimate kernel latency)
* \-c, \-\-counting  (use counting to model execution time (instead of busy wait on timer))
* \-h, \-\-help     (run help command)

### File option
File is file in libconfig format (see example task_spec.cfg). Here you can specify tasks parameters, e.g.
to which cpu task will be assigned (cpu option), task priority (priotiry option), inter arrival times (period option) for this option you should specify a pmf, and execution time (execution option) also pmf should be specified.


## Execution time modeling
To model execution times there are 2 approaches:

Counting:

```cpp
long long countTo = executionTime / HowLongOneAdditionTake;
for (long long i = 0; i < countTo; ++i) {}
```

Timer busy waiting:

```cpp
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_time);
while (start_time + executionTime > current_time) {
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &current_time);
}
```
Roughly counting gives an error +-0.5ms, and timer busy waiting gives an error +0.3ms.
So the timer busy waiting is default option (you can specify it with -c option).

## Latency generation
Latency generated by running on each CPU that tasks uses, dummy thread which fork() and immediately kill the child every 1ms.

## Sample result 
Result for Task Set Task1((100ms, 200ms), (10ms, 15ms, 20ms)), Task2((60ms, 50ms), (10ms, 19ms)), Task3((50ms, 10ms)).
First is inter arrival time, second is execution time. Everything with equal probabilities. <BR/>
Priority of Task1 - 81, Task2 - 82, Task3 - 83.

#### Responce time analysis
* Task1: worst case response time 49ms
* Task2: worst case response time 29ms
* Task3: worst case response time 10ms

#### Run without -l
* Task1: worst case response time 49.320491ms
* Task2: worst case response time 29.584214ms
* Task3: worst case response time 10.261378ms

#### Run with -l
* Task1: worst case response time 79.245297ms
* Task2: worst case response time 30.486118ms
* Task3: worst case response time 11.436412ms
* Worst case kernel latency: 1.486000ms










