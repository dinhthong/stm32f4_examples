#   MAIN              
if [ $# != 0 ]; then
	case $1 in
		"template" )
			make all 2>&1 | tee "build_log.txt"
			;;
		"clean" )
			make clean
			;;
		*)
			echo "=== $1 === : Build option is not supported. Please choose another one!"
			#build_help
			;;
	esac
else
	#build_help
	echo "No argument were provided!"
fi