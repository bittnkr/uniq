This history started almost 10 years ago, on the the night of November 10, 2011.

Back to home, I couldn't turn off my mind from a need I had at work: Speed up a daily task that was taking hours to be completed.

Buying another server was not the solution. We already had the best we could buy. A brand new 16 core server. Which could finish the job just 20% faster then its antecessor.

That task was part of a legacy system running in a single thread process.

And was my task to write a multithreading version of it, splitting up the data the in smaller pieces, execute each one in parallel and glue the results at the end. So that one hour task would be finished in about 4 minutes.

Figuring a way to break my data in smaller chunks to send to some imaginary running process of my system, before falling asleep, Leaving a breadcrumb to myself tomorrow, I took a piece of paper and wrote:

```
send(data, actor);
```

From this door, in the next days I started designing a new class model. At same time, something "magical" was happening to me. Taken by what I use to call The Muse, I started having waves of inspiration, where every question I posed was just accompanied by the solution. And each solution leading to a new intriguing question.

I was having some kind of epiphany. For some brief moments I had glimpses of higher dimensions. And everything just made sense, with an overwhelming flow of ideas that came chaotically to my mind at an incredible speed that I could not even type. So I took a pen and a half-used statistics notebook and started writing furiously.

At that point I had been working with software development for 20 years and finally I was able to see the whole picture. Not only of the computer science but also of my own life and existence.

The puzzle was being completed. And I was realizing that everything is just a part of a 'bigger machine' in a level I could never imagined.

But suddenly, by the end of the year, just as she came, the capricious muse was gone. And all I had from her visit was a notebook full of squiggles and some proof of concepts written in pascal.

Of these, the most valuable was a small implementation of a *lock-free circular buffered queue*. Which is the reason of this writings and the base of [UniQ][1] multithreading libray. hence the name, *uni-queue*, a single unifying queue.

You may ask, what the heck is this and why should I care?

I will answer this in some minutes, but before I must tell you what others are saying about this problem.

In a question in Stackoverflow about [Circular lock-free buffer][2], (asked 11 years ago), the most upvoted answer says:

> I've made a particular study of lock-free data structures in the last couple of years. I've read most of the papers in the field (there's only about fourty or so - although only about ten or fifteen are any real use

> AFAIK, a lock-free circular buffer has not been invented.

In [another S.O. question][3] someone said:

> Lock-free queues are unicorns.

Searching the literature, we found no more encouraging words: The book [The Art of Multiprocessor Programming][4] (2012) says that the construction of a wait-free queue is impossible:

> Corollary 5.4.1. It is impossible to construct a wait-free implementation of a queue, stack, priority queue, set, or list from a set of atomic registers.

> Although FIFO queues solve two-thread consensus, they cannot solve 3-thread consensus. (pg. 107)

And that was the gift of the muse, the pearl behind the cryptic squiggles in the black book:

A surprisingly small and simple solution to the 3-thread consensus using only 2 atomic registers.

It is so simple that when I first figured it out I thought: Why hasn't anyone seen this before?

In the next articles, I will guide you trough the line of thought she gave me and everything will make sense.

After the Muse departure, the black book got the drawer bottom and I was back to my life of middle aged IT guy, in a broken marriage, solving problems of others while my own life was a mess.

But something had changed in me... After those glimpses of the higher dimensions, I was not the same person anymore.

I use to say that the year 2012 was my personal revelation and my world has came to an end. Then, I took the hardest decision in my life and got divorced. Wasn't easy to end a 18-year marriage, but a yearning was burning in my heart and just needed to go get it.

When in tears, my daughter asked me why I was leaving her, the only answer I had was: I'm not leaving you. I'm going to find myself.

Then, the book got a safe place in my mom's closet. I got a backpack, a warm jacket and a good pair of boots, and went to walk the world. Learn how to say slangs and tell dirt jokes in foreign languages. (There are some things that sounds funnier when said by a foreigner.)

When someone asked me what I was doing in the winter in "La Tierra del Fuego" near the Antarctica I just answered in spanish: *Para cometer nuevos errores* (To make new mistakes) because I was tired of doing the same over and over again.

And that's the reason why took me almost 10 years to bring you the gift of the Muse.

Maybe someday, around a campfire, we could share histories of shamanic experiences with the indian tribes in the Amazon forest or the sky of the Andes, but for now, I need to help you unlock your computer.

If you like to think about fundamental problems of computer science and want to get the most of your computing power, stick with me and I'll take you on a beautiful journey through the magic world of parallel computing and its sorcery.

See you in the next chapter:

Multithreading 101 (Soon)

[1]: https://github.com/bittnkr/uniq
[2]: https://stackoverflow.com/a/890269/9464885
[3]: https://stackoverflow.com/questions/6089029/lock-free-queue#comment7056198_6089029
[4]: https://www.amazon.com.br/Art-Multiprocessor-Programming-Revised-Reprint/dp/0123973376