if [ 1 != $# ];
then
	echo "Need a parse-rsa!";
	exit 1;
fi

for L in 128 256 512 1024;
do
	for iter in 1 2 3 4;
	do
		sh ./genrsa.sh $L | $1
	done
done
cat rsa-key-*.bin > rsa-keys.bin
rm -f rsa-key-*.bin
