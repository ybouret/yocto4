all:


veryclean:
	@echo "-- removing out of sources builds" && cd forge && touch targets && ( cat targets | xargs rm -rf ) && rm -f targets

gnu:
	@bash ../src/share/create.sh src gnu ${BUILD_TYPE}

clang:
	@bash ../src/share/create.sh src clang ${BUILD_TYPE}

xcode:
	@bash ../src/share/create.sh src xcode ${BUILD_TYPE}
	
vs9:
	@bash ../src/share/create.sh src vs9 ${BUILD_TYPE}

