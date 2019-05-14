 list="$(find .. -name '*.s')"

for f in $list; do
    # do some stuff here with "$f"
    # remember to quote it or spaces may misbehave
	echo "$f"
	../ressources/asm "$f"
	mv "${f%.*}.cor" their
	../asm "$f" 
	mv "${f%.*}.cor" our
	diff our their
done