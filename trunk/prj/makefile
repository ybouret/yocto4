all:


veryclean:
	@echo "-- removing out of sources builds" && cd forge && touch targets && ( cat targets | xargs rm -rf ) && rm -f targets

xcode:
	@bash ../src/share/create.sh src xcode
	
vs9:
	@bash ../src/share/create.sh src vs9

