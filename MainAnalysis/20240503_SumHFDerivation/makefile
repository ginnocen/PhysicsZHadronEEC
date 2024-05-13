
default: TestRun

TestRun: Execute
	./Execute --Input DYLLMC_Gen.root

Execute: DeriveWeight.cpp
	g++ DeriveWeight.cpp -o Execute \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/Messenger.o \
		-I$(ProjectBase)/CommonCode/include


