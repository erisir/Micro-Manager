package org.ndaguan.micromanager.mmtracker;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.apache.commons.math3.analysis.UnivariateFunction;
import org.apache.commons.math3.analysis.interpolation.SplineInterpolator;
import org.apache.commons.math3.analysis.polynomials.PolynomialFunction;
import org.apache.commons.math3.analysis.solvers.LaguerreSolver;
import org.apache.commons.math3.complex.Complex;
import org.apache.commons.math3.exception.NoBracketingException;
import org.apache.commons.math3.stat.correlation.PearsonsCorrelation;
import org.apache.commons.math3.stat.descriptive.DescriptiveStatistics;
import org.apache.commons.math3.stat.regression.SimpleRegression;
import org.apache.commons.math3.transform.DftNormalization;
import org.apache.commons.math3.transform.FastFourierTransformer;
import org.apache.commons.math3.transform.TransformType;

import Jama.Matrix;
import Jama.SingularValueDecomposition;

public class Kernel {

	private static Kernel kernel_;
	private DescriptiveStatistics statis_;
	private DescriptiveStatistics sumX_;
	private DescriptiveStatistics sumY_;
	private FastFourierTransformer FFT_;
	private SplineInterpolator interpolator;
	private PearsonsCorrelation pearCorrelation_;

	private SimpleRegression regrX;
	private SimpleRegression regrY;

	public List<RoiItem> roiList_;

	public  double[] zPosProfiles;
	public  double[] xPosProfiles;
	public  double[] yPosProfiles;

	public int imageWidth = 512;
	public int imageHeight = 512;
	public boolean isCalibrated_ = false;

	public double[] zTestingPosProfiles;
	public Kernel( List<RoiItem> roiList){
		roiList_ = roiList;
		FFT_ = new FastFourierTransformer(DftNormalization.STANDARD);//gosseCenter
		interpolator = new SplineInterpolator(); // gosseCenter and getZlocation
		statis_ = new DescriptiveStatistics();	//get zScore of posProfile
		sumX_ = new DescriptiveStatistics();	//gosseCenter
		sumY_ = new DescriptiveStatistics();	//gosseCenter
		pearCorrelation_ = new PearsonsCorrelation();// getZlocation
		regrX = new SimpleRegression(); //XY calibration
		regrY = new SimpleRegression(); //XY calibration
	}

	public static  Kernel getInstance(List<RoiItem> roiList) 
	{
		if(kernel_ == null)
			kernel_ = new Kernel(roiList);
		return kernel_;
	}
	public static Kernel getInstance() {
		return kernel_;
	}

	public boolean getXYPosition(Object image){	
		double[] force = new double[]{0,0};
		if(roiList_.size() <= 0)return false;
		//				MMT.tik();
		double[][] ret = gosseCenter(image);
		//		MMT.tok("GooseCenter");		
		if(ret == null)return false;
		MMT.currentframeIndex_++;
		for (int i = 0; i < roiList_.size(); i++) {
			//set value
			if(MMT.VariablesNUPD.iTerm_x.value() != 123){
				roiList_.get(i).setXY(ret[i]);
			}
			roiList_.get(i).setZ(ret[i][2]);
			//force = calcForces(roiList_.get(i).getStats());
			roiList_.get(i).setForce(force);
		}

		return true;
	}
	public boolean getXYZPosition(Object image){	

		double[] force = new double[]{0,0};
		if(roiList_.size() <= 0)return false;
		double[][] ret = gosseCenter(image);
		if(ret == null)return false;
		MMT.currentframeIndex_++;
		for (int i = 0; i < roiList_.size(); i++) {
			//set value
			roiList_.get(i).setXY(ret[i]);
			force = calcForces(roiList_.get(i).getStats());
			roiList_.get(i).setForce(force);

		}
		double beanRadiuPixel = MMT.VariablesNUPD.beanRadiuPixel.value();
		//get z
		double[] currProfiles = new double[(int) (beanRadiuPixel/MMT.VariablesNUPD.rInterStep.value())];
		for (int k = 0; k < roiList_.size(); k++) {
			double[] xy = roiList_.get(k).getXY();
			int roiX = (int) (xy[0] - beanRadiuPixel);
			int roiY = (int) (xy[1] - beanRadiuPixel);
			if(isRoiOutOfImage(roiX,roiY)){
				if(MMT.VariablesNUPD.AutoDeleteRoi.value() ==1){
					roiList_.remove(k);
					if(isCalibrated_){
						if(roiList_.size() == 0){
							setIsCalibrated(false);
						}
					}
				}
				return false;
			}
			currProfiles = polarIntegral(image,xy[0],xy[1]);		
			//			currProfiles = polarIntegral2(image,roiX,roiY,beanRadiuPixel);		
			double zpos = getZLocation(k,currProfiles);
			roiList_.get(k).setZ(zpos);
			//roiList_.get(k).setL();
			Function.getInstance().updatePosProfileChart(k,currProfiles);
		}
		return true;
	}
	@SuppressWarnings("unused")
	private double[] calcSkrewness(DescriptiveStatistics[] stats,DescriptiveStatistics statCross) {
		double[] skrewness = new double[2];
		double[] stds = new double[2];
		for (int i = 0; i < stds.length; i++) {
			stds[i] = stats[i].getStandardDeviation();
		}

		double n = statCross.getN();
		skrewness[0] = stds[0] / stds[1];
		skrewness[1] = (statCross.getMean() - stats[0].getMean()
				* stats[1].getMean())
				* n / (n - 1) / (stds[0] * stds[1]);
		return skrewness;
	}

