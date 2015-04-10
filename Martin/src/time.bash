START=$(date +%s)
bach.exe > dump.txt
END=$(date +%s)
DIFF=$(( $END - $START ))
echo "It took $DIFF seconds"
