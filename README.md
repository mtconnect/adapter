Introduction
These instructions are specific to the fanuc adapter on the linux compatible branch of my copy of the mtconnect/adapter repository

Installation

to install on a x64 system, add the libc6-dev-i386 library and multilib for g++.
apt-get install libc6-dev-i386
sudo apt-get install g++-multilib

To build:

git clone https://github.com/mmxe/adapter.git
git checkout linux-compat
cd adapter
cmake .
sudo make install

Install process doesn't copy config file. Copy it from adapter/fanuc/adapter.ini into /etc/mtconnect/adapter/ and edit to your liking.

Run adapter_fanuc /etc/mtconnect/adapter/adapter.ini

My install responds like this:
2016-09-15T17:11:13.347944Z - Info: Arguments: 1
2016-09-15T17:11:13.348857Z - Info: Ini File: adapter/fanuc/adapter.ini
2016-09-15T17:11:13.351756Z - Info: Showing hidden axis.
2016-09-15T17:11:13.352613Z - Info: Adding sample macro 'gauge1' at location 500
2016-09-15T17:11:13.354319Z - Info: Adding pmc 'Fovr' at location -12
2016-09-15T17:11:13.355044Z - Info: Adding pmc 'r100' at location 50500
2016-09-15T17:11:13.355729Z - Info: Adding parameter 'iochannel' at location 20
2016-09-15T17:11:13.355994Z - Info: Adding parameter 'cuttime' at location 6754
2016-09-15T17:11:13.356081Z - Info: Adding parameter 'powerontime' at location 6750
2016-09-15T17:11:13.356496Z - Info: Adding diagnostic 'XposError' at location 15202
2016-09-15T17:11:13.356558Z - Info: Adding diagnostic 'DCLink' at location 10752
2016-09-15T17:11:13.356643Z - Info: Adding alarm 'alarm1' at location 1
2016-09-15T17:11:13.356721Z - Info: Adding alarm 'alarm2' at location 2
2016-09-15T17:11:13.356923Z - Info: Adding alarm 'alarm3' at location 3
2016-09-15T17:11:13.356969Z - Info: Adding alarm 'alarm4' at location 4
2016-09-15T17:11:13.357281Z - Info: Adding critical 'critical1' at location 0
2016-09-15T17:11:13.357366Z - Info: Adding critical 'critical2' at location 0
2016-09-15T17:11:13.357670Z - Info: Adding critical 'critical3' at location 0
2016-09-15T17:11:13.357741Z - Info: Adding critical 'critical4' at location 0
2016-09-15T17:11:13.357820Z - Info: Server started, waiting on port 7878

Notice it's listening on Port 7878 for an agent to make a request. It doesn't contact an actual Fanuc controller until an agent makes a request.

Next steps:
Get it do daemonize properly, and to be able to run either multiple instances of the program on different ports (without colliding log files as will happen now) OR to fork multiple processes based on how many hosts and ports are defined in the config file.