	private double[] calcForces(DescriptiveStatistics[] stats) {

		LaguerreSolver solver = new LaguerreSolver();
		double[] forces = new double[2];
		double persistance = MMT.VariablesNUPD.persistance.value()*1000;
		double contourLength = MMT.VariablesNUPD.contourLen.value()*1000;
		double beanRadius = MMT.VariablesNUPD.beanRadius.value()*1000;
		double kT = MMT.VariablesNUPD.kT.value();

		for (int i = 0; i < forces.length; i++) {
			double variance = stats[i].getVariance();
			double a = 4 * persistance * contourLength/ variance;
			double b = 4 * persistance * beanRadius / variance;
			PolynomialFunction func = new PolynomialFunction(new double[] { b,
					a - 2 * b - 6, b - 2 * a + 9, a - 4 });
			try {
				forces[i] = (solver.solve(100, func, 0, 1, 0.8) * a + b) * kT
						/ (4 * persistance);
			} catch (NoBracketingException e) {

			} finally {

			}
		}
		return forces;
	}


	public  static void main(String[] args) {


		MMT.VariablesNUPD.calRange.value(10);
		MMT.VariablesNUPD.skipRadius.value(0);
		MMT.VariablesNUPD.frameToRefreshChart.value(100);
		MMT.VariablesNUPD.beanRadiuPixel.value(50);
		MMT.VariablesNUPD.pixelToPhysX.value(1.0);
		MMT.VariablesNUPD.pixelToPhysY.value(1.0);
		MMT.VariablesNUPD.calStepSize.value(1);
		MMT.VariablesNUPD.polarFactor.value(1);
		MMT.VariablesNUPD.showDebugTime.value(1);
		MMT.VariablesNUPD.precision.value(0.0001);
		MMT.VariablesNUPD.saveFile.value(1);
		double log2 = Math.log10(MMT.VariablesNUPD.beanRadiuPixel.value()*2 - 1)/Math.log10(2);
		MMT.maxN = (int) Math.pow(2, Math.floor(log2)+1);
		List<RoiItem> rt = Collections.synchronizedList(new ArrayList<RoiItem>());
		rt.add(RoiItem.createInstance(new double[]{130,130,0}));
		rt.add(RoiItem.createInstance(new double[]{100,100,0}));
		Function.getInstance(null, rt);

		Kernel kl = new Kernel(rt);

		kl.imageWidth = 300;
		kl.imageHeight = 300;

		int bitDepth = 16;
		boolean flag = false;
		int calRange = 10;

		if(flag)
			for (int i = 0; i < calRange ; i++) {
				Object image = getImg(i+1,bitDepth);
				//MMT.tik();
				boolean ret = kl.getXYPosition(image);
				if(!ret)continue;
				//MMT.tok("getXYPosition");
				for (int k = 0; k < rt.size(); k++) {
					double[] xy = rt.get(k).getXY();
					double xpos = xy[0];
					double ypos = xy[1];
					double[] xyPhy = rt.get(k).getXYZPhy();
					double xphy = xyPhy[0];
					double yphy = xyPhy[1];
					System.out.print(String.format("\r\nx:\t%f\ty:%f xphy:\t%f\typhy:\t%f", xpos,ypos,xphy,yphy));

				}
			}

		if(!flag){
			for (int i = 0; i < rt.size(); i++) {
				rt.get(i).setChartVisible(true);
			}
			kl.updateCalibrationProfile();
			for (int i = 0; i < calRange; i++) {
				Object image = getImg(i+1,bitDepth);
				kl.calibration(image,i,150,150,i+1);
			}
			kl.isCalibrated_ = true;
			long frameNm = 0;
			Object image = getImg(8.5,bitDepth);
			Object image1 = getImg(3.5,bitDepth);
			Object[] img = new Object[]{image,image1};
			for (int jj = 0; jj < 10000; jj++) {

				for (int i = 0; i < calRange; i++) {
					frameNm++;
					MMT.tik();
					kl.getXYZPosition(img[(int) (frameNm%2)]);
					//					try {
					//						kl.saveRoiData("ACQ",frameNm,System.nanoTime()/10e6);
					//					} catch (IOException e) {
					//						MMT.logError("Save data error");
					//					}
					MMT.tok("getXYZPosition");

					Function.getInstance().updateChart(frameNm);

					for (int k = 0; k < rt.size(); k++) {
						double[] xyPhy = rt.get(k).getXYZPhy();
						double xphy = xyPhy[0];
						double yphy = xyPhy[1];
						double zphy = rt.get(k).getZ();
						System.out.print(String.format("\r\n\r\n[%d]\txphy:\t%.3f\typhy:\t%.3f\tzphy:\t%.3f\tzset:\t%.3f\tdelta:\t%f\t",frameNm,xphy,yphy,zphy,frameNm%2==0?8.5:3.5,zphy+frameNm%2+2.5));
					}

				}

			}
			try {
				kl.dataCleanUp();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}


	}


	private boolean dataCleanUp() throws IOException {
		for (int i = 0; i < roiList_.size(); i++) {
			roiList_.get(i).dataClean(false);
		}
		return true;
	}
	public boolean saveRoiData(String acqName,long frameNum, double elapsed) throws IOException {
		for (int i = 0; i < roiList_.size(); i++) {
			roiList_.get(i).writeData(acqName, frameNum, elapsed);
		}
		return true;
	}
	private static double[] zeroPadding(double[] signal) {
		double log2 = Math.log10(signal.length*2 - 1)/Math.log10(2);
		double[] ret = new double[(int) Math.pow(2, Math.floor(log2)+1)];
		for (int i = 0; i < signal.length; i++) {
			ret[i] = signal[i];
		}
		for (int i = signal.length; i < ret.length; i++) {
			ret[i] = 0.0;
		}
		return ret;
	} 
	@SuppressWarnings("unused")
	private double[] polarIntegral2(Object image,int x,int y, double beanRadiuPixel){

		int maxN = MMT.maxN;
		double[][] signal = new double[maxN][maxN];
		double [] temp1 = new double[maxN];
		double [] temp = new double[maxN/2];

		statis_.clear();
		switch(image.getClass().getName()){
		case "[D":
			break;
		case "[F":
			break;
		case "[S":
			for (int row=y;row<y+beanRadiuPixel*2;row++){
				for(int cloumn = x;cloumn<x+beanRadiuPixel*2;cloumn++){
					signal[row-y][cloumn-x] =  ((short[]) image)[cloumn + row* imageWidth];//get curve
				}

				Complex[] fRespns = FFT_.transform(signal[row-y], TransformType.FORWARD);//FFT
				for(int cloumn =0;cloumn<maxN;cloumn++)
					signal[row-y][cloumn] = fRespns[cloumn].getReal();//save FFT result
			}
			for(int cloumn = 0;cloumn<maxN/2;cloumn++){
				for(int row = 0;row<beanRadiuPixel*2;row++){
					temp1[row] = signal[row][cloumn];
				}
				Complex[] fRespns = FFT_.transform(temp1, TransformType.FORWARD);//FFT
				double v = fRespns[0].getReal();//save FFT result
				temp[cloumn] = v;
				statis_.addValue(v);
			}

			break;
		case "[B":
			break;
		}
		//normalization(temp,statis_);	
		temp[0]=0;
		return temp;
	}

	private double[] polarIntegral(Object image,double xpos,double ypos){

		double S00 = 0, S01 =0, S10 = 0, S11 =0;				 
		double beanRadiuPixel = MMT.VariablesNUPD.beanRadiuPixel.value();
		double rInterStep = MMT.VariablesNUPD.rInterStep.value();
		double skipRadius = MMT.VariablesNUPD.skipRadius.value();
		int skipStart  = (int) (skipRadius/rInterStep);
		int polarFactor = (int) MMT.VariablesNUPD.polarFactor.value();
		double xFactor = MMT.VariablesNUPD.xFactor.value();
		double yFactor = MMT.VariablesNUPD.yFactor.value();
		double[] profile = new double[(int) ((beanRadiuPixel-skipRadius)/rInterStep)];
		statis_.clear();
		switch(image.getClass().getName()){
		case "[D":
			profile[0] = ((double[]) image)[(int)xpos + ((int)ypos)* imageWidth];
			for(int i = skipStart;i< beanRadiuPixel/rInterStep ;i++)
			{
				double sumr = 0;
				double r =i* rInterStep;
				double dTheta = polarFactor/r;
				int nTheta =(int) (2*3.141592653579/dTheta);
				for(int j = 0;j<nTheta;j++)
				{

					double x = (xpos+ xFactor*r*Math.cos(dTheta*j));
					double y = (ypos+ yFactor*r*Math.sin(dTheta*j));
					int x0 = (int)x;
					int y0 = (int)y;
					int x1 = x0 +1;
					int y1 = y0 +1;
					double dx = x - x0;
					double dy = y - y0;

					S00 = ((double[]) image)[x0 + y0* imageWidth];
					S01 = ((double[]) image)[x1 + y0* imageWidth];
					S10 = ((double[]) image)[x0 + y1* imageWidth];
					S11 = ((double[]) image)[x1 + y1* imageWidth];
					double Sxy = S00*(1-dx)*(1-dy)+S01*dy*(1-dx)+S10*dx*(1-dy) +S11*dx*dy;
					sumr += Sxy;
				}
				profile[i-skipStart] =sumr/nTheta;
				statis_.addValue(profile[i-skipStart]);
			}
			break;
		case "[F":
			profile[0] = ((float[]) image)[(int)xpos + ((int)ypos)* imageWidth];
			for(int i = skipStart;i< beanRadiuPixel/rInterStep ;i++)
			{
				double sumr = 0;
				double r =i*rInterStep;
				double dTheta = polarFactor/r;
				int nTheta =(int) (2*3.141592653579/dTheta);
				for(int j = 0;j<nTheta;j++)
				{

					double x = (xpos+xFactor*r*Math.cos(dTheta*j));
					double y = (ypos+yFactor*r*Math.sin(dTheta*j));
					int x0 = (int)x;
					int y0 = (int)y;
					int x1 = x0 +1;
					int y1 = y0 +1;
					double dx = x - x0;
					double dy = y - y0;

					S00 = ((float[]) image)[x0 + y0* imageWidth];
					S01 = ((float[]) image)[x1 + y0* imageWidth];
					S10 = ((float[]) image)[x0 + y1* imageWidth];
					S11 = ((float[]) image)[x1 + y1* imageWidth];
					double Sxy = S00*(1-dx)*(1-dy)+S01*dy*(1-dx)+S10*dx*(1-dy) +S11*dx*dy;
					sumr += Sxy;
				}
				profile[i-skipStart] =sumr/nTheta;
				statis_.addValue(profile[i-skipStart]);
			}
			break;
		case "[S":
			profile[0] = ((short[]) image)[(int)xpos + ((int)ypos)* imageWidth];
			statis_.addValue(profile[0]);
			for(int i = skipStart+1;i<((int) (beanRadiuPixel/rInterStep)) ;i++)
			{
				double sumr = 0;
				double r =i*rInterStep;
				double dTheta = polarFactor/r;
				int nTheta =(int) (2*3.141592653579/dTheta);
				nTheta = (nTheta==0)?1:nTheta;
				for(int j = 0;j<nTheta;j++)
				{


					double x = (xpos+xFactor*r*Math.cos(dTheta*j));
					double y = (ypos+yFactor*r*Math.sin(dTheta*j));
					int x0 = (int)x;
					int y0 = (int)y;
					int x1 = x0 +1;
					int y1 = y0 +1;
					double dx = x - x0;
					double dy = y - y0;

					S00 = ((short[]) image)[x0 + y0* imageWidth];
					S01 = ((short[]) image)[x1 + y0* imageWidth];
					S10 = ((short[]) image)[x0 + y1* imageWidth];
					S11 = ((short[]) image)[x1 + y1* imageWidth];
					//((short[]) image)[x0 + y0* imageWidth] = 5555;
					double Sxy = S00*(1-dx)*(1-dy)+S01*dy*(1-dx)+S10*dx*(1-dy) +S11*dx*dy;
					sumr += Sxy;
				}
				profile[i-skipStart] =sumr/nTheta;
				statis_.addValue(profile[i-skipStart]);
			}
			break;
		case "[B":
			profile[0] = ((byte[]) image)[(int)xpos + ((int)ypos)* imageWidth];
			for(int i = skipStart+1;i< beanRadiuPixel/rInterStep ;i++)
			{
				double sumr = 0;
				double r =i*rInterStep;
				double dTheta = polarFactor/r;
				int nTheta =(int) (2*3.141592653579/dTheta);
				for(int j = 0;j<nTheta;j++)
				{

					double x = (xpos+xFactor*r*Math.cos(dTheta*j));
					double y = (ypos+yFactor*r*Math.sin(dTheta*j));
					int x0 = (int)x;
					int y0 = (int)y;
					int x1 = x0 +1;
					int y1 = y0 +1;
					double dx = x - x0;
					double dy = y - y0;

					S00 = ((byte[]) image)[x0 + y0* imageWidth];
					S01 = ((byte[]) image)[x1 + y0* imageWidth];
					S10 = ((byte[]) image)[x0 + y1* imageWidth];
					S11 = ((byte[]) image)[x1 + y1* imageWidth];
					double Sxy = S00*(1-dx)*(1-dy)+S01*dy*(1-dx)+S10*dx*(1-dy) +S11*dx*dy;
					sumr += Sxy;
				}
				profile[i-skipStart] =sumr/nTheta;
				statis_.addValue(profile[i-skipStart]);
			}
			break;
		}
		normalization(profile,statis_);	
		return profile;
	}

	public void updateCalibrationProfile(){
		double calRange = MMT.VariablesNUPD.calRange.value();
		double calStepSize = MMT.VariablesNUPD.calStepSize.value();

		zPosProfiles = new double[ (int) (calRange/calStepSize)];
		xPosProfiles = new double[ (int) (calRange/calStepSize)];
		yPosProfiles = new double[ (int) (calRange/calStepSize)];
		for (RoiItem it:roiList_)
			it.InitializeCalProflie((int) (calRange/calStepSize),MMT.maxN/2);
	}

	public  boolean calibration(Object image,int index,double currXPos,double currYPos,double currZPos) {
		double[][] ret = gosseCenter(image);
		if(ret == null)
			return false;
		if(index == 0){
			regrX.clear();
			regrX.clear();
		}
		double[] xy = ret[0];
		regrX.addData(xy[0],currXPos);
		regrY.addData(xy[1],currYPos);
		zPosProfiles [index] = currZPos;
		double beanRadiuPixel = MMT.VariablesNUPD.beanRadiuPixel.value();
		for (int k = 0; k < roiList_.size(); k++) {
			int roiX = (int) (ret[k][0] - beanRadiuPixel);
			int roiY = (int) (ret[k][1] - beanRadiuPixel);
			if(isRoiOutOfImage(roiX,roiY)){
				roiList_.remove(k);
				return false;
			} 
			roiList_.get(k).setXY(ret[k][0],ret[k][1]);
			//roiList_.get(k).updateCalProfile(index,polarIntegral2(image,roiX,roiY,beanRadiuPixel));
			xy = roiList_.get(k).getXY();
			double[] posPofile = polarIntegral(image,xy[0],xy[1]);
			roiList_.get(k).updateCalProfile(index,posPofile);
			Matrix A = new Matrix(new double[][]{posPofile});
			SingularValueDecomposition s = A.svd();
			double[] svd = s.getSingularValues();
			roiList_.get(k).getCalProfileNorm()[index]=svd[0];
		}
		return true;
	}
	private double dot(double[] posProfileStart, double[] posProfileEnd) {
		double temp=0;
		for (int i = 0; i < posProfileEnd.length; i++) {
			temp += posProfileStart[i]*posProfileEnd[i];
		}
		return temp;
	}

	private double getZLocation(int roiIndex, double[] currrProfiles) {
		double max = 0;
		int index = 0;
		double pos = 0;
		double[][] calProfile = roiList_.get(roiIndex).getCalProfile();
		double[] yArray = new double[calProfile.length];
		for (int j = 0; j < yArray.length; j++) {//range
			double value = pearCorrelation_.correlation(currrProfiles, calProfile[j]);
			yArray[j] = value;
			if(value>max){
				max = value;
				index = j;
			}
		}
		//new
		if(MMT.VariablesNUPD.pTerm_z.value()>2){
		
		int start = index-1;
		int end = index+1;
		if (start<0)start=0;
		if (end>calProfile.length-1)end=calProfile.length-1;

		double normStart = roiList_.get(roiIndex).getCalProfileNorm()[start];
		double normEnd = roiList_.get(roiIndex).getCalProfileNorm()[end];
		double[] posProfileStart = roiList_.get(roiIndex).getCalProfile()[start];
		double[] posProfileEnd = roiList_.get(roiIndex).getCalProfile()[end];
	 	double dotAll = dot(posProfileStart,posProfileEnd);
		double dot = dot(posProfileStart,currrProfiles);

		Matrix A = new Matrix(new double[][]{currrProfiles});
		SingularValueDecomposition s = A.svd();
		double[] svd = s.getSingularValues();
		double normNow = svd[0];

		double phaseShiftAll = Math.acos(dotAll/(normStart*normEnd));
		double phaseShiftStart2Now = Math.acos(dot/(normStart*normNow));
		double posStart = zPosProfiles[start];
		double posEnd = zPosProfiles[end];
		pos = posStart+(posEnd-posStart)*phaseShiftStart2Now/phaseShiftAll;
		
		}else{
		
		//old 
				UnivariateFunction function = interpolator.interpolate(zPosProfiles, yArray);
				pos = zPosProfiles[index];
				double pos0 = zPosProfiles[0];
				double posEnd = zPosProfiles[zPosProfiles.length-1];
				max = 0;
				double start,end;
		
				for(double inc = 0.1;inc>MMT.VariablesNUPD.precision.value();inc =inc/10){
					start = pos-1;
					end = pos+1;
					if(start<pos0)start=pos0;
					if(end>posEnd)end=posEnd;
					for (double i = start; i < end; i+=inc) {
						double value  = function.value(i);
						if(value >max){
							max = value;
							pos = i;
						}
					}
				}
		}
		Function.getInstance().updateCorrChart(roiIndex, yArray);
		return pos;
	}


	public  void setPixelToPhys() {
		if(MMT.xyStage_ != null){
			MMT.VariablesNUPD.pixelToPhysX.value(regrX.getSlope());
			MMT.VariablesNUPD.pixelToPhysY.value(regrY.getSlope());
			MMTFrame.getInstance().preferDailog.saveUserData();
			MMTFrame.getInstance().preferDailog.UpdateData(false);
		}
	}
	public double[][] gosseCenter(Object image){
		int roiNum = roiList_.size();
		double beanRadiuPixel = MMT.VariablesNUPD.beanRadiuPixel.value();
		double[][] position = new double[roiNum][3];
		for (int i = 0; i < roiNum; i++) {

			double[] xy = roiList_.get(i).getXY();
			int roiX = (int) (xy[0] -beanRadiuPixel);
			int roiY = (int) (xy[1] -beanRadiuPixel);
			if(isRoiOutOfImage(roiX,roiY)){
				if(MMT.VariablesNUPD.AutoDeleteRoi.value() == 1){
					roiList_.remove(i);
					MMT.lastError_ = "Roi out of image,removed!";
					if(MMT.isCalibrationRunning_ || MMT.isTestingRunning_)return null;
					if(isCalibrated_ && (roiList_.size() == 0))
					{
						setIsCalibrated(false);
						MMT.lastError_ = "Roi out of image,removed,there is no roi in the image,new calibrateion is needed!";
						return null;
					}
					i--;
					roiNum --;
				}
				continue;
			}
			int crossSize = (int) MMT.VariablesNUPD.crossSize.value();
			//double[][] sumXY = getXYSum(image, roiX,roiY,crossSize);//0.02
			
			
			double xPos = 0;//getCurveCenter(sumXY[0])+ roiX;//0.02
			double yPos= 0;//getCurveCenter(sumXY[1])+ roiY;
			
			//获得选框内的灰度值经过xy方向相加后的二维数组信息
			double[][] sumXY = getXYSum(image, roiX,roiY,crossSize);
			Function.getInstance().updateChartSumXY(i, sumXY);
			//二维灰度值的边长
			int len=sumXY[0].length;
			//创建三个初始double数
			double bMA = 0;
			double cMA = 0;
			double averageX = 0;
			//x方向上的中心位置
			for(int ii=0; ii<len;ii++  )
			{
			bMA = bMA + sumXY[0][ii];
			cMA = cMA + (ii+1)*sumXY[0][ii];
			}
			averageX = (cMA / bMA) + (roiX-1); 
			//计算y方向上的平均位置
			//创建三个初始double数
			double eMA = 0;
			double fMA = 0;
			double averageY = 0;
			//y方向上的中心位置
			for(int ii=0; ii<len; ii++  )
			{
			eMA = eMA + sumXY[1][ii];
			fMA = fMA + (ii+1)*sumXY[1][ii];
			}
			averageY = (fMA / eMA) + (roiY-1); 
			
			double counter = sumXY[2][0];
			position[i][0] = averageX;
			position[i][1] = averageY;
			position[i][2] = counter;
		}
		//deBackground
		int bgIndex = Function.getInstance().getBackgroundRoiIndex();
		if(bgIndex != -1){
			double bgMeanGray = position[bgIndex][2];
			for (int i = 0; i < roiNum; i++)
			{
				if(i == bgIndex)
					continue;
				position[i][2] = position[i][2] - bgMeanGray;
			}
		}
		return position;
	}

	public void setIsCalibrated(boolean flag) {
		if(!flag){
			isCalibrated_ = false;
			Function.getInstance().installAnalyzer("XYACQ");
			Function.getInstance().liveView();
			MMTFrame.getInstance().setCalibrateIcon(false);
			MMTFrame.getInstance().setFeedbackIcon(false);
			MMTFrame.getInstance().setLiveViewIcon(false);
			MMTFrame.getInstance().preferDailog.enableEdit(true);
		}
	}
	private boolean isRoiOutOfImage(int roiX, int roiY) {
		int boder = 5;
		double beanRadiuPixel = MMT.VariablesNUPD.beanRadiuPixel.value();
		if(roiX< boder)
			return true;
		if(roiY< boder)
			return true;
		if(roiX+2*beanRadiuPixel >imageWidth - boder)
			return true;
		if(roiY+2*beanRadiuPixel >imageHeight - boder)
			return true;
		return false;
	}

	private double getCurveCenter(double[] curve) {
		double[] signal = zeroPadding(curve);
		Complex[] fRespns = FFT_.transform(signal, TransformType.FORWARD);
		for (int i = 0; i < fRespns.length; i++) {
			fRespns[i] = fRespns[i].multiply(fRespns[i]);
		}
		Complex[] convResult = FFT_.transform(fRespns, TransformType.INVERSE);
		int convLen = curve.length * 2 -1;
		double max = 0;
		int index = 0;

		double yArray[] = new double[convLen];
		double[] xArray = new double[convLen];

		for (int i = 0; i < convLen; i++) {
			xArray[i] = i;
			double value = convResult[i].getReal();
			yArray[i] = value;
			if(value > max){
				max = value;
				index = i;
			}
		}

		UnivariateFunction function = interpolator.interpolate(xArray, yArray);
		double pos = xArray[index];

		max = 0;
		double start,end,pos0,posEnd;

		pos0 = xArray[0];
		posEnd = xArray[convLen-1];
		for(double inc = 0.1;inc>MMT.VariablesNUPD.precision.value();inc =inc/10){
			start = pos-1;
			end = pos+1;
			if(start<pos0)start=pos0;
			if(end>posEnd)end=posEnd;
			for (double i = start; i < end; i+=inc) {
				double value  = function.value(i);
				if(value >max){
					max = value;
					pos = i;
				}
			}
		}

		return (pos+1)/2;
	}

	private double [][] getXYSum(Object image, int roiX,int roiY,int CrossSize) {

		int roiBorder = (int) (2*MMT.VariablesNUPD.beanRadiuPixel.value());
		int centerX = roiX + roiBorder/2;
		int centerY = roiY + roiBorder/2;
		double sumGrayValue = 0;
		int sRoi = roiBorder*roiBorder;
		double[][] sumXY = new double[3][roiBorder];
		sumX_.clear();
		sumY_.clear();
		switch(image.getClass().getName()){
		case "[D" :
			//x
			for (int x = roiX; x < roiX+roiBorder; x++) {
				for (int y = centerY-CrossSize/2; y < centerY+CrossSize/2; y++) {
					double gray = (double ) ((double[])image)[(y)*imageWidth +x];
					sumXY[0][x-roiX] += gray;
				}
				sumX_.addValue(sumXY[0][x-roiX]);
			}
			//y
			for (int y = roiY; y < roiY+roiBorder; y++) {
				for (int x = centerX-CrossSize/2; x < centerX+CrossSize/2; x++) {
					double gray = (double ) ((double[])image)[(y)*imageWidth +x];
					sumXY[1][y-roiY] += gray;
				}
				sumY_.addValue(sumXY[1][y-roiY]);
			}
			//intensity
			for (int x = 0; x < roiBorder; x++) {
				for (int y = 0; y < roiBorder; y++) {
					double gray = (double) ((double[])image)[(y+roiY)*imageWidth+(x+roiX)];
					sumGrayValue += gray/sRoi;
				}
				sumX_.addValue(sumXY[0][x]);
			}
			break;
		case "[F" :
			//x
			for (int x = roiX; x < roiX+roiBorder; x++) {
				for (int y = centerY-CrossSize/2; y < centerY+CrossSize/2; y++) {
					double gray = (double ) ((float[])image)[(y)*imageWidth +x];
					sumXY[0][x-roiX] += gray;
				}
				sumX_.addValue(sumXY[0][x-roiX]);
			}
			//y
			for (int y = roiY; y < roiY+roiBorder; y++) {
				for (int x = centerX-CrossSize/2; x < centerX+CrossSize/2; x++) {
					double gray = (double ) ((float[])image)[(y)*imageWidth +x];
					sumXY[1][y-roiY] += gray;
				}
				sumY_.addValue(sumXY[1][y-roiY]);
			}
			//intensity
			for (int x = 0; x < roiBorder; x++) {
				for (int y = 0; y < roiBorder; y++) {
					double gray = (double) ((float[])image)[(y+roiY)*imageWidth+(x+roiX)];
					sumGrayValue += gray/sRoi;
				}
				sumX_.addValue(sumXY[0][x]);
			}
			break;
		case "[S" :
			//x
			for (int x = roiX; x < roiX+roiBorder; x++) {
				for (int y = centerY-CrossSize/2; y < centerY+CrossSize/2; y++) {
					double gray = (double ) (0x0FFFF& ((short[])image)[(y)*imageWidth +x]);
					sumXY[0][x-roiX] += gray;
				}
				sumX_.addValue(sumXY[0][x-roiX]);
			}
			//y
			for (int y = roiY; y < roiY+roiBorder; y++) {
				for (int x = centerX-CrossSize/2; x < centerX+CrossSize/2; x++) {
					double gray = (double )(0x0FFFF&  ((short[])image)[(y)*imageWidth +x]);
					sumXY[1][y-roiY] += gray;
				}
				sumY_.addValue(sumXY[1][y-roiY]);
			}
			//intensity
			for (int x = 0; x < roiBorder; x++) {
				for (int y = 0; y < roiBorder; y++) {
					double gray = (double) (0x0FFFF& ((short[])image)[(y+roiY)*imageWidth+(x+roiX)]);
					sumGrayValue += gray/sRoi;
				}
				sumX_.addValue(sumXY[0][x]);
			}
			break;
		case "[B" :
			//x
			for (int x = roiX; x < roiX+roiBorder; x++) {
				for (int y = centerY-CrossSize/2; y < centerY+CrossSize/2; y++) {
					double gray = (double ) (0x0FF& ((byte[])image)[(y)*imageWidth +x]);
					sumXY[0][x-roiX] += gray;
				}
				sumX_.addValue(sumXY[0][x-roiX]);
			}
			//y
			for (int y = roiY; y < roiY+roiBorder; y++) {
				for (int x = centerX-CrossSize/2; x < centerX+CrossSize/2; x++) {
					double gray = (double ) (0x0FF& ((byte[])image)[(y)*imageWidth +x]);
					sumXY[1][y-roiY] += gray;
				}
				sumY_.addValue(sumXY[1][y-roiY]);
			}
			//intensity
			for (int x = 0; x < roiBorder; x++) {
				for (int y = 0; y < roiBorder; y++) {
					double gray = (double) (0x0FF& ((byte[])image)[(y+roiY)*imageWidth+(x+roiX)]);
					sumGrayValue += gray/sRoi;
				}
				sumX_.addValue(sumXY[0][x]);
			}
			break;
		}
		int radius = (int) MMT.VariablesNUPD.beanRadiuPixel.value();
		int skipRadius = (int) MMT.VariablesNUPD.skipRadius.value();
		for(int i=radius-skipRadius;i<radius+skipRadius;i++){
			sumXY[0][i] = 0;
			sumXY[1][i] = 0;
		}
		//normalization(sumXY[0], sumX_);
		//normalization(sumXY[1], sumY_);
		sumXY[2][0] = sumGrayValue;
		return sumXY;
	}

	private void normalization(double[] data,DescriptiveStatistics statis){
		double mean = statis.getMean();
		double std = statis.getStandardDeviation();
		std = std*std;
		for (int i = 0; i < data.length; i++) {
			data[i] = (data[i] - mean)/std;
		}
	} 

	private static Object getImg(Object nameext,int bitDepth)  {
		String[] img_ = getimgString(nameext);
		switch(bitDepth){
		case 8:
			byte[] imgb = new byte[img_.length];
			for (int i = 0; i < img_.length; i++) {
				imgb[i] = (byte) Double.parseDouble(img_[i]);
			}
			return imgb;	
		case 16:
			short[] imgs = new short[img_.length];
			for (int i = 0; i < img_.length; i++) {
				imgs[i] = (short) ((short) 4000*Double.parseDouble(img_[i]));
			}
			return imgs;	
		case 32:
			float[] imgf = new float[img_.length];
			for (int i = 0; i < img_.length; i++) {
				imgf[i] = (float) Double.parseDouble(img_[i]);
			}
			return imgf;	
		case 64:
			double[] imgd = new double[img_.length];
			for (int i = 0; i < img_.length; i++) {
				imgd[i] = Double.parseDouble(img_[i]);
			}
			return imgd;	
		}
		return null;

	}

	private static String[] getimgString(Object nameext)  {
		File imgFile = new File("F:/Development/SandBox/CalImages/img"+nameext+".txt");
		if(!imgFile.exists())
			return null;		
		try {
			BufferedReader in;

			in = new BufferedReader(new FileReader(imgFile));

			String line;
			if((line = in.readLine()) == null)
			{
				in.close();
				return null;
			}

			String[] temp = line.split(","); 

			in.close();
			return temp;
		} catch (FileNotFoundException e) {
			return null;
		} catch (IOException e) {
			return null;
		} 

	}

}
