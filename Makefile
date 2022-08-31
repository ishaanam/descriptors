parse_descriptor : main.o parse_descriptor.o descriptor.o sort_keys.o tokenizer.o script_functions.o
	g++ -std=c++17 -o parse_descriptor main.o parse_descriptor.o descriptor.o sort_keys.o tokenizer.o script_functions.o

main.o : main.cpp parse_descriptor.h
	g++ -std=c++17 -c main.cpp

parse_descriptor.o : parse_descriptor.cpp tokenizer.h descriptor.h
	g++ -std=c++17 -c parse_descriptor.cpp

script_functions.o : script_functions.cpp script_functions.h
	g++ -std=c++17 -c script_functions.cpp

descriptor.o : descriptor.cpp descriptor.h script_functions.h
	g++ -std=c++17 -c descriptor.cpp

sort_keys.o : sort_keys.cpp sort_keys.h
	g++ -std=c++17 -c sort_keys.cpp

tokenizer.o : tokenizer.cpp tokenizer.h
	g++ -std=c++17 -c tokenizer.cpp
