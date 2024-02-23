int findIndexOfLargestValue(vector<double>* arr) {
    int indexOfLargestValue = 0;
    for (int i = 1; i < arr->size(); i++) {
        if ((*arr)[i] > (*arr)[indexOfLargestValue]) {
            indexOfLargestValue = i;
        }
    }
    return indexOfLargestValue;
}

double PhiRangeSymmetric(double Phi)
{
   if(Phi < -M_PI)   Phi = Phi + 2 * M_PI;
   if(Phi > +M_PI)   Phi = Phi - 2 * M_PI;
   return Phi;
}

double DeltaPhi(double Phi1, double Phi2)
{
   return PhiRangeSymmetric(Phi1 - Phi2);
}


void smartWrite(TH1* h) {
   if (h!=0) {
      cout <<"write "<<h->GetName()<<endl;
      h->Write();
   } 
}


class HistogramRangeChecker {
private:
    double minValue;
    double maxValue;

public:
    HistogramRangeChecker() : minValue(0.0), maxValue(0.0) {}

    void checkHistogramRange(TH1D* histogram) {
        if (histogram == nullptr) {
            // Handle null histogram pointer
            return;
        }

        int numBins = histogram->GetNbinsX();

        if (numBins == 0) {
            // Handle empty histogram
            return;
        }

        double currentMinValue = histogram->GetBinContent(1);
        double currentMaxValue = histogram->GetBinContent(1);

        for (int i = 2; i <= numBins; ++i) {
            double binContent = histogram->GetBinContent(i);
            if (binContent < currentMinValue) {
                currentMinValue = binContent;
            }
            if (binContent > currentMaxValue) {
                currentMaxValue = binContent;
            }
        }

        if (histogram == nullptr || numBins == 0) {
            // If the histogram is null or empty, no need to update the range
            return;
        }

        if (histogram == nullptr || numBins == 0 || currentMinValue < minValue) {
            minValue = currentMinValue;
        }
        if (histogram == nullptr || numBins == 0 || currentMaxValue > maxValue) {
            maxValue = currentMaxValue;
        }
    }

    double getMinValue() const {
        return minValue;
    }

    double getMaxValue() const {
        return maxValue;
    }
};


