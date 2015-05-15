package org.ndaguan.micromanager.mmtracker;
import ij.WindowManager;
import ij.gui.ImageWindow;

import java.io.IOException;
import java.util.List;

import mmcorej.TaggedImage;

import org.json.JSONException;
import org.micromanager.acquisition.TaggedImageQueue;
import org.micromanager.api.TaggedImageAnalyzer;
import org.micromanager.utils.MDUtils;
import org.micromanager.utils.MMScriptException;

public class GetXYZPositionAnalyzer extends TaggedImageAnalyzer {

	private static GetXYZPositionAnalyzer instance_;
	private long frameNum_;

	private Listener listener_;
	private double elapsed = 0;
	public int bitDepth_;
	public double imgwidth_;
	public double imgheight_;
	private Kernel kernel_;

	public static GetXYZPositionAnalyzer getInstance() {	
		return instance_;
	}
	public static GetXYZPositionAnalyzer getInstance(Kernel kernel, Listener listener,
			List<RoiItem> roiList_, OverlayRender render) {
		if(instance_ == null)
			instance_ = new GetXYZPositionAnalyzer(kernel,listener,roiList_,render);
		return instance_;
	}

	public GetXYZPositionAnalyzer(Kernel kernel, Listener listener,List<RoiItem> roiList,OverlayRender render) {
		listener_ = listener;
		kernel_ = kernel;
		frameNum_ = 0;
	}

	@Override
	protected void analyze(TaggedImage taggedImage) {

		if (taggedImage == null || taggedImage == TaggedImageQueue.POISON)
		{
			Function.getInstance().dataReset();
			frameNum_ = 0;
			elapsed = 0;
			return;
		}

		try {
			elapsed = taggedImage.tags.getDouble("ElapsedTime-ms");
		} catch (JSONException e1) {
			e1.printStackTrace();
		}
		try {
			ImageWindow win = ij.WindowManager.getCurrentWindow();
			if(!listener_.isRunning()){
				Function.getInstance().dataReset();
				listener_.start(win);
				String[] acqnames =  MMTracker.getInstance().getMMJ().getAcquisitionNames();
				MMT.AcqName = acqnames[acqnames.length-1];
				frameNum_ = 0;
				kernel_.imageHeight = Integer.parseInt(taggedImage.tags
						.get("Height").toString());
				kernel_.imageWidth = Integer.parseInt(taggedImage.tags.get(
						"Width").toString());
			}
			if(MMT.AcqName.equals("Snap/Live Window"))
				frameNum_++;
			else
				frameNum_ = MDUtils.getFrameIndex(taggedImage.tags);
			synchronized(MMT.Acqlock){
				if(kernel_.roiList_.size()<=0){
					Function.getInstance().reDraw( WindowManager.getCurrentImage(), frameNum_, true,true);
					return;
				}
				if(!kernel_.getXYZPosition(taggedImage.pix))return;
				if(MMT.isFeedbackRunning_ && frameNum_ % MMT.VariablesNUPD.frameToFeedBack.value() == 0){
					Function.getInstance().doFeedback();
				}
			}//lock
			if(MMT.VariablesNUPD.saveFile.value() == 1 && kernel_.isCalibrated_)
				try {
					kernel_.saveRoiData("Acq",frameNum_,elapsed);
				} catch (IOException e) {
					MMT.logError("Save data error");
				}
			Function.getInstance().updateChart(frameNum_);
			Function.getInstance().reDraw( WindowManager.getCurrentImage(), frameNum_, true,false);
			Function.getInstance().PullMagnet(frameNum_);

		} catch (JSONException e) {
		} catch (MMScriptException e) {
		}
	}
}