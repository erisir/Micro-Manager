package org.ndaguan.micromanager.mmtracker;
import ij.WindowManager;
import ij.gui.ImageWindow;

import java.io.IOException;

import java.util.List;

import mmcorej.TaggedImage;

import org.json.JSONException;
import org.micromanager.MMStudio;
import org.micromanager.acquisition.TaggedImageQueue;
import org.micromanager.api.TaggedImageAnalyzer;
import org.micromanager.utils.MDUtils;
import org.micromanager.utils.MMScriptException;

public class GetXYPositionAnalyzer extends TaggedImageAnalyzer {

	private static GetXYPositionAnalyzer instance_;
	private long frameNum_;

	private Listener listener_;
	private double elapsed = 0;
	public int bitDepth_;
	public double imgwidth_;
	public double imgheight_;
	public  String acqName_;
	private Kernel kernel_;
	private long startTime;

	public static GetXYPositionAnalyzer getInstance() {	
		return instance_;
	}
	public static GetXYPositionAnalyzer getInstance(Kernel kernel, Listener listener,
			List<RoiItem> roiList_, OverlayRender render) {
		if(instance_ == null)
			instance_ = new GetXYPositionAnalyzer(kernel,listener,roiList_,render);
		return instance_;
	}

	public GetXYPositionAnalyzer(Kernel kernel, Listener listener,List<RoiItem> roiList,OverlayRender render) {
		listener_ = listener;
		kernel_ = kernel;
		frameNum_ = 0;
	}

	@Override
	protected void analyze(TaggedImage taggedImage) {
		MMT.tik();
		boolean update =  true;
		if (taggedImage == null || taggedImage == TaggedImageQueue.POISON)
		{
			Function.getInstance().dataReset();
			frameNum_ = 0;
			elapsed = 0;
			return;
		}

		if (taggedImage.tags.has("ElapsedTime-ms"))
		{
			try {
				elapsed = taggedImage.tags.getDouble("ElapsedTime-ms")/1000;
			} catch (JSONException e) {
			}

		}
		else{
			elapsed = (System.nanoTime() - startTime)  / 1e9;
		}

		try {
			String acqName = "Snap/Live Window";
			

			if(!listener_.isRunning()){
				Function.getInstance().dataReset();
				ImageWindow win = ij.WindowManager.getCurrentWindow();
				listener_.start(win);
				acqName_ = acqName;
				startTime = System.nanoTime();
				frameNum_ = 0;
				if (acqName.equals(MMT.SIMPLE_ACQ)) {
					kernel_.imageHeight = Integer.parseInt(taggedImage.tags
							.get("Height").toString());
					kernel_.imageWidth = Integer.parseInt(taggedImage.tags.get(
							"Width").toString());
				} else {
					Object height = taggedImage.tags.get("Height");
					Object width = taggedImage.tags.get("Width");
					if (height instanceof Number)
						kernel_.imageHeight = ((Number) height).intValue();
					else
						kernel_.imageHeight = Integer.parseInt(height
								.toString());
					if (width instanceof Number)
						kernel_.imageWidth = ((Number) width).intValue();
					else
						kernel_.imageHeight = Integer
						.parseInt(width.toString());
				}
			}
			if(!update){
				frameNum_ = MDUtils.getFrameIndex(taggedImage.tags);
			}
			else{
				frameNum_ ++;
			}
			synchronized(MMT.Acqlock){
				if(kernel_.roiList_.size()<=0){
					Function.getInstance().reDraw( WindowManager.getCurrentImage(), frameNum_, update,true);
					return;
				}
				if(!kernel_.getXYPosition(taggedImage.pix))return;
				if(MMT.isFeedbackRunning_ && frameNum_ % MMT.VariablesNUPD.frameToFeedBack.value() == 0){
					Function.getInstance().doFeedback();
				}
			}//lock
			String nameComp;
			if (acqName.equals(MMT.SIMPLE_ACQ))
				nameComp = "Live";
			else
				nameComp = acqName;
			if(MMT.VariablesNUPD.saveFile.value() == 1 && MMT.VariablesNUPD.responceXY.value() == 1)
				try {
					kernel_.saveRoiData(nameComp,frameNum_,elapsed);
				} catch (IOException e) {
					MMT.logError("Save data error");
				}
			Function.getInstance().reDraw( WindowManager.getCurrentImage(), frameNum_, true,true);
			if( MMT.VariablesNUPD.responceXY.value() == 1){
				Function.getInstance().updateChart(frameNum_);
				Function.getInstance().PullMagnet(frameNum_);
			}
      
		} catch (JSONException e) {
		} catch (MMScriptException e) {
		}

		MMT.tok("GetXYPosition");
	}
}