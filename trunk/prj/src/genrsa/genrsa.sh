if [ 0 == $# ];
then
	L="512";
else
	L="$1";
fi

dd if=/dev/random bs=1024 count=4 of=my-key.ran &> /dev/null
openssl genrsa -out my-key.pem -rand my-key.ran $L &> /dev/null
openssl rsa -in my-key.pem -noout -text

rm -f my-key.pem my-key.ran

