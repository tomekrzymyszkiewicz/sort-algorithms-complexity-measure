# Sort algorithms complexity measure
A program that measures the time and memory complexity of the quick sort and counting sort algorithms.

---

## Using complexity-measure program
Set properties in configuration file:

- choose file with data to sort
- choose file when the results will be saved
- enter a series of task to test by the program

---

## config.ini syntax

```
<data_source (*.csv)>
<results_destination (*.csv)>
<task>
<task>
.
.
.
```
Task syntax
```
<sort_algorithm>,<min size of array> ,<max size of structure>, <step>, <number of repeats>
```

---

## data-config.ini syntax

```
<data_file (*.csv)>
<number of elements>
<min value>
<max value>
```
