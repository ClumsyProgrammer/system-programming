#!/bin/bash



function usage
{
    echo "Usage: jms_script.sh -c <command> -l <path>"
}


path=
command=
size=




#http://linuxcommand.org/wss0130.php

while [ "$1" != "" ]; do
    case $1 in
        -c)           	shift
                      	command=$1
		      	shift
		      	size=$1
                      	;;
        -l)    	      	shift
			path=$1
                        ;;
        -h)           	usage
                        exit
                        ;;
        * )             usage
                        exit 1
    esac
    shift
done


if [ "$path" = "" ]; then
	usage
	exit 1
fi



    case $command in
        list)           echo "Folders created by submitted jobs:"			
			find $path -type d -name "std07095*"                      	
			;;
        size)    	echo "sort by size"


			if  [ "$size" = "" ] ; then
				du -h $path/std07095* | sort -nr  
			else

				#http://stackoverflow.com/questions/806906/how-do-i-test-if-a-variable-is-a-number-in-bash
				re='^[0-9]+$'
				if ! [[ $size =~ $re ]] ; then
					usage
					exit 1
				fi
				du -h $path/std07095* | sort -nr | head -$size  
			fi
			;;
        purge)          echo "delete everything"
                        find $path -type d -name "std07095*" -prune -exec rm -rf {} \;
                        ;;
        * )             usage
                        exit 1
    esac










