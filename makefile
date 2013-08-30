CREATE=./src/share/create.sh

all:

clean:
	@echo "-- removing sdk" && rm -Rf sdk
	@echo "-- removing temporary files" && rm -f *.dat *.ppm *.vtk *.png *.dot *.bin *.curve *.silo *.raw
	@echo "-- cleaning auxiliary files"
	@${MAKE} -s -C samples             clean
	@${MAKE} -s -C src/y-aqueous/docs  clean
	@${MAKE} -s -C src/y-mk/docs       clean
	@${MAKE} -s -C src/y-chemical/docs clean
	@${MAKE} -s -C src/y-gems/docs     clean
	@echo "-- removing MacOSX Finder info" && find . -name '.DS_Store' | xargs rm -f


preclean:
	@echo "-- removing out of sources builds" && cd forge && touch targets &&  ( ( cat targets | xargs rm -rf ) && rm -f targets )

veryclean: preclean clean
	@${MAKE} -s -C prj veryclean

xcode:
	@bash ${CREATE} src xcode ${BUILD_TYPE} 
	
vs9:
	@bash ${CREATE} src vs9 ${BUILD_TYPE}

vs10:
	@bash ${CREATE} src vs10 ${BUILD_TYPE}

gnu:
	@bash ${CREATE} src gnu ${BUILD_TYPE}

clang:
	@bash ${CREATE} src clang ${BUILD_TYPE}

intel:
	@bash ${CREATE} src intel ${BUILD_TYPE}

codeblocks:
	@bash ${CREATE} src codeblocks ${BUILD_TYPE}

pathscale:
	@bash ${CREATE} src pathscale ${BUILD_TYPE}

	
