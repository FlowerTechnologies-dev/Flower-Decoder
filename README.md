# TwOS Bootanimation File System
TwOS Bootanimation file system (.twbf) allows for more efficient and customizable animation of the video file displayed during TwOS boot.

The logic is simple: when opened with a decoder, you see a video consisting of 1 (minimum) to 5 (maximum) sections, depending on the file. Numbers from 0 to 4 determine the order in which each section plays. The loop section, usually in the middle or at the end, is the part that repeats continuously after the previous one, depending on the system's boot process, and the subsequent sections play closer to the system's startup.

**Creator:** *Azeno42* (Muratcan Yücepur)
