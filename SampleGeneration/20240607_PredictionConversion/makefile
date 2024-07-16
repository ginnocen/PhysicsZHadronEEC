
TestRun: ExecuteHepMC ExecutePU14 ExecuteHybrid ExecuteHepMC3
	./ExecutePU14 --Input Input/pyquen/output/PbPbWide_0_30ZJet_14.pu14 --Output Meow1.root
	./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJetPbPb/0-30/out_ZJet_PbPb_14.hepmc \
		--Output Meow2.root --NegativeID 3
	./ExecuteHybrid --Input Input/sample/zjet10_q.root \
		--Output Meow3.root --NegativeID 2
	./ExecuteHepMC3 --Input  /data/yjlee/jetscapeGuu.hepmc \
		--Output Meow4.root

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

ExecuteHepMC3: ConvertHepMC3.cpp Helper.h
	g++ ConvertHepMC3.cpp -o ExecuteHepMC3 \
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
