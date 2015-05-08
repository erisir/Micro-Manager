package org.ndaguan.micromanager.mmtracker;
import ij.IJ;
import ij.WindowManager;

import mmcorej.TaggedImage;

import org.micromanager.MMStudio;
import org.micromanager.acquisition.TaggedImageQueue;
import org.micromanager.api.TaggedImageAnalyzer;

public class TestAnalyzer extends TaggedImageAnalyzer {

	private static TestAnalyzer instance_;
	private Kernel kernel_;

	public static TestAnalyzer getInstance() {	
		return instance_;
	}
	public static TestAnalyzer getInstance(Kernel kernel) {
		if(instance_ == null)
			instance_ = new TestAnalyzer(kernel);
		return instance_;
	}

	public TestAnalyzer(Kernel kernel) {
		kernel_ = kernel;
	}

	@Override
	protected void analyze(TaggedImage taggedImage) {

		if (!MMT.isTestingRunning_ || taggedImage == null || taggedImage == TaggedImageQueue.POISON)return;
		//Testing start
		try {
			double[] pos = Function.getInstance().getStagePosition();//new double[]{kernel_.xPosProfiles[MMT.calibrateIndex_],kernel_.yPosProfiles[MMT.calibrateIndex_],kernel_.zPosProfiles[MMT.calibrateIndex_]};
			boolean ret = kernel_.getXYZPosition(taggedImage.pix);
			
			Function.getInstance().reDraw( WindowManager.getCurrentImage(),MMT.testingIndex_, true,true);
			if(!ret){
				MMT.logError("Testting Error");
				MMT.isTestingRunning_ = false;
				MMT.isAnalyzerBusy_ = false;
				return;
			}
			Function.getInstance().updateTestingChart( pos[2]);//kernel_.zTestingPosProfiles[MMT.testingIndex_]);
			MMT.logMessage(String.format("Testing:\t\t%d/%d",MMT.testingIndex_,kernel_.zTestingPosProfiles.length));
		} catch (Exception e) {
			MMT.isTestingRunning_  = false;
			MMT.lastError_ = e.toString();
		}
		MMT.isAnalyzerBusy_ = false;
		//Testing end
	}
}