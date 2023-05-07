# Philosophers
Implementation of the **Dining Philosophers Problem**: it is an abstraction to get used to concurrent programming, where multiple entities run independently, sharing resources for which the read/write access must be synchronized.


# Overview
There are n philosophers sitting on a circular table and on their left and right they have a fork, so that the right fork is the left one for the following philo and in total there are n philosophers, n forks, and a lot of delicious meals that stimulate the Philosophy. 

From the beginning, each philosopher does three (mutually exclusive) actions: eat, sleep, or think: they wait for the availability of the forks on their left and right so they can eat, with a full belly they sleep and then they think, so the cycle can start again.

The catch is that a philosopher needs two forks to eat, so they have to take turns for eating: as a consequence at most n/2 philosophers can eat at the same time.

Last but not least, a philosopher dies if too much time passes since their last meal.


# The simulation
## Intro:
The designed solution replicates the behavior of every philosopher: different programs are executed and every program represents a philosopher; when it has access to two forks/resources it stops the execution for x ms (eating), after that leaves the forks and stops again for y ms (sleeping), at the end it waits to use the forks again; the simulation is interrupted when and only when one of this two events occurs:
- A philosopher dies;
- The last philosopher ate enough times, if the optional parameter is given *(see Parameters)*;
When an action is performed (eat, grab a fork, die, ...) a message is printed on stdout with the format:
    `[time_stamp] -- [id_philosopher] [action]`

Furthermore, the solution was implemented with these restrictions:
1. **Philosophers shouldn't die:** the code must be optimized, so the time losses are reducted as much as possible;
2. **Philosophers do not talk with each other:** threads/processes run by themselves: they cannot notify other philosophers of a change in their status, or anything else.

## Parameters:
The program takes some parameters to set the environments *(see #Disclaimer##2 for parameters limitations)*;
1. number of existing philosophers                                    (a positive number is required);
2. number of milliseconds that phil is able to survive without eating     (a non-negative number is required);
3. number of milliseconds that phil needs to eat                      (a non-negative number is required);
4. number of milliseconds that phil needs to sleep                    (a non-negative number is required);
5. number of times the philosopher has to eat, optional               (a positive number is required).

## Eat-Sleep-Think:
The input received is checked and parsed, and after validation, the child programs are executed: the idea is that every philosopher is represented by an independent program that does its actions inside the endless (hopefully) cycle.

Because there can be no exchange of information between the philosophers/programs, how is it possible to terminate the simulation, i.e. to advise all the philosophers of the death of one of their own or that all of them ate max_meals times?

Another program, called monitor, must run behind the scenes, its job is to verify if one of the two ending conditions *(see #Intro)* is satisfied, if so, it tells all the programs to stop their execution.

NB: A philosopher can and **will** die even while eating or sleeping, in case t_eat or t_sleep is greater than the surviving time.


# Approaches
## Threads and mutexes:
The first implementation is located inside folder ```philo```, it creates a thread for every philosopher and another one, the monitor; so with n philos there are n + 2 (main + monitor) threads.
To avoid rata races to shared variables, mutexes are used to protect those:
- **forks:** every fork has its own mutex that makes the use of that fork mutually exclusive;
- **status:** the status of the philosopher (ALIVE, DEAD, ...) is protected because it is read and written both by the philosopher itself and the monitor;
- **last_time_eat:** timestamp of the last meal of the philosopher is protected because it is read and written both by the philosopher itself and the monitor.

## Processes and semaphores:
The second implementation, located inside folder ```philo_bonus```, runs a process for every philosopher and a thread (monitor) for every process; so with n philos there are n + 1 processes and n threads. 
To avoid rata races to shared variables, semaphores are used to protect those:
- **forks**: the total amount of forks is regulated by a semaphore *(see #About the differences##1)*;
- **status**: the status of the philosopher (ALIVE, DEAD, ...) is protected because it is read and written both by the philosopher itself and its monitor;
- **last_time_eat**: timestamp of the last meal of the philosopher is protected because it is read and written both by the philosopher itself and its monitor;
- **stdout**: yes, printf() is already protected for concurrent access, but this semaphore makes sure that after the death of a philosopher no other message will be printed: in fact, to terminate all the philosopher/processes is not really a fast operation, so there's a small delay *(see #About the differences##4)*.

## About the differences:
1. The second approach modifies a little the rules: now the forks are no longer on the left and right of every participant but in the center of the table, so everyone can just pick up two forks from the pile, if there're any available;
2. Processes do not share memory so a single process/monitor can no longer update the variable of every other process, that is why every philosopher needs its own monitor. Finally, once a process ends with a DEATH status, the main process sends a SIG_INT to all of its child processes;
3. Creating processes takes time, so every one of them would start its job a little bit after the previous one, the workaround is that the processes start all together one second after the first process was created, to even all the differences. Threads, on the other side, are faster, so once they're created they start their execution right away, **but** threads with even id are delayed by 5 ms, otherwise every philosopher would grab their left fork and wait for the right one, which is helded by the philo next to them, resulting is a deadlock;
4. It is not possible to use a mutex for the stdout in the thread case, because all the threads must end by themselves (i.e. without a SIG_INT signal), with a mutex they would be stuck waiting for an impossible unlock, again resulting in a deadlock.


# Code
## Compiling and running:
[inside ```philo``` or ```philo_bonus```]
    `make`    creates the executable;
    `./philo[_bonus] n_philos t_death t_eat t_sleep [max_meals]`    runs the executable.

## Structure:
    philo/                  <- thread simulation
    philo/include           <- header file
    philo/objects           <- object files
    philo/sources           <- source C files
                 /init.c    <- initialization of structs for container and philosophers
                 /main.c    <- main program, check-parse-simulation
                 /mutex.c   <- wrappers to access protected variables
                 /threads.c <- thread functions
                 /tools.c   <- non-specific functions

    philo_bonus/                      <- process simulation
    philo_bonus/include               <- header file
    philo_bonus/objects               <- object files
    philo_bonus/sources               <- source C files
                       /init.c        <- initialization of structs for container and philosophers
                       /main.c        <- main program, check-parse-simulation
                       /processes.c   <- process functions
                       /semaphores.c  <- wrappers to access to protected variables
                       /tools.c       <- non-specific functions


# Disclaimer
- Following the rules of this project, only certain functions were allowed (see specifications in Chapter V/Chapter VI of the PDF project *(see #References##Philosophers))* to manage threads, processes, mutexes, and semaphores, it's natural that there are better implementations of this simulation using all of the tools available; nevertheless this wasn't that case;
- This program greatly relies on the hardware underneath and performances will be affected with big number of philosophers or short timings (or wooden computers).. something to keep in mind:
    - for thread/mutex case not more than 200 philosophers can exist;
    - for process/semaphore case the number of philo/forks also depends on the maximum amount of semaphores the OS can handle, for my use-case it was 125 (TODO PUT_VERSION_MAC_USED);
    - its not recommended to use timings smaller than 60 ms, also the differences should consider 10 ms to be precise (t_death - (t_sleep + t_eat) > 10);
    - in both of the Makefiles the compiling flag for fsanitize is commented, only for debugging reasons should be enabled, because it slows the overall performances.

# References
- 42 project: [Philosophers](https://cdn.intra.42.fr/pdf/pdf/68830/en.subject.pdf)
- By: Francesco Aru ([GithHub](https://github.com/Orpheus-3145)), francesco.aru25@gmail.com, intra42/slack nickname: @faru, Codam, Amsterdam
