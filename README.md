# Philosophers
Implementation of the classic Dining Philosophers Problem, this abstraction helps to get used to concurrent programming, i.e. multiple entities reading/writing one or more shared resources.


# Overview
The initial situation can be described as follows: there are n philosophers sitting on a circular table and on their left and right there's a fork, so on the table there are n philosophers and n forks, and a lot of meals to eat that stimulate philosphy. 
Every philosopher can do only three (mutual exclusive) actions: eat, sleep or think: after the philo ate, they sleep, then they think waiting the availability of the forks on their left and right, so the cycle can start again. The catch is that a philosopher needs two forks to eat, once they're done they can let go the forks: as a consequence at most n/2 philosophers are eating at the same time. Last but not least, a philosopher dies if too much time passes since their last meal (i.e. the philosophers cannot think forever to have access to the forks).


# The simulation
## Intro
The designed solution imitates the behaviour of every philosopher: when it has access to two forks/resources it stops the execution for x millisecs, after that leaves the forks and stops again for y millisecs, at the end it waits to use the forks again; the simulation is interrupted when and only when one of this two events occurs:
- A philosopher dies
- The last philosopher ate enough times (optional parameter, see next)
When an action is performed (eat, grab a fork, die, ...) a message is printed on stdout with the format:
    - [time_stamp] -- [id_philosopher] [action]

Furthermore, the solution was implemented with these constraints:
- 1) the philosophers shouldn't die                   --> the code must be optimized, so the time losses are reducted as much as possible
- 2) messages must be fast                            --> not more than 10 millisecs must pass between the death of a philo and the printing of the message on stdout
- 3) the philosophers do not talk with each other     --> the threads/proceses run by themselves: they cannot notify other philosophers a change of their status, or anything else

## Parameters:
The program takes some parameteres to set the environments (NB: the last parameter is optional!)
- 1) number of existing philosophers  (NB: a positive number is required)
- 2) number of milliseconds that phil can stay alive without eating   (NB: a non-negative number is required)
- 3) number of milliseconds that phil needs to eat    (NB: a non-negative number is required)
- 4) number of milliseconds that phil needs to sleep  (NB: a non-negative number is required)
- 5) number of times the philosopher has to eat, optional (NB: a positive number is required)

## Approaches
There are two solutions to this problem, one with threads and mutexes and the other with processes and semaphores.
- The first one (folder ```philo```) works exactly as the problem is exposed



- By: Francesco Aru ([GithHub](https://github.com/Orpheus-3145)), francesco.aru25@gmail.com, intra42/slack nickname: @faru, Codam, Amsterdam
