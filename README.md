# cpusetcores
This is a utility application for managing CPU cores / threads on Linux machines.
It allows you to enable/disable threads (via reading/writing to `/sys/devices/system/cpu/cpuX/online`).

The plan is to eventually through deamons or some kernel magic to manage the amount of cores automatically, in hopes of saving power.
How well this'll work and how much power is gained from it is still researched and the project is not actively being worked on.
So for now it just serves as something faster than a shell script.

I assume the kernel likely already has things for this, or it is being worked on.
I just haven't looked into it that much yet, since I haven't really started the project yet.
