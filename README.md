# timecard

Calculates how long was spent on various activities throughout the work day.

## Building
Requires CMake 3.17 and a C++17-compliant compiler
1. Clone repository
1. `$ cd raytracer`
1. `$ mkdir build`
1. `$ cd build`
1. `$ cmake ../`
1. `$ cmake --build .`

## Running
1. `$ ./build/timecard <time1> <label1> <time2> <label2> <timeN> <labelN>...`

Starting with the first time (using 24-hour time), list all times that activities changed along with the activities that occurred between adjacent times. If you spend 8:00am to 10:00am gardening then 10:00am to 11:00am reading, the arguments would look like this:

`$./build/timecard 0800 gardening 1000 reading 1100`

and you should expect to see the following returned in the console:

```
gardening: 120 minutes
reading: 60 minutes
```
