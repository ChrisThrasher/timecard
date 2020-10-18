# timecard

Calculates how much time was spent on various activities throughout the day.

For help:

```
$ timecard --help
Usage: timecard <time1> <activity1> <time2> <activity2> <time3> <activityN> <timeN>

Times are formatted as 3 or 4 digits followed by either an 'am' or 'pm' suffix.
```

Starting with the time the first activity started, list all times that activities changed along with the activities that occurred between those times. If you spent 8:00am to 10:00am gardening then 10:00am to 11:00am reading, the command would look like this:

```
$ timecard 800am gardening 1000am reading 1100am
gardening: 2.0 hours
reading: 1.0 hours

Total: 3.0 hours
```

If multiple chunks of time were spent on one activity, then include the additional chunks using the same label. Expanding on our previous example, lets add two more chunks for eating lunch and reading some more.

```
$ timecard 800am gardening 1000am reading 1100am lunch 1230pm reading 200pm
gardening: 2.0 hours
lunch: 1.5 hours
reading: 2.5 hours

Total: 6.0 hours
```

Because the label `reading` appeared twice, its two durations were accumulated. This will be done for any labels which appear more than once.

To ignore certain periods of time so that they're not reported, name them `-`. This will exclude them from the printed totals. Here's what that looks like:

```
$ timecard 800am gardening 1000am reading 1100am lunch 1230pm reading 200pm - 900pm reading 1000pm
gardening: 2.0 hours
lunch: 1.5 hours
reading: 3.5 hours

Total: 7.0 hours (7.0 hours off)
```
