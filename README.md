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
`$ ./build/timecard <time1> <label1> <time2> <label2> <timeN> <labelN> <timeN+1>...`

Starting with the first time (using 24-hour time), list all times that activities changed along with the activities that occurred between adjacent times. If you spend 8:00am to 10:00am gardening then 10:00am to 11:00am reading, the arguments would look like this:

`$ ./build/timecard 0800 gardening 1000 reading 1100`

and you should expect to see the following returned in the console:

```
gardening: 120 minutes
reading: 60 minutes
```

If multiple chunks of time were spent on one activity, then simply include whenever the additional chunks happening. Expanding on our previous example, lets add two more chunks for eating lunch and reading again.

`$ ./build/timecard 0800 gardening 1000 reading 1100 lunch 1230 reading 1400`

This yields:

```
gardening: 120 minutes
lunch: 90 minutes
reading: 150 minutes
```

Because the label `reading` appeared twice, its two durations were accumulated. This will be done for any labels which appear more than once.
