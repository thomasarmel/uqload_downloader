#!/bin/bash
########################################################################
# Author: papiche
# Version: 0.1
# License: AGPL-3.0 (https://choosealicense.com/licenses/agpl-3.0/)
########################################################################
MY_PATH="`dirname \"$0\"`"              # relative
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"  # absolutized and normalized
SCRIPT="${0##*/}"

[[ ! -f $HOME/.local/bin/uqload_downloader ]] && echo "g++ -o uqload_downloader uqload_downloader.cpp Downloader.cpp -lcurl && mv uqload_downloader $HOME/.local/bin/" 

[[ $1 == "old" ]] && OLD='old.' || OLD=''

cycle=1
for uqlink in $(cat ~/.kodi/temp/kodi.${OLD}log | grep uqload | grep 'play :' | rev | cut -d '/' -f 1 | rev);
do
	uqname=$(cat ~/.kodi/temp/kodi.${OLD}log | grep uqload | grep $uqlink | grep VideoPlayer | cut -d '=' -f 4 | cut -d '&' -f 1 | cut -d '%' -f 1 | sed 's/\+/_/g')
	cycle=$((cycle+1))
	echo "uqload_downloader https://uqload.com/$uqlink \"$uqname.mp4\""
done
