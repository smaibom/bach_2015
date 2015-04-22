START=$(date +%s)
a.exe > a.txt
END=$(date +%s)
DIFF=$(( $END - $START ))
echo "It took $DIFF seconds"
