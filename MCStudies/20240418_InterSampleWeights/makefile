
default: TestRun Deploy

TestRun: Execute
	./Execute --AA Input/DYLLMC_Gen.root --PP Input/PPMC_Gen.root

Execute: DeriveWeight.cpp
	g++ DeriveWeight.cpp -o Execute \
		`root-config --cflags --libs` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/Messenger.o

Deploy:
	cp *pdf ~/Dropbox/Temp/
