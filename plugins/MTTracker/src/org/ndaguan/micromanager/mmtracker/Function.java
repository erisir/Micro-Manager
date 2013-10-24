package org.ndaguan.micromanager.mmtracker;

import ij.ImagePlus;
import ij.ImageStack;
import ij.WindowManager;

import java.awt.Point;
import java.awt.Rectangle;
import java.io.IOException;
import java.net.UnknownHostException;
import java.util.List;
import java.util.concurrent.TimeUnit;

import javax.swing.SwingUtilities;

import mmcorej.CMMCore;

import org.micromanager.MMStudioMainFrame;
import org.micromanager.utils.MMScriptException;
import org.ndaguan.micromanager.stagecontrol.SigmaKoki;
import org.ndaguan.micromanager.stagecontrol.SigmaStageControlFrame;

public class Function {

	private static Function instance_;
	private MMStudioMainFrame gui_;
	private CMMCore core_;

	private double currxpos_;
	private double currypos_;
	private double currzpos_;

	public static boolean isCalibrationRunning = false;
	public static boolean isInstalCallback;
	public static boolean isCapture_ = false;
	public static boolean isTestingRunning_ = false;

	private boolean isXYAcqAnalyzerInstall_  = false;
	private boolean isXYZAcqAnalyzerInstall_  = false;
	private boolean isCalibrateAnalyzerInstall_ = false;
	private boolean isTestingAnalyzerInstall_ = false;

	private List<RoiItem> roiList_;
	private Kernel kernel_;
	private boolean buttonIsLocked_ = true;

	public void clearROI(){
		Kernel.getInstance().roiList_.clear();
	}
	public Function(MMStudioMainFrame gui, List<RoiItem> roiList) {
		core_ = gui.getMMCore();
		gui_ = gui;
		roiList_ = roiList;
		kernel_ = Kernel.getInstance();

	}
	public Function(List<RoiItem> roiList) {
		roiList_ = roiList;
		kernel_ = Kernel.getInstance();

	}

	public static Function getInstance(MMStudioMainFrame gui,List<RoiItem> roiList) {
		if(instance_ == null)
			instance_ = new Function(gui,roiList);

		return instance_;
	}
	public static Function getInstance(){
		return instance_;
	}

	void dataReset(){
		if(Listener.getInstance().isRunning())
			Listener.getInstance().stop();
		for(RoiItem it:roiList_)
			it.dataClean(false);
	}

