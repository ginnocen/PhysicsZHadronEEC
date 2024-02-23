default: ExecuteZHadron

ExecuteZHadron: ZHadron.cpp
	g++ ZHadron.cpp -I. -I$(ProjectBase)/CommonCode/include \
		-o ExecuteZHadron `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteZHadron
	./ExecuteZHadron --Input XXX1.root,XXX2.root --Output output.root