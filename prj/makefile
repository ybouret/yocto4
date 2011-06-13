all:


veryclean:
	@echo "-- removing out of sources builds" && cd forge && test -f targets && ( cat targets | xargs rm -rf ) && rm -f targets

xcode:
	@bash ../src/share/create.sh src xcode
	
vs9:
	@bash ../src/share/create.sh src vs9

