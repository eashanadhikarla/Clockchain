# /*
# ** Name: Eashan Adhikarla
# ** Project: ClockChain
# ** Class: CSE411 Advanced Programming Techniques
# */


#!/bin/bash
# Runs client and server using test inputs, and logs stdout/stderr.

# Log file settings
LOG_DIR=logs
SERVER_LOG=$LOG_DIR/server.out
SERVER_ERR=$LOG_DIR/server.err

CLIENT_LOG=$LOG_DIR/client.out
CLIENT_ERR=$LOG_DIR/client.err
CLIENT_INP=client.inp

# run settings
PORT=4000
HOST=localhost

# remove all log files that haven't been removed already
LOGFILES=( $SERVER_LOG $SERVER_ERR $CLIENT_LOG $CLIENT_ERR )
for LOGFILE in ${LOGFILES[@]}; do
    if [ -e $LOGFILE ]; then
        rm $LOGFILE
    fi
done

# Runs the commands
obj64/server.exe > $SERVER_LOG 2> $SERVER_ERR &
sleep 1

obj64/client.exe $HOST $PORT < $CLIENT_INP > $CLIENT_LOG 2> $CLIENT_ERR
sleep 1

## kill all pid of processes with a given name (for systems where `killall`
## isn't available
ps
kill $(ps -o pid=,comm= | grep server | awk '{print $1}')

## kill jobs managed by this shell with a given name; same as above but
## kills by job ID instead of process ID
#kill %$(jobs | grep server | sed "s/[^0-9]*//g")

FILES_TO_CHECK=( $SERVER_LOG $SERVER_ERR $CLIENT_LOG $CLIENT_ERR )
for filename in $FILES_TO_CHECK; do
    filesize=$(wc -c $filename | sed -E "s/^[^0-9]*([0-9]+)[^0-9]*/\1/")
    if [ "$filesize" -ne 0 ]; then
        echo $filename:
        cat $filename | while read line; do
            echo -e "\t"$line
        done

        echo
        echo "========"
        echo
    fi
done

echo Done
