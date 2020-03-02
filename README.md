# CircularBuffer

CircularBuffer is an api implemented in C for Nordic Semiconductor.
Coding and building was performed in Geany and tests implemented with Unity.

## Installation


```bash
gcc -Wall -o TestRunner ../src/CircularBuffer.c TestCircularBuffer.c TestRunner.c /usr/local/lib/Unity/src/unity.c /usr/local/lib/Unity/extras/fixture/src/unity_fixture.c 

```

## Usage

```bash
./TestRunner

Terminal output can be toggeled on/off via the PRINT_INFO define in either TestCircularBuffer.c
or CircularBuffer.c.
```


## Author and date

Richard Hermstad McCrae
15.01.2019
