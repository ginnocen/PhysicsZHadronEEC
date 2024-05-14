./finalAnalysis.sh output/PbPb0_30 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/HISingleMuon-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/PbPb0_30 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/HISingleMuon-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/PbPb0_30 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/HISingleMuon-v6.root  --IsPP false --IsGenZ false --nMix 10

hadd -f plots/PbPb0_30-result.root output/PbPb0_30-1_2-result.root output/PbPb0_30-2_4-result.root output/PbPb0_30-4_10-result.root

./finalAnalysis.sh output/PbPb30_50 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/HISingleMuon-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/PbPb30_50 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/HISingleMuon-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/PbPb30_50 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/HISingleMuon-v6.root  --IsPP false --IsGenZ false --nMix 10

hadd -f plots/PbPb30_50-result.root output/PbPb30_50-1_2-result.root output/PbPb30_50-2_4-result.root output/PbPb30_50-4_10-result.root




