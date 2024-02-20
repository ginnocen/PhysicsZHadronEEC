Here we put ZHadron EEC code

We try to compartmentalize things, each piece of work will be in one subdirectory

For example

- SamplePreparation/
   - 20240220_Pythia8Generation/
   - 20240225_PyquenGeneration/
   - ...
- MCStudies/
   - 20240220_FirstExploration/
   - ...
- MainAnalysis/
   - ...

Each subdirectory will be standalone and does one thing very well.  If we need to rewrite significant portion of things we make a new folder for the same thing.  CommonCode will have things everyone needs, like correction tables, common header files, etc.

