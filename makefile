all:

clean:
	@echo "-- removing sdk" && rm -Rf sdk
	@echo "-- removing MacOSX Finder info" && find . -name '.DS_Store' | xargs rm -f

veryclean: clean
	@echo "-- removing out of sources builds" && cd forge && touch targets &&  ( ( cat targets | xargs rm -rf ) && rm -f targets )
	@${MAKE} -s -C prj veryclean

xcode:
	@bash ./src/share/create.sh src xcode ${BUILD_TYPE} 
	
vs9:
	@bash ./src/share/create.sh src vs9 ${BUILD_TYPE}

gnu:
	@bash ./src/share/create.sh src gnu ${BUILD_TYPE}

clang:
	@bash ./src/share/create.sh src clang ${BUILD_TYPE}

intel:
	@bash ./src/share/create.sh src intel ${BUILD_TYPE}
