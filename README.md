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
<sort_algorithm>,<size_of_structure> ,<size_of_structure (min. 1)>, <max_value>, <number of repeats>
```