////////////////////////////////////////////////////////////////////////
//   A simple example of finding 2D track from the detected positron  //
// hit position. I hope it helpful.                                   //
//                                                                    //
//               - 2017. 8. 2. Hoyong Jeong (hyjeong@hep.korea.ac.kr) //
////////////////////////////////////////////////////////////////////////

void track()
{
	gROOT -> Reset();

	// Read file
	TChain* chain = new TChain("tree");
	chain -> Add("BFieldWithGDML.root");

	// Get branches
	int eventID, trackID, parID;
	double x, y, z;
	chain -> SetBranchAddress("eventID", &eventID);
	chain -> SetBranchAddress("trackID", &trackID);
	chain -> SetBranchAddress(  "parID",   &parID);
	chain -> SetBranchAddress(   "posX",       &x);
	chain -> SetBranchAddress(   "posY",       &y);
	chain -> SetBranchAddress(   "posZ",       &z);

	// Define TGraph
	TGraph* gr = new TGraph();

	// Fill points into the TGraph object
	for ( int i = 0; i < chain -> GetEntries(); i++ )
	{
		chain -> GetEntry(i);
		gr -> SetPoint(i, x, y);
	}

	// Function to be minimized
	auto chi2Function = [&](const Double_t *par)
	{
		// Minimization function computing the sum of squares of residuals
		// looping at the graph points
		int np = gr -> GetN();
		double f = 0;
		double *x = gr -> GetX();
		double *y = gr -> GetY();

		for ( int i = 0; i < np; i++ )
		{
			double u  = x[i] - par[0];
			double v  = y[i] - par[1];
			double dr = par[2] - TMath::Sqrt(u*u + v*v);
			f += dr*dr;
		}

		return f;
	};

	// Wrap chi-square funciton in a function object for the fit
	// 3 is the number of fit parameters (size of array par)
	ROOT::Math::Functor fcn(chi2Function, 3);
	ROOT::Fit::Fitter fitter;

	double pStart[3] = {-150, -150, 150}; // I decided it by my eyes.
	fitter.SetFCN(fcn, pStart);
	fitter.Config().ParSettings(0).SetName("x0");
	fitter.Config().ParSettings(1).SetName("y0");
	fitter.Config().ParSettings(2).SetName("R");

	// Do the fit 
	bool ok = fitter.FitFCN();
	if (!ok) Error("line3Dfit","Line3D Fit failed");

	const ROOT::Fit::FitResult & result = fitter.Result();
	result.Print(std::cout);

	// Draw
	TCanvas* c = new TCanvas("c", "Track", 0, 0, 900, 900);
	c -> SetGrid();
	gr -> Draw("AP");

 	TArc *arc = new TArc(result.Parameter(0), result.Parameter(1), result.Parameter(2));
	arc -> SetLineColor(kBlue);
	arc -> SetLineWidth(2);
	arc -> SetFillStyle(4000); // Don't fill. Make it transparent.
	arc -> Draw("same");

	// Style
	gr -> SetMarkerStyle(20);
	gr -> SetMarkerColor(kRed);
	gr -> GetXaxis() -> SetLimits(-300, 300);
	gr -> GetYaxis() -> SetRangeUser(-300, 300);
	gr -> GetHistogram() -> GetXaxis() -> SetTitle("x position (mm)");
	gr -> GetHistogram() -> GetYaxis() -> SetTitle("y position (mm)");
}
