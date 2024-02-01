# What is Task Manager?

Task Manager is a little program coded in C with ncurses for the graphic interface.<br>
You can add tasks and subtaks, delete them, validate them and set a deadline.

This is a final project to validate the C course during my second year at IT school.<br>
This is why bugs can occur.


# Building and running

I have only tested the compilation on Linux Debian.

In the src folder, use:
```bash
make
```

To run Task Manager, just type:
```bash
./task-manager
```

# Configuration file

Task Manager has a configuration files that lets you customize some parameters.

You can change the name of the file storing the tasks.

It is possible to change the color of the interface.<br>
Colors available are : **BLACK**, **RED**, **GREEN**, **YELLOW**, **BLUE**, **MAGENTA**.

The date display format can be changed. You can use **%d** for the day, **%m** for the month, **%Y** or **%y** for the year, each separated by **/** or **-**.<br>
The default is the international format (yyyy-mm-dd).

You can enable or disable **deleteIfCompleted** parameter.<br>
If the is true, all completed tasks will be deleted when the program is started.

# Commands

Here are the program commands:
- "a": add a task
- "s": add a subtask to the current task
- "d": delete the current task
- "x" or "space": validate the current task
- "q": quit the program
