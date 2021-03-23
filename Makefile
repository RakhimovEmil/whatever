all: check

smoke_test: smoke_test.cpp whatever.hpp
    $(CXX) -g -Wall -Wextra -std=c++17 -o 0 smoke_test.cpp

smoke_test_1: run_assign_whatever_empty.cpp whatever.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o 1 run_assign_whatever_empty.cpp

smoke_test_2: run_reference_change.cpp whatever.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o 2 run_reference_change.cpp

smoke_test_3: run_whatever_cast_nullptr.cpp whatever.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o 3 run_whatever_cast_nullptr.cpp

smoke_test_4: run_array.cpp whatever.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o 4 run_array.cpp

smoke_test_5: run_assign_copy_once.cpp whatever.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o 5 run_assign_copy_once.cpp

check: smoke_test smoke_test_1 smoke_test_2 smoke_test_3 smoke_test_4 smoke_test_5
   ./0
   ./1
   ./2
   ./3
   ./4
   ./5