#!/bin/bash


read stdinvals

if [ "$(uname)" == "Darwin" ]; then
	echo "$stdinvals" | ./gccerc-mac
else
	echo "$stdinvals" | ./gccerc
fi
