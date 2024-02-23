

default: TestRun

TestRun: Execute
	./Execute --Input HiForestMiniAOD_314.root --Output Skim.root --MinZPT 40

Execute: ReduceForest.cpp
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
