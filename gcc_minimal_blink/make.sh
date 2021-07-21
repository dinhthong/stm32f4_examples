#   MAIN
show_help() 
{
	echo "This is placeholder for help"
}              
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
			show_help
			;;
	esac
else
	echo "No argument were provided!"
	show_help
fi