	public void PullMagnet(long frameNum ) {

		if(MMTFrame.getInstance().isMagnetAuto() && ( (int)( frameNum % MMT.VariablesNUPD.frameToCalcForce.value()) == 0)){
			MagnetIncrease();
		}
		if(MMTFrame.getInstance().isMagnetCirculate() && ( (int)( frameNum % MMT.VariablesNUPD.frameToCalcForce.value()) == 0)){
			try {
				MagnetCirculate();
			} catch (Exception e) {
				MMT.logError("Set MP285 ZStage ERR" + e.toString());
			}
		}

	}
	private void MagnetCirculate() throws Exception {
		double currMP285zpos = core_.getPosition(MMT.magnetZStage_);
		double z = MMT.VariablesNUPD.magnetStepSize.value();
		double target = currMP285zpos;
		if(MMT.magnetCurrentStage){//up
			MMT.magnetCurrentStage = false;
			target =  currMP285zpos + z;
		}else{//down
			MMT.magnetCurrentStage = true;
			target =  currMP285zpos - z;
		}
		if(target>0){
			target = 0;
		}
		final double c = currMP285zpos;
		final double t = target;
		(new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					core_.setPosition(MMT.magnetZStage_,t);
					MMT.magnetCurrentPosition = t;
					MMT.logMessage(String.format("\tSet MP285 ZStage\t\t%f\t->\t%f",
							c, core_.getPosition(MMT.magnetZStage_)));
				} catch (Exception e) {
					MMT.logError("Set MP285 ZStage ERR" + e.toString());
				}
			}
		})).start();

	}
	public void MagnetIncrease(){
		(new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					double currMP285zpos = core_.getPosition(MMT.magnetZStage_);
					double z = MMT.VariablesNUPD.magnetStepSize.value();
					double target =  currMP285zpos - z;
					if(target>0){
						target = 0;
						MMTFrame.getInstance().MagnetManual.setSelected(true);
					}
					core_.setPosition(MMT.magnetZStage_,target);
					MMT.magnetCurrentPosition = target;
					MMT.logMessage(String.format("\tSet MP285 ZStage\t\t%f\t->\t%f",
							currMP285zpos , core_.getPosition(MMT.magnetZStage_)));
				} catch (Exception e) {
					MMT.logError("Set MP285 ZStage ERR" + e.toString());
				}
			}
		})).start();
	}
	public int getFocusRoiIndex() {
		int index =0;
		for(RoiItem it:roiList_){			
			if(it.isFocus())
				return index;
			else
				index++;		
		}
		return -1;
	}

	public boolean overlapCheck(int xCenter,int yCenter) {
		for(RoiItem it:roiList_){	
			double[] xy = it.getXY();
			double dx = xCenter - xy[0];
			double dy = yCenter - xy[1];
			double r = Math.sqrt(dx*dx + dy*dy);			
			if(r <2 * (int)MMT.VariablesNUPD.beanRadiuPixel.value()){
				return true;
			}		
		}
		return false;
	}

	public int getCurrentRoiIndex(Point loc) {
		int index =0;
		int beanRadius = (int) MMT.VariablesNUPD.beanRadiuPixel.value();
		for(RoiItem it:roiList_){	
			double[] xy = it.getXY();
			Rectangle roi = new Rectangle((int) xy[0]-beanRadius,(int) xy[1]-beanRadius,2*beanRadius,2*beanRadius);
			if(roi.contains(loc))
				return index;
			else
				index++;		
		}
		return -1;
	}

	public int getReferenceRoiIndex() {
		int index =0;
		for(RoiItem it:roiList_){			
			if(it.isPreference())
				return index;
			else
				index++;		
		}
		return -1;
	}
	public int getBackgroundRoiIndex() {
		int index =0;
		for(RoiItem it:roiList_){			
			if(it.isBackground())
				return index;
			else
				index++;		
		}
		return -1;
	}


	public void reSetFocusRoi() {
		ImagePlus ip = WindowManager.getCurrentImage();
		final int focus = getFocusRoiIndex();
		if(focus != -1){
			double[] xy = roiList_.get(focus).getXY();
			int xCenter = (int) xy[0];	
			int yCenter = (int) xy[1];	
			int beanRadius = (int) MMT.VariablesNUPD.beanRadiuPixel.value();
			if(ip != null)
				ip.setRoi(xCenter-beanRadius,yCenter-beanRadius,2*beanRadius,2*beanRadius);					
		} 
	}

	public void setFocusdRoi(Point point) {
		int focus = getFocusRoiIndex();
		if(focus != -1){
			roiList_.get(focus).setFocus(false);
		}

		int index = getCurrentRoiIndex(point);
		if( index != -1){
			roiList_.get(index).setFocus(true);
		}
	}

	public void moveFocusdRoi(int dx, int dy) {
		int index = getFocusRoiIndex();
		if( index != -1){
			double[] xy = roiList_.get(index).getXY();
			roiList_.get(index).setXY(xy[0]+dx,xy[1]+dy);
		}	
	}

	public void addRoi(Rectangle rectangle) {
		int xCenter = rectangle.x+rectangle.width/2;
		int yCenter = rectangle.y+rectangle.height/2;
		if(kernel_.isCalibrated_)
			return;
		if(!overlapCheck(xCenter,yCenter))//selected area exist ROI
		{	
			int index = getFocusRoiIndex();
			if(index != -1)
			{
				roiList_.get(index).setFocus(false);
			}
			synchronized(MMT.Acqlock){
				roiList_.add(RoiItem.createInstance(new double[]{xCenter,yCenter,0,0,0,},GetXYPositionAnalyzer.getInstance().acqName_));
			}
		}
	}
	public void selectRoiAsBackground(Rectangle rectangle) {
		int index = getFocusRoiIndex();
		if( index != -1){
			int backgroundIndex = getBackgroundRoiIndex();
			if(backgroundIndex != -1){
				roiList_.get(backgroundIndex).setBackground(false);
			}
			roiList_.get(index).setBackground(true);
			MMT.logMessage("ROI"+String.valueOf(index)+" is selected as background ");
		}else{
			int xCenter = rectangle.x+rectangle.width/2;
			int yCenter = rectangle.y+rectangle.height/2;
			if(kernel_.isCalibrated_)
				return;
			if(!overlapCheck(xCenter,yCenter))//selected area exist ROI
			{	
				synchronized(MMT.Acqlock){
					roiList_.add(RoiItem.createInstance(new double[]{xCenter,yCenter,1,0,0,},GetXYPositionAnalyzer.getInstance().acqName_));
				}
			}
		}
	}
	public void deleteRoi() {
		int index = getFocusRoiIndex();
		if( index != -1){
			synchronized(MMT.Acqlock){
				roiList_.remove(index);
			}
			if(kernel_.isCalibrated_ && roiList_.size() == 0){
				kernel_.setIsCalibrated(false);
			}
			MMT.logMessage("ROI added :OK");
		}	
	}

	public void selectRoiAsReference() {
		int index = getFocusRoiIndex();
		if( index != -1){
			int preReferenceIndex = getReferenceRoiIndex();
			if(preReferenceIndex != -1){
				roiList_.get(preReferenceIndex).setPreference(false);
			}
			roiList_.get(index).setPreference(true);
			MMT.logMessage("ROI"+String.valueOf(index)+" is selected as Reference ");
		}
	}



	public void showChartManager() {
		int focus = getFocusRoiIndex();
		if(focus != -1){
			roiList_.get(focus).setChartVisible(true);
		}

	}

	public void reDraw(final String acqName, final long frameNum_, final boolean update,boolean forceRedraw) {

		if(frameNum_ %MMT.VariablesNUPD.frameToRefreshImage.value() !=0 && !forceRedraw)
			return;

		SwingUtilities.invokeLater(new Runnable(){
			@Override
			public void run() {
				try {
					OverlayRender.getInstance().render(acqName, roiList_, frameNum_, update);
					reSetFocusRoi();
				} catch (MMScriptException e) {
					MMT.logError("Render error!");
				}
			}
		});
	}

	public void showPreferencesDialog() {
		MMTFrame.getInstance().preferDailog.UpdateData(false);
		MMTFrame.getInstance().preferDailog.setVisible(true);
	}
	public void installAnalyzer(String analyzer){

		UninstallAnalyzer("CAL");
		UninstallAnalyzer("TEST");
		UninstallAnalyzer("XYACQ");
		UninstallAnalyzer("XYZACQ");

		switch(analyzer){
		case "XYACQ":
			gui_.getAcquisitionEngine().addImageProcessor(GetXYPositionAnalyzer.getInstance());
			isXYAcqAnalyzerInstall_ = true;
			break;
		case "XYZACQ":
			gui_.getAcquisitionEngine().addImageProcessor(GetXYZPositionAnalyzer.getInstance());
			isXYZAcqAnalyzerInstall_ = true;
			MMTFrame.getInstance().LiveView.setText("StopCapture");
			MMT.logMessage("XTZ Call back install,Start capture");
			break;
		case "CAL":
			gui_.getAcquisitionEngine().addImageProcessor(CalibrateAnalyzer.getInstance());
			isCalibrateAnalyzerInstall_ = true;
			break;
		case "TEST":
			gui_.getAcquisitionEngine().addImageProcessor(TestAnalyzer.getInstance());
			isTestingAnalyzerInstall_ = true;
			break;
		}
	}
	private void UninstallAnalyzer(String analyzer){
		switch(analyzer){
		case "XYACQ":
			if (isXYAcqAnalyzerInstall_) {
				gui_.getAcquisitionEngine().removeImageProcessor(GetXYPositionAnalyzer.getInstance());
				isXYAcqAnalyzerInstall_ = false;
				MMTFrame.getInstance().LiveView.setText("LiveView");
				MMT.logMessage("XYCall back uninstal,Stop capture");
			} 
			break;
		case "XYZACQ":
			if (isXYZAcqAnalyzerInstall_) {
				gui_.getAcquisitionEngine().removeImageProcessor(GetXYZPositionAnalyzer.getInstance());
				isXYZAcqAnalyzerInstall_ = false;
				MMTFrame.getInstance().LiveView.setText("LiveView");
				MMT.logMessage("XYZCall back uninstal,Stop capture");
			} 
			break;
		case "CAL":
			if (isCalibrateAnalyzerInstall_) {
				gui_.getAcquisitionEngine().removeImageProcessor(CalibrateAnalyzer.getInstance());
				isCalibrateAnalyzerInstall_ = false;
			} 
			break;
		case "TEST":
			if (isTestingAnalyzerInstall_) {
				gui_.getAcquisitionEngine().removeImageProcessor(TestAnalyzer.getInstance());
				isTestingAnalyzerInstall_ = false;
			} 
			break;
		}
	}

	public void multiAcq() {
		gui_.getAcqDlg().setVisible(true);
	}

	public void setXYZCalPosition(int z) throws Exception 
	{
		if(MMT.VariablesNUPD.needStageServer.value() == 1){
			TCPClient.getInstance().setPosition(kernel_.xPosProfiles[z],kernel_.yPosProfiles[z],kernel_.zPosProfiles[z]);
			TimeUnit.MILLISECONDS.sleep((long) MMT.VariablesNUPD.stageMoveSleepTime.value());
		}
		else{
			setStageZPosition(kernel_.zPosProfiles[z]);
			if(MMT.VariablesNUPD.needXYcalibrate.value() == 1)
				setStageXYPosition(kernel_.xPosProfiles[z],kernel_.yPosProfiles[z]);

			if(MMT.VariablesNUPD.needCheckStageMovment.value() == 1){
				boolean stageReady = false;
				double targetPosition[] = new double[]{kernel_.xPosProfiles[z],kernel_.yPosProfiles[z],kernel_.zPosProfiles[z]};
				double allowError = MMT.VariablesNUPD.stageMovmentPrecision.value();
				while(!stageReady){
					double[] pos = getStagePosition();
					for(int i = 0;i<3;i++){
						stageReady = pos[i]-targetPosition[i]<allowError;
						if(!stageReady){
							TimeUnit.MICROSECONDS.sleep((long) MMT.VariablesNUPD.stageMoveSleepTime.value());
							break;
						}
					}
				}
			}
		}

	}
	public void setZCalPosition(int z) throws Exception {
		setStageZPosition(kernel_.zPosProfiles[z]);		
	}
	public double[] getStagePosition() throws Exception
	{
		double pos[] = new double[3];

		if(MMT.VariablesNUPD.needStageServer.value() == 1){
			pos = TCPClient.getInstance().getPosition();
		}
		else{
			double[] xpos = new  double[1];
			double[] ypos = new  double[1];
			if(MMT.VariablesNUPD.hasXYStage.value() != 0){
				core_.getXYPosition(MMT.xyStage_, xpos, ypos);
				pos[0] = xpos[0];
				pos[1] = ypos[0];
			}
			pos[2] = SigmaKoki.getInstance().getPosition();
//			pos[2] = core_.getPosition(MMT.zStage_);
		}
		return pos;
	}

	public void updateCalibrationProfile(){
		double calRange = MMT.VariablesNUPD.calRange.value();
		double calStepSize = MMT.VariablesNUPD.calStepSize.value();
		double testingPrecision = MMT.VariablesNUPD.testingPrecision.value();
		double[][] cal = new double[ (int) (calRange/calStepSize)][(int) (MMT.VariablesNUPD.beanRadiuPixel.value()/MMT.VariablesNUPD.rInterStep.value())];
		kernel_.zPosProfiles = new double[ (int) (calRange/calStepSize)];
		kernel_.zTestingPosProfiles = new double [(int) (calRange/testingPrecision)];

		for (RoiItem it:roiList_) 
			it.InitializeCalProflie(cal);
		int calSize = (int) (calRange/calStepSize);
		try {
			updatePositions();
		} catch (Exception e) {
			e.printStackTrace();
		}
		int midPoint = (int) (calRange / 2);
		for (int i = 0; i < calSize; i++) {
			kernel_.zPosProfiles[i] = currzpos_ - midPoint + i * calStepSize;
		}
		double start = kernel_.zPosProfiles[0];
		double end = kernel_.zPosProfiles[kernel_.zPosProfiles.length -1];
		for (int i = 0; i < kernel_.zTestingPosProfiles.length; i++) {
			double target = kernel_.zPosProfiles[0] +  (i+ Math.floor(Math.random()*100)/100) * testingPrecision;
			if(target > end)
				target = end;
			if(target <start)
				target = start;
			kernel_.zTestingPosProfiles[i] = target;
		}
		midPoint = (int) (calSize / 2);
		kernel_.xPosProfiles = new double[ (int) (calRange/calStepSize)];
		kernel_.yPosProfiles = new double[ (int) (calRange/calStepSize)];

		for (int j = 0; j < midPoint; j++){
			kernel_.xPosProfiles[j] =  currxpos_ + calStepSize * j;
			kernel_.yPosProfiles[j] =  currypos_ + calStepSize * j;
		}
		for (int j = midPoint; j < calSize; j++){
			kernel_.xPosProfiles[j] = kernel_.xPosProfiles[midPoint - 1] - calStepSize * (j - midPoint);
			kernel_.yPosProfiles[j] = kernel_.yPosProfiles[midPoint - 1] - calStepSize * (j - midPoint);
		}
	}
	public void CalProfileDataCleanup(){
		kernel_.xPosProfiles = null;
		kernel_.yPosProfiles = null;
		kernel_.zPosProfiles = null;
		for(RoiItem it:roiList_)
			it.clearCalProfile();
	}
	public void snapImage(){
		gui_.snapSingleImage();		
	}

	private void WHATISLOVE(String mode){
		MMT.isAnalyzerBusy_ = false;
		switch(mode){
		case "CalibrateStart":
			kernel_.isCalibrated_ = false;

			MMT.isCalibrationRunning_ = true;
			MMT.isTestingRunning_ = false;
			MMT.isGetXYPositionRunning_ = false;
			MMT.isGetXYZPositionRunning_ = false;
			setProgressBarMaxValue(kernel_.zPosProfiles.length);
			MMTFrame.getInstance().preferDailog.enableEdit(false);
			MMTFrame.getInstance().setCalibrateIcon(false);
			MMTFrame.getInstance().setEnableCalibrateIcon(false);
			MMTFrame.getInstance().setEnableLiveIcon(false);
			MMTFrame.getInstance().setLiveViewIcon(false);
			setProgressBarVisible(true);
			MMT.logMessage("Calibration Start......");
			break;
		case "CalibrateTrue":
			if(MMT.VariablesNUPD.needXYcalibrate.value() == 1)
				kernel_.setPixelToPhys();

			kernel_.isCalibrated_ = false;

			MMT.isCalibrationRunning_ = false;
			MMT.isTestingRunning_ = false;
			MMT.isGetXYPositionRunning_ = false;
			MMT.isGetXYZPositionRunning_ = false;

			MMTFrame.getInstance().preferDailog.enableEdit(false);
			MMTFrame.getInstance().setCalibrateIcon(false);
			MMTFrame.getInstance().setEnableCalibrateIcon(false);
			setProgressBarVisible(false);
			MMT.logMessage("Calibration OK......");
			break;
		case "CalibrateFalse":
			kernel_.isCalibrated_ = false;
			installAnalyzer("XYACQ");
			MMT.isCalibrationRunning_ = false;
			MMT.isTestingRunning_ = false;
			MMT.isGetXYPositionRunning_ = false;
			MMT.isGetXYZPositionRunning_ = false;
			MMTFrame.getInstance().preferDailog.enableEdit(true);
			MMTFrame.getInstance().setCalibrateIcon(false);
			MMTFrame.getInstance().setEnableCalibrateIcon(true);
			setProgressBarVisible(false);
			CalProfileDataCleanup();
			try {
				Function.getInstance().StageGoHome();
			} catch (Exception e1) {
				MMT.logError("Stage cannot go back home");
			}
			MMT.logMessage("Calibration False......");
			liveView();
			break;
		case "TestingStart":
			kernel_.isCalibrated_ = true;

			MMT.isCalibrationRunning_ = false;
			MMT.isTestingRunning_ = true;
			MMT.isGetXYPositionRunning_ = false;
			MMT.isGetXYZPositionRunning_ = false;

			MMTFrame.getInstance().preferDailog.enableEdit(false);
			MMTFrame.getInstance().setCalibrateIcon(false);
			MMTFrame.getInstance().setEnableCalibrateIcon(false);
			setProgressBarVisible(true);
			setProgressBarMaxValue(kernel_.zTestingPosProfiles.length);
			cleanTestingData();
			roiList_.get(0).setChartVisible(true);
			roiList_.get(0).setSelectTap("Chart-Testing");
			MMT.logMessage("Testing Start......");
			break;
		case "TestingTrue":
			kernel_.isCalibrated_ = true;

			MMT.isCalibrationRunning_ = false;
			MMT.isTestingRunning_ = false;
			MMT.isGetXYPositionRunning_ = false;
			MMT.isGetXYZPositionRunning_ = false;
			MMTFrame.getInstance().preferDailog.UpdateData(false);
			MMTFrame.getInstance().preferDailog.enableEdit(true);
			MMTFrame.getInstance().setCalibrateIcon(true);
			MMTFrame.getInstance().setEnableCalibrateIcon(true);
			setProgressBarVisible(false);
			try {
				Function.getInstance().StageGoHome();
			} catch (Exception e1) {
				MMT.logError("Stage cannot go back home");
			}
			MMTFrame.getInstance().LiveViewCaptureEnable(true);
			MMTFrame.getInstance().MultCaptureEnable(true);
			MMTFrame.getInstance().setEnableLiveIcon(true);
			MMT.logMessage("Testing OK......");
			break;
		case "TestingFalse":
			kernel_.isCalibrated_ = false;
			installAnalyzer("XYACQ");
			MMT.isCalibrationRunning_ = false;
			MMT.isTestingRunning_ = false;
			MMT.isGetXYPositionRunning_ = false;
			MMT.isGetXYZPositionRunning_ = false;
			MMTFrame.getInstance().preferDailog.enableEdit(true);
			MMTFrame.getInstance().setCalibrateIcon(false);
			MMTFrame.getInstance().setEnableCalibrateIcon(true);
			setProgressBarVisible(false);
			CalProfileDataCleanup();
			try {
				Function.getInstance().StageGoHome();
			} catch (Exception e1) {
				MMT.logError("Stage cannot go back home");
			}
			MMT.logMessage("Testing False......");
			liveView();
			break;
		}
	}

	private void testing() {
		installAnalyzer("TEST");
		WHATISLOVE("TestingStart");
		try {
			setXYZCalPosition(0);
		} catch (Exception e) {
			WHATISLOVE("TestingFalse");
			MMT.logError("testing error1:Set Pi Stage error!\r\n"+e.toString());
			return;
		}
		//testing start
		for (int i = 0; i <kernel_.zTestingPosProfiles.length; i ++) {
			if(MMT.isTestingRunning_){
				try {
					MMT.testingIndex_ = i;
					setStageZPosition(kernel_.zTestingPosProfiles[i]);
					MMT.isAnalyzerBusy_ = true;
					snapImage();
					while(MMT.isAnalyzerBusy_){
						TimeUnit.MICROSECONDS.sleep(10);
					}
				} catch (Exception e) {
					WHATISLOVE("TestingFalse");
					MMT.logError("Testing error1:Set Pi Stage error!\r\n"+e.toString());
					return;
				}
			}
			else{
				WHATISLOVE("TestingFalse"); 
				MMT.logError(MMT.lastError_);
				return;
			}
			setProgressBarValue(i);
		}//for end
		WHATISLOVE("TestingTrue");
		installAnalyzer("XYACQ");
		liveView();
	}


	public void calibrate() {
		if(roiList_ ==null ||roiList_.size() <= 0){
			MMT.logError("No roi in the image,Try ctrl+A");
			return;
		}
		if(MMT.VariablesNUPD.needStageServer.value() == 0){
			MMT.xyStage_ = (((int)MMT.VariablesNUPD.hasXYStage.value()) == 1)?core_.getXYStageDevice():null;
			MMT.zStage_ = (((int)MMT.VariablesNUPD.hasZStage.value()) == 1)?core_.getFocusDevice():null;
			if( MMT.zStage_ == null){
				MMT.logError("there is no zstage avilable");
				return;
			}
		}
		stopLiveView();
		stopFeedBack();
		updateCalibrationProfile();
		installAnalyzer("CAL");
		WHATISLOVE("CalibrateStart");

		try {
			setXYZCalPosition(0);
		} catch (Exception e) {
			WHATISLOVE("CalibrateFalse");
			MMT.logError("Calbration error1:Set Pi Stage error!\r\n"+e.toString());
			return;
		}

		for(RoiItem it:roiList_)
			it.setZOrign(currzpos_ - MMT.VariablesNUPD.beanRadius.value());
		//calibration start
		for (int i = 0; i < kernel_.zPosProfiles.length; i++) {
			if(MMT.isCalibrationRunning_){
				try {
					Function.getInstance().setXYZCalPosition(i);
					MMT.calibrateIndex_ = i;
					MMT.isAnalyzerBusy_ = true;
					snapImage();
					while(MMT.isAnalyzerBusy_){
						TimeUnit.MICROSECONDS.sleep(10);
					}
				} catch (Exception e) {
					WHATISLOVE("CalibrateFalse");
					MMT.logError("Calbration error1:Set Pi Stage error!\r\n"+e.toString());
					return;
				}
			}
			else{
				WHATISLOVE("CalibrateFalse");
				MMT.logError(MMT.lastError_);
				return;
			}
			setProgressBarValue(i);
		}//for end
		WHATISLOVE("CalibrateTrue");
		testing();
	}
	private void stopFeedBack() {
		if(MMT.isFeedbackRunning_)
			EnableFeedback();
	}
	private void stopLiveView() {
		if (gui_.getAcquisitionEngine().isAcquisitionRunning()) {
			gui_.getAcquisitionEngine().stop(true);
		}

		if (gui_.isLiveModeOn()) {
			gui_.enableLiveMode(false);
		}		
	}

	private void setProgressBarVisible(boolean b) {
		MMTFrame.getInstance().bar.setVisible(b);		
	}
	void setProgressBarMaxValue(int i){
		MMTFrame.getInstance().bar.setMaximum(i);
	}
	void setProgressBarValue(int i){
		final int value = i;
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				MMTFrame.getInstance().bar.setValue(value);
			}
		});
	}

	public void liveView() {
		if(gui_.isLiveModeOn()){
			gui_.enableLiveMode(false);
			try {
				TimeUnit.MICROSECONDS.sleep(200);
			} catch (InterruptedException e) {
			}
			gui_.enableLiveMode(true);
		}else{
			gui_.enableLiveMode(true);
		}
	}
	public void updateTestingChart(final double currZpos) {
		SwingUtilities.invokeLater(new Runnable(){
			@Override
			public void run() {
				for(int i = 0;i<roiList_.size();i++)
				{
					roiList_.get(i).addChartData("Chart-Testing",currZpos,roiList_.get(i).getZ() - currZpos);
				}
			}
		});
	}

	public void cleanTestingData() {
		for(int i = 0;i<roiList_.size();i++)
		{
			roiList_.get(i).clearChart("Chart-Testing");
		}
	}

	public void updateChart(final long frameNum) {
		for (RoiItem item:roiList_)
			item.updateDataSeries(frameNum);
	}


	public void ShowMagnetManualDialBox() {
		MMTFrame.getInstance().MagnetAuto.setSelected(false);
		MMTFrame.getInstance().myStageControlFrame_.setVisible(true);
	}
	@SuppressWarnings("unused")
	private void whataday() throws Exception{
		MMT.zStage_ = core_.getFocusDevice();
		MMT.xyStage_ = core_.getXYStageDevice();
		double range = MMT.VariablesNUPD.calRange.value();
		double stepsize = MMT.VariablesNUPD.calStepSize.value();
		int imageNum = (int) MMT.VariablesNUPD.frameToCalcForce.value();
		int len = (int) (range/stepsize);
		updatePositions();
		final double start = currzpos_ - range/2;
		for(int k =0;k<(int)MMT.VariablesNUPD.showDebugTime.value();k++){//
			for(int i=0;i<len;i++){
				double target = start+i*stepsize;
				setStageZPosition(target);
				for (int j = 0; j <imageNum; j++){ 
					gui_.snapAndAddToImage5D();
					TimeUnit.MILLISECONDS.sleep((long) MMT.VariablesNUPD.stageMoveSleepTime.value());
					MMT.logMessage(String.format("currZPos:\t%f(%d/%d)\timageNum:\t%d/%d", target,i,len,j,imageNum));
				}
			}
		}
		setStageZPosition(currzpos_);
	}
	public void setAutoContrast() {

		try {
			gui_.setContrastBasedOnFrame(GetXYPositionAnalyzer.getInstance().acqName_, 0, 0);
		} catch (MMScriptException e) {
			MMT.logError("Set auto contrast false");
		}		
	}

	public void showGui() {
		gui_.setVisible(!gui_.isVisible());
	}

	public void showIJ() {
		ij.ImageJ.getFrames()[0].setVisible(!ij.ImageJ.getFrames()[0].isVisible());
	}

	public void updatePositions() throws Exception  {
		double[] pos = getStagePosition();
		currxpos_ = pos[0];
		currypos_ = pos[1];
		currzpos_ = pos[2];
	}

	public void StageGoHome() throws Exception  {
		setStageXYPosition( currxpos_,currypos_);
		setStageZPosition(currzpos_);
	}

	public void setStageXYPosition(double xpos,double ypos) throws Exception {
		if(MMT.xyStage_ != null){
			if(MMT.VariablesNUPD.needStageServer.value() == 1){
				TCPClient.getInstance().setPosition(MMT.xyStage_,xpos, ypos);
			}
			else{
				core_.setXYPosition(MMT.xyStage_,xpos, ypos);
				TimeUnit.MILLISECONDS.sleep((long) MMT.VariablesNUPD.stageMoveSleepTime.value());
			}
		}
	}
	public void setStageRelativeXYPosition(double xpos,double ypos) throws Exception {
		if(MMT.xyStage_ != null){
			if(MMT.VariablesNUPD.needStageServer.value() == 1){
				TCPClient.getInstance().setRelativePosition(MMT.xyStage_,xpos, ypos);
			}
			else{
				core_.setRelativeXYPosition(MMT.xyStage_,xpos, ypos);
				TimeUnit.MILLISECONDS.sleep((long) MMT.VariablesNUPD.stageMoveSleepTime.value());
			}
		}
	}
	public void setStageXYZPosition(double[] pos) throws Exception {
		setStageXYPosition(pos[0],pos[1]);
		setStageZPosition(pos[2]);
	}

	public void setStageRelativeXYZPosition(double[] pos) throws Exception {
		if(pos[0] != 0 || pos[1] != 0)
			setStageRelativeXYPosition(pos[0],pos[1]);
		if(pos[2] != 0)
			setStageRelativeZPosition(pos[2]);
	}

	public void setStageRelativeXYPosition(double[] pos) throws Exception {
		if(pos[0] != 0 || pos[1] != 0)
			setStageRelativeXYPosition(pos[0],pos[1]);
	}

	public void setStageZPosition(double zPos) throws Exception {
		if(MMT.zStage_ != null){
			if(MMT.VariablesNUPD.needStageServer.value() == 1){
				TCPClient.getInstance().setPosition(MMT.zStage_,zPos);
			}
			else{
//				core_.setPosition(MMT.zStage_, zPos);
				SigmaKoki.getInstance().setPosition(zPos);
			}
			TimeUnit.MILLISECONDS.sleep((long) MMT.VariablesNUPD.stageMoveSleepTime.value());
		}
	}
	public void setStageRelativeZPosition(double zPos) throws Exception {
		if(MMT.zStage_ != null){
			if(MMT.VariablesNUPD.needStageServer.value() == 1){
				TCPClient.getInstance().setRelativePosition(MMT.zStage_,zPos);
			}
			else{
				core_.setRelativePosition(MMT.zStage_, zPos);
			}
			TimeUnit.MILLISECONDS.sleep((long) MMT.VariablesNUPD.stageMoveSleepTime.value());
		}
	}
	public void liveCapture() {
		if(isXYAcqAnalyzerInstall_ && kernel_.isCalibrated_){
			installAnalyzer("XYZACQ");
			for(RoiItem it:roiList_)
				it.setSelectTap("Chart-Z");
			MMTFrame.getInstance().preferDailog.enableEdit(false);
			MMTFrame.getInstance().setLiveViewIcon(true);
			for(RoiItem it:roiList_)
				it.setChartVisible(true);
		}else{
			installAnalyzer("XYACQ");
			stopFeedBack();
			MMTFrame.getInstance().preferDailog.enableEdit(true);
			MMTFrame.getInstance().setLiveViewIcon(false);
			for(RoiItem it:roiList_)
				it.setChartVisible(false);
		}
		liveView();
	}

	public void cleanStaticData() {
		for(RoiItem item: roiList_){
			item.clearStaticData();
		}
	}

	public void lockEveryThingButThis() {
		MMTFrame.getInstance().lockEveryThingButThis(!buttonIsLocked_);
		buttonIsLocked_ = !buttonIsLocked_;
	}

	public void updateCorrChart(final int roiIndex,final double[] yArray) {

		if(MMT.debug && MMT.currentframeIndex_% MMT.VariablesNUPD.showDebugTime.value() == 0){
			roiList_.get(roiIndex).clearChart("Chart-Corr");
			SwingUtilities.invokeLater(new Runnable() {
				@Override
				public void run() {
					for (int j = 0; j < yArray.length; j++) {
						roiList_.get(roiIndex).addChartData("Chart-Corr",j,yArray[j]);
					}
				}
			});
		}
	}
	public void updateChartSumXY(final int roiIndex, final double[][] sumXY) {
		if(MMT.debug && MMT.currentframeIndex_% MMT.VariablesNUPD.showDebugTime.value() == 0){
			roiList_.get(roiIndex).clearChart("Chart-SumX");
			roiList_.get(roiIndex).clearChart("Chart-SumY");
			SwingUtilities.invokeLater(new Runnable() {
				@Override
				public void run() {
					for (int j = 0; j < sumXY[0].length; j++) {
						roiList_.get(roiIndex).addChartData("Chart-SumX",j,sumXY[0][j]);
						roiList_.get(roiIndex).addChartData("Chart-SumY",j,sumXY[1][j]);
					}
				}
			});
		}

	}
	public void updatePosProfileChart(final int roiIndex,final double[] currProfiles) {
		final  double[] posProfile = currProfiles;
		if(MMT.debug && MMT.currentframeIndex_% MMT.VariablesNUPD.showDebugTime.value() == 0){
			roiList_.get(roiIndex).clearChart("Chart-PosProfile");
			SwingUtilities.invokeLater(new Runnable() {
				@Override
				public void run() {
					for (int j = 0; j < posProfile.length; j++) {
						roiList_.get(roiIndex).addChartData("Chart-PosProfile",j,posProfile[j]);
					}
				}
			});		
		}
	}

	public void SetXYOrign() {
		for(RoiItem it:roiList_)
			it.setXYOrign();
	}

	public void doFeedback() {
		try {
			double Kp = MMT.VariablesNUPD.pTerm.value();
			double Ki = MMT.VariablesNUPD.iTerm.value();
			int index = getReferenceRoiIndex();
			double[] target = roiList_.get(index).getFeedbackTarget();
			double[] currPos = roiList_.get(index).getXYZPhy();
			double [] delta = new double[3];
			double [] integrate = roiList_.get(index).getFeedbackIntegrate();
			for(int i=0;i<3;i++){
				delta[i] = currPos[i] - target[i];
			}
			double[] stageTarget = new double[3];
			if(MMT.VariablesNUPD.XYMirror.value() == 1){
			stageTarget[0] = Kp*delta[1]+Ki*integrate[1];//XY Transfer
			stageTarget[1] = Kp*delta[0]+Ki*integrate[0];
			}else{
				stageTarget[0] = Kp*delta[0]+Ki*integrate[0];//XY not need Transfer
				stageTarget[1] = Kp*delta[1]+Ki*integrate[1];
			}
			
			stageTarget[2] = Kp*delta[2]+Ki*integrate[2];
			double maxMoveStep = MMT.VariablesNUPD.feedBackMaxStepSize.value();
			double minMoveStep = MMT.VariablesNUPD.feedBackMinStepSize.value();
			for(int i=0;i<3;i++){
				double absT = Math.abs(stageTarget[i]);
				stageTarget[i] = absT>maxMoveStep?(maxMoveStep*(absT/stageTarget[i])):stageTarget[i];
				stageTarget[i] = absT<minMoveStep?0:stageTarget[i];
			}
			if(kernel_.isCalibrated_)
				setStageRelativeXYZPosition(stageTarget );
			else
				setStageRelativeXYPosition(stageTarget );
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void EnableFeedback() {
		if(!MMT.isFeedbackRunning_){
			int index = getReferenceRoiIndex();
			if(index != -1){
				roiList_.get(index).setFeedbackTarget();
				MMTFrame.getInstance().setFeedbackIcon(true);
				MMT.logMessage("Feedback mode is ON");
				MMT.isFeedbackRunning_ = true;
			}else{
				MMT.logError("No reference ROI is selected! try ctrl+s");
			}
		}else{
			MMTFrame.getInstance().setFeedbackIcon(false);
			MMT.logMessage("Feedback mode is OFF");
			MMT.isFeedbackRunning_ = false;
		} 

	}
	public void TCPIPClient() {
		TCPServer tcpServer = MMTracker.getInstance().getTcpServer();
		if(tcpServer != null && tcpServer.isRunning())
			try {
				MMTracker.getInstance().getTcpServer().stop();
			} catch (InterruptedException e) {
				MMT.logError("TCPIPServer stop error!" + e.toString());
			}
		try {
			MMTracker.getInstance().setTcpClient(TCPClient.getInstance("127.0.0.1", MMT.TCPIPPort));
			MMT.logMessage("TCPClient start ok");
		} catch (UnknownHostException e) {
			MMT.logError("TCPIPClient start error!" + e.toString());
		} catch (IOException e) {
			MMT.logError("TCPIPClient start error!" + e.toString());
		}

	}
	public void TCPIPServer() {
		TCPClient tcpClient = MMTracker.getInstance().getTcpClient();
		if(tcpClient != null && tcpClient.isRunning())
			try {
				MMTracker.getInstance().getTcpClient().stop();
			} catch ( IOException e) {
				MMT.logError("TCPIPClient stop error!" + e.toString());
			}
		MMTracker.getInstance().setTcpServer(TCPServer.getInstance(core_,MMT.TCPIPPort));
		MMTracker.getInstance().getTcpServer().start();	
		MMT.logMessage("TCPIPServer start ok");
	}

	public void runDebug() {
		gaussionCenterlization();		
	}
	public void gaussionCenterlization(){
		final ImagePlus currentImage = WindowManager.getCurrentImage();
		ImageStack images = currentImage.getImageStack();

		kernel_.imageHeight = images.getHeight();
		kernel_.imageWidth = images.getWidth();
		kernel_.roiList_.add(RoiItem.createInstance(new double[]{ images.getHeight()/2, images.getWidth()/2,0,0,0,},"test"));
		for (int i = 0; i < images.getSize(); i++) {
			if(!kernel_.getXYPosition(images.getPixels(i+1)))return;
			final int index = i;
			SwingUtilities.invokeLater(new Runnable(){
				@Override
				public void run() {
					currentImage.setSlice(index+2);
					updateChart(index);
				}
			});
			reDraw(currentImage.getWindow().getName(), i,true,true);

		}

	}
	public void showStageControl() {
		SigmaStageControlFrame.getInstance().setVisible(true);
	}

}
