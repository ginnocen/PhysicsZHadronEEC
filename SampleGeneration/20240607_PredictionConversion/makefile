
TestRun: ExecuteHepMC ExecutePU14 ExecuteHybrid
	# ./ExecutePU14 --Input Input/pyquen/output/PbPbWide_0_30ZJet_14.pu14 --Output Meow.root
	# ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJetPbPb/0-30/out_ZJet_PbPb_14.hepmc \
	# 	--Output Meow.root --NegativeID 3
	./ExecuteHybrid --Input Input/sample/dijet80_q.root \
		--Output Meow.root --NegativeID 2

ExecutePU14: ConvertPU14.cpp Helper.h
	g++ ConvertPU14.cpp -o ExecutePU14 \
		`root-config --cflags --libs` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecuteHepMC: ConvertHepMC.cpp Helper.h
	g++ ConvertHepMC.cpp -o ExecuteHepMC \
		`root-config --cflags --libs` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecuteHybrid: ConvertHybrid.cpp Helper.h
	g++ ConvertHybrid.cpp -o ExecuteHybrid \
		`root-config --cflags --libs` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/Messenger.o

RunAll: ExecuteHepMC ExecutePU14 ExecuteHybrid
	bash RunAll.sh
