---
name: philosophers-guide
description: Specific guidance for the Dining Philosophers (philo) project. Use when working on philosophers, debugging threading, synchronization, or race conditions.
---

# Philosophers Project Guide

Complete guide for the Dining Philosophers problem at 42 School.

## Program Usage

```bash
./philo num_philos time_to_die time_to_eat time_to_sleep [must_eat_count]
```

**Parameters:**
- `num_philos`: Number of philosophers (and forks)
- `time_to_die`: Milliseconds until death without eating
- `time_to_eat`: Milliseconds spent eating
- `time_to_sleep`: Milliseconds spent sleeping
- `must_eat_count`: (Optional) Stop when all eat this many times

**Examples:**
```bash
./philo 5 800 200 200      # 5 philos, should not die
./philo 4 410 200 200      # 4 philos, tight timing
./philo 5 800 200 200 7    # Stop after 7 meals each
./philo 1 400 200 200      # 1 philo (should die)
```

## Core Data Structures

```c
typedef struct s_philo
{
    int             id;              // Philosopher ID (1-N)
    int             meals_eaten;     // Meal counter
    long            last_meal_time;  // Last meal timestamp
    pthread_t       thread;          // Thread handle
    pthread_mutex_t *left_fork;      // Left fork mutex
    pthread_mutex_t *right_fork;     // Right fork mutex
    t_data          *data;           // Shared data pointer
}   t_philo;

typedef struct s_data
{
    int             num_philos;      // Number of philosophers
    int             time_to_die;     // Death timer (ms)
    int             time_to_eat;     // Eating duration (ms)
    int             time_to_sleep;   // Sleeping duration (ms)
    int             num_must_eat;    // Required meals (-1 = infinite)
    long            start_time;      // Simulation start
    bool            someone_died;    // Death flag
    bool            all_ate_enough;  // Completion flag
    pthread_mutex_t print_mutex;     // Print protection
    pthread_mutex_t death_mutex;     // Death flag protection
    pthread_mutex_t meal_mutex;      // Meal data protection
    pthread_mutex_t *forks;          // Fork mutexes array
    t_philo         *philos;         // Philosophers array
}   t_data;
```

## Thread Architecture

### Main Thread
1. Parse and validate arguments
2. Initialize data structure and mutexes
3. Create philosopher threads
4. Create monitor thread
5. Join all threads (wait for completion)
6. Cleanup resources

### Philosopher Thread
```c
void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (!check_death(philo->data))
    {
        philo_eat(philo);        // Grab forks, eat, release
        philo_sleep(philo);      // Sleep
        philo_think(philo);      // Think
    }
    return (NULL);
}
```

### Monitor Thread
```c
void *monitor_routine(void *arg)
{
    t_data *data = (t_data *)arg;

    while (1)
    {
        if (check_death(data))        // Check all philos for death
            break;
        if (check_all_ate(data))      // Check meal completion
            break;
        usleep(1000);                 // Small delay
    }
    return (NULL);
}
```

## Critical Synchronization Points

### Fork Acquisition (Deadlock Prevention)
```c
// Even/Odd strategy to prevent deadlock
void grab_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)  // Even: left first
    {
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
    }
    else  // Odd: right first
    {
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
    }
}
```

### Protected Printing
```c
void print_status(t_philo *philo, char *status)
{
    pthread_mutex_lock(&philo->data->print_mutex);
    
    if (!check_death(philo->data))  // Don't print after death
    {
        long timestamp = get_time() - philo->data->start_time;
        printf("%ld %d %s\n", timestamp, philo->id, status);
    }
    
    pthread_mutex_unlock(&philo->data->print_mutex);
}
```

### Death Check (Race Prevention)
```c
bool check_death(t_data *data)
{
    bool result;

    pthread_mutex_lock(&data->death_mutex);
    result = data->someone_died;
    pthread_mutex_unlock(&data->death_mutex);
    
    return (result);
}
```

### Meal Time Update
```c
void update_meal_time(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->meal_mutex);
    philo->last_meal_time = get_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->meal_mutex);
}
```

## Common Issues & Solutions

### Data Races
**Problem:** Multiple threads access shared variables without protection  
**Solution:** Protect ALL shared data with mutexes:
- `someone_died` → `death_mutex`
- `last_meal_time`, `meals_eaten` → `meal_mutex`
- Console output → `print_mutex`

### Deadlock
**Problem:** All philosophers grab left fork, wait for right forever  
**Solution:** Use even/odd fork grabbing order or resource hierarchy

### Delayed Death Detection
**Problem:** Philosopher dies but not detected immediately  
**Solution:** Monitor thread checks frequently (every 1ms), uses proper locks

### Print After Death
**Problem:** Messages printed after simulation should end  
**Solution:** Check death flag before every print, inside print_mutex

### Imprecise Timing
**Problem:** usleep() oversleeps, philos die unexpectedly  
**Solution:** Custom precise_sleep() that checks stop conditions

## Precise Sleep Implementation

```c
void precise_sleep(long milliseconds, t_data *data)
{
    long start;
    long current;

    start = get_time();
    while (1)
    {
        current = get_time();
        if (current - start >= milliseconds)
            break;
        if (check_death(data))  // Check if should stop
            break;
        usleep(500);  // Sleep in small increments
    }
}
```

## Time Functions

```c
long get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
```

## Testing Checklist

- [ ] No philosopher dies (normal cases)
- [ ] Death detected correctly (tight timing)
- [ ] One philosopher case handled
- [ ] Meal counting works (optional argument)
- [ ] No data races (run with `-fsanitize=thread`)
- [ ] No memory leaks (check with valgrind)
- [ ] No deadlock (philosophers keep running)
- [ ] Output format correct: `timestamp_ms id status`
- [ ] No output after death detected
- [ ] Large numbers work (200 philosophers)

## Compilation Flags

```bash
# Development
cc -Wall -Wextra -Werror -pthread -g src/*.c -I include -o philo

# Thread Sanitizer (detect races)
cc -g -fsanitize=thread -pthread src/*.c -I include -o philo

# Memory check
valgrind --leak-check=full ./philo 5 800 200 200 7
```
