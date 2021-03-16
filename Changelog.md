1.0.0
5/9/2020

Initial release

1.0.1
5/18/2020

Moved some definitions out of .h files so that SchedTask.h can be used in .cpp files.  Otherwise duplicate definitions occur.

1.0.2
7/4/2020

Corrected SchedBase.cpp which was not handling setNext(NEVER) correctly.

1.0.3
7/7/2020

Changed the dispatcher to fix exposure to millis() overflow at 49 days.
Previously the library would fail at that point.

Now it should work provided any task scheduled in the future is less than about 24 days hence.

1.0.4
8/18/2020

The IDE Library Manager was not showing the latest version number.
Corrected library.properties and set the Tag to 1.0.4.

1.0.5
8/20/202013

Use linked list rather than vector for task list.
Added support for task iterations to limit dispatches.
Added Example_10 and Example_11 to illustrate use of iterations.
Expanded documentation in comments in SchedTask.h and SchedTastT.h.

1.1.0
10/01/2020

Changes to support other platforms.  The library should now compile for
ESP32 and ESP8266 platforms though they have not been tested.
Member functions setFunc and getFunc for SchedTaskT are now deprecated
and replaced by setFuncT and getFuncT. See README.md or comments in the .h files.

1.1.1
10/13/2020

Minor changes to eliminate some warnings/errors when compiling with PlatformIO.

1.1.2
02/01/202013

Correct description comment in SchedTaskT.h.
Moved construction and destructor definitions from SchedTask.h to new file SchedTask.cpp.
This will allow a module to compile and support SchedTask and SchedTaskT.

1.1.3
3/15/2021

Changed default constructors.

