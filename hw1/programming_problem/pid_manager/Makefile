all:
	mkdir -p ./output
	g++ -std=c++14 -isystem /usr/include/gtest -pthread pid_manager.cpp test_manager.cpp -o ./output/test_pid_manager -lgtest -lgtest_main
	./output/test_pid_manager

clean:
	rm -rf ./output

install_gtest:
	@sudo apt-get install libgtest-dev