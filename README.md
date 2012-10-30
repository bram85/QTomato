QTomato
=======

This is QTomato, a Qt implementation of the Pomodoro technique.

Pomodoro is a technique to increase your productivity by working on a task
without interrupts for 25 minutes. Once done, you have a short break of five
minutes before you start a new 'pomodoro'. For more information, including a
free e-book, on this technique, please go to the [Pomodoro Technique page] [1].

QTomato allows you to adjust the length of the pomodoros and the breaks. You
can also configure a penalty and a reward factor. A *penalty* is incurred when
your break is taking longer than it should. Each additional second is
multiplied with the penalty factor and added to the length of the next
pomodoro. For example, if the penalty factor is 2.0 and a break takes 60
seconds longer than planned by QTomato, the next pomodoro will be 60 * 2.0 =
120 seconds longer.  Similarly, the *reward* factor is calculated when you work
longer than planned. The extra seconds are multiplied with the reward factor
and added to your next break. Typically, the penalty factor is higher than 1.0
and the reward factor is lower than 1.0. By default, both factors are 0.0,
which complies with the default Pomodoro technique.

This project is hosted on [Github] [2]. Contributions are always welcome.

[1]: http://www.pomodorotechnique.com "Pomodoro Technique"
[2]: http://github.com/bram85/QTomato "github Project Page"
