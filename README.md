# About Q

a simple todo-queue management tool

## Why isn't this repository real?

Okay, that's obviously a contrived question.  Still:

The "home" version control repository is [at FossRec][qfr].

I also mirror the repository [on GitHub][qgh].

I wrote a tool called [FossGit][fg] to help with this mirroring process.

Now you know everything.

## Why doesn't it do everything?

This is a C rewrite of the original shell script implementation.  The C version
is nearly feature-complete, but it still lacks the ability to remove arbitrary
queue entries by number, which is not strictly necessary and should not be part
of standard q workflow anyway.  I should probably call this a complete version
1.0 release, but so far I'm too indecisive to call it anything in particular.

It is also intended to get somewhat close to adhering to the Unix philosophy of
software design: Do One Thing Well.  As such, it should not do things that are
not part of the core purpose of the tool.  If you think it needs a feature it
lacks, feel free to let me know, but I do not anticipate getting many "good
idea" feature requests for this project.  See more about this in the "Why is q
awesome?" section below.

If you're interested, you can still get [the source of the shell version][sh],
which has a working implementation of the `remove-number` command.  It is also
probably a less-safe program, because I'm too lazy to do much in the way of
error-checking with shell script code, which I find excruciating.  Writing C is
fun, though.

## Why is q awesome?

Todo lists and task trackers abound.  There are many ways to track what you
need to do.  Some of them are very useful for specific types of task tracking.
None of them seem to be useful everywhere, but almost all of them assume
everything can be handled by one tool, and aim to be that tool.

This is not one of those tools.  The purpose of q is to provide a dead-simple
way to manage a queue of things you need to do.  It is not a checklist; for
things you do every day, you probably need a checklist.  It is not a scheduling
tool; for things that need to be done at or by a specific time in the near
future, or just at a specific time in the less-near future, you probably need a
scheduler.  There are many other approaches to tracking things you need to do
out there that handle some types of tasks effectively, where q does not handle
those types of tasks well at all.

A scheduler is great at handling scheduled events (as long as it's a great
scheduler; unfortunately, most of them suck), but it probably sucks at
every-day checklists, and to the extent it is good at checklists it probably
compromises on being the best scheduler it could have been.  A checklist system
is great at handling every-day tasks without a schedule (as long as it's a
great checklist system; unfortunately, most of them that do not use paper are
pretty mediocre), but it probably sucks at scheduled task management, and to
the extent it is good at scheduling it probably compromises on being the best
checklist system it could have been.

Both of these things tend to suck at tracking the stuff that has no schedule,
only needs to be done once, and does not necessarily have to be done today, but
should not be put off, either.  When there are things with specific completion
conditions that you should do "soon" but not on a specific schedule, and you
need to keep track of a fairly short list of such things so you will not forget
to do any of them, what you really need is a task queue.  If you have a daily
checklist of things to do, you might put something in there about checking your
task queue, if only to make sure you actually try to clear an item or two from
the queue every day, but the checklist itself is not the task queue.

A great thing about task queues is that you can effectively rotate them by
popping something off the front of the queue and sticking it in the back.  If
you have time to work on the tasks in your queue, and you are not sure off the
top of your head what you want to tackle first, just check what's in the front
of the queue; if it's not something you can do right now, just pop it off the
front of the queue and stick it into the back for later.  Do the next thing.
This approach removes a lot of the mental administrative overhead of getting
things done, because you no longer have to try to consider all the things in
your todo list, weighing priority, current conditions, and personal readiness
to do the work, before starting work.  Just determine whether the one thing
sitting there staring at you is something you will do right now, and if not, do
the next thing instead.

Another great thing about task queues that you can rotate that way is that,
when you look at the task in the front of the queue, it's always the task you
have neglected the longest.  Because the only way to put off the current item
in the queue (as long as you actually use it as a queue) is to move it to the
back of the queue, you have to look at every other task currently in the queue
again before it comes up once more, ensuring that none of those tasks get
completely forgotten.  It helps keep you from forgetting all about that
critically important thing you just wanted to put off until next time.

The q tool is an idealized implementation of a task queue, keeping things as
simple and straightforward as possible to help you get things done.  It offers
a few features other than strict queue management, such as looking at the
entire list or deleting a single item out of the middle of the list, but that
is only because people make mistakes and sometimes we need a way to clean up
after them.  Those operations are intentionally made slightly less convenient
than the primary feature set of a task queue.  The shortest commands in the q
tool are `show` (because you can just execute `q` and get `show` for free),
`help` (because you can abbreviate it as `h`), `add`, `del`, and `rotate`
(because you can abbreviate it `rot`).

The things you "should" do when using q "correctly" are the things that should
be easiest, and most convenient, to do.  Almost everything else is a
second-class citizen in the world of q.  The single exception is
`create-fresh-queue`, the command to set up q for use in the first place, a
first-class operation that should only ever be used once in a given user
environment.  It is the least convenient command (with the possible exception
of `remove-number`, if only because `remove-number` requires you to know the
number of the item you want to remove, thus necessitating either `list-all`
first or a truly superlative memory) because it is the one operation you should
never use after initially setting up q for use.

Superficially, a checklist and a queue are very similar.  In terms of a couple
of key, defining characteristics, and a deeper philosophical usage model, they
are like night and day: part of a continuum of  similar functionality, but with
radically different benefits.  Checklist are great when you have a single,
multi-step task to complete, following which you throw away the list.

Checklists are also great when you just need to have the list on-hand to make
sure you do not forget any of a set of important tasks that must be performed
over and over again.  They can be a real pain in the butt for managing the
kinds of tasks for which q was designed.

If you add the features to a checklist system that make a good task queue
system, you get a task queue management system.  It is no longer a checklist
system -- not really, even if it has a bunch of checklist features layered on
top.  If its underlying structure is still the sort of thing best optimized for
checklist use, it will not be as good a task queue as you could have had,
either.  A queue system can sometimes be used well for checklists, but if you
need it to be useful for all checklists, or even if you just need to be able to
rearrange the things in the queue to ensure sequential step completion in a
particular order, either it fails to be very good at checklist management or
your task queue system has compromised as a task queue system to the extent
that it probably hurts its usefulness for queue management.

This tool is intended to do exactly one thing, and do it well.  It is designed
that way to make our lives simpler, and help us manage a particular class of
the must-do tasks we face, in the most effective way it can.  Toward that end,
it refuses to be anything but a task queue management tool, because trying to
be everything to everyone is a great way to fail to be the best at anything to
anyone at all.

[fg]: https://fossrec.com/u/apotheon/fossgit
[qfr]: https://fossrec.com/u/apotheon/q
[qgh]: https://github.com/apotheon/q
[sh]: http://sprunge.us/ZJRZ
