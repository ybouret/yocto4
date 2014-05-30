for r in 01 02 03 04 10 14 15 16 17;
do
f="$r.dat";
echo $f;
if [ ! -f $f  ]
then
	echo "$f does not exist"
	wget http://paulbourke.net/texture_colour/colourspace/$f -O $f 
fi
g="$r.gif";
if [ ! -f $g  ]
then
        echo "$g does not exist"
        wget http://paulbourke.net/texture_colour/colourspace/$g -O $g
fi
done

