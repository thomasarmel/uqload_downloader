#!/bin/bash
########################################################################
# Author: papiche
# Version: 0.1
# License: AGPL-3.0 (https://choosealicense.com/licenses/agpl-3.0/)
########################################################################
# download_from_kodi_log.sh
########################################################################
echo "Extract uqload links from ~/.kodi/temp/kodi.${OLD}log"
# Detects uqload links and ask for copying it to $HOME/astroport
########################################################################
MY_PATH="`dirname \"$0\"`"              # relative
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"  # absolutized and normalized
SCRIPT="${0##*/}"

## CHECKING ENV
[[ ! -d $HOME/astroport ]] && mkdir $HOME/astroport
[[ ! -d $HOME/.local/bin ]] && mkdir -p $HOME/.local/bin
[[ ! $(echo $PATH | grep "$HOME/.local/bin") ]] && export PATH="$PATH:$HOME/.local/bin"

## CHECK BINARY / BUILD and INSTALL
if [[ ! -f $HOME/.local/bin/uqload_downloader ]]; then
	g++ -o uqload_downloader uqload_downloader.cpp Downloader.cpp -lcurl
	[[ -f uqload_downloader ]] && mv uqload_downloader $HOME/.local/bin/ || echo "INSTALL FAILED, 'sudo apt-get install libcurl4-openssl-dev -y' TRY AGAIN." && exit 1
fi

## CHOOSE kodi.${OLD}log
[[ $1 == "old" ]] && OLD='old.' || OLD=''

[[ ! $(which kodi) ]] && echo "KODI IS MISSING. VISIT https://copylaradio.com" && exit 1

## LOOP
cycle=1
for uqlink in $(cat ~/.kodi/temp/kodi.${OLD}log | grep uqload | grep 'play :' | rev | cut -d '/' -f 1 | rev);
do
	uqname=$(cat ~/.kodi/temp/kodi.${OLD}log | grep uqload | grep $uqlink | grep VideoPlayer | cut -d '=' -f 4 | cut -d '&' -f 1 | cut -d '%' -f 1 | sed 's/\+/_/g')
	cycle=$((cycle+1))
	echo "########################################################################"
	echo "MANUAL : uqload_downloader https://uqload.com/$uqlink \"$HOME/astroport/$uqname.mp4\""
	## CHECK & MANAGE COPY
	if [[ $(find $HOME/astroport -name "$uqname.mp4" -type f -print) ]];
	then
		echo "COPY ALREADY IN $HOME/astroport/"
	else
		echo "DETECTED MOVIE : $uqname (https://uqload.com/$uqlink)"
		echo "WANT TO COPY ? Yes? Write any character + enter, else just hit enter."
		read YESNO
		if [[ "$YESNO" != "" ]]; then
			## COPY STREAMING
			uqload_downloader https://uqload.com/$uqlink "$HOME/astroport/$uqname.mp4"
			echo "COPY ~/astroport/$uqname.mp4 DONE"
			## ARE WE ASTROPORT STATION? https://astroport.com 
			[[ "$USER" != "xbian" && ${IPFSNODEID} ]] && ~/.zen/astrXbian/ajouter_video.sh
		else
			continue
		fi
	fi
done
echo 
echo "########################################################################"
[[ $cycle == 1 && ! ${OLD} ]] && echo "NOTHING IN CURRENT LOG, TRY old ?" && read OLD && [[ "$OLD" != "" ]] && $MY_PATH/$SCRIPT old
echo "DONE... VideoClub Datacenter Virtuel entre amis. https://copylaradio.com"
echo "ASTROPORT. Le web des gens."
exit 0
