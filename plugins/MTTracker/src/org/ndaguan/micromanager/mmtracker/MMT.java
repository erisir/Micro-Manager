package org.ndaguan.micromanager.mmtracker;

import java.awt.Color;

public class MMT {
	public static final String menuName = "MultZIndexMeasure";
	public static final String SIMPLE_ACQ = "Snap/Live Window";
	public static final String tooltipDescription = "MultZIndexMeasure";
	public static final int TCPIPPort = 50501;
	public static String DEFAULT_TITLE = "Magnetic Tweezers Images Analyzer(SM4.IOP.CAS.CN)";

	public static  String magnetXYstage_ = "MP285 XY Stage";
	public static  String magnetZStage_ = "MP285 Z Stage";
	public static  boolean debug = true;
	     
	public static  String xyStage_ = ""; 
	public static  String zStage_ = "";
	
	public static boolean isAnalyzerBusy_ = false;
	public static boolean isCalibrationRunning_ = false;
	public static boolean isTestingRunning_ = false;
	public static boolean isGetXYPositionRunning_ = false;
	public static boolean isGetXYZPositionRunning_ = false;
	
	public static Object Acqlock = 0;
	public static int calibrateIndex_ = 0;
	public static int testingIndex_ = 0;
	public static int currentframeIndex_ = 0;
	public static double[][] Coefficients = null;
	public static String lastError_ = "No Error";
	private static long timeStart;	
	
	public static String[] CHARTLIST = new String[]{
		"Chart-Z","Chart-X","Chart-Y","Chart-FX","Chart-FY","Chart-Testing","Chart-Cal-Pos","Chart-Corr","Chart-SumX","Chart-SumY"
	};

	public static void logError(String string) 
	{
		MMTFrame.getInstance().infomation_.setForeground(new Color(255,0,0));
		MMTFrame.getInstance().infomation_.setText("Error!\t"+string);
		System.out.print(String.format("Error!!!\t%s\r\n",string));
	}
	public static void tik() 
	{
		timeStart = System.nanoTime();
	}
	public static void tok(String mouldeName ) 
	{
		System.out.print(String.format("\r\nMoulde��%s��:\tcostTime:\t%f ms\t\n", mouldeName,(System.nanoTime()-timeStart)/10e6));
	}
	public static void debugError(String string) 
	{
		System.out.print(String.format("Error!!!\t%s\r\n",string));
	}
	public static void debugMSG(double[][] arry) 
	{
		for (int i = 0; i < arry.length; i++) {
			for (int j = 0; j < arry[0].length; j++) {
				
				System.out.print(String.format("%.6f,",arry[i][j]));
			}
			System.out.print("\r\n");
		}
	}
	public static void debugMSG(double[]arry) 
	{
			for (int j = 0; j < arry.length; j++) {
				
				System.out.print(String.format("%.6f,",arry[j]));
			}
			System.out.print("\r\n");
	}
	
	public static void logMessage(String string) 
	{
		MMTFrame.getInstance().infomation_.setForeground(new Color(0,0,0));
		MMTFrame.getInstance().infomation_.setText("Msg:\t"+string);
		System.out.print(String.format("Msg>>\t%s\r\n",string));
	}
	public static void debugMessage(String string) 
	{
		System.out.print(String.format("Msg>>%s\t\r\n",string));
	}
	
	public static int[] unEditAfterCalbration = new int[]{0,4,5,8};
	public static double currXP = 40;
	public static boolean isFeedbackRunning_ = false;
	protected static String currentUser = "n~daguan";
	public static boolean magnetCurrentStage= false;
	public static double magnetCurrentPosition = 0;
	public static double stageCurrentPosition = 0;
	public static int maxN=0;
	public static String AcqName = "Snap/Live Window";
	public static int calibrateSubIndex_ = 0;
	public static int TestingSubIndex_ = 0;
	public static double TestingCurrPosition = 0;
	public static enum VariablesClassify{
		General,
		DataSheet,
		Advance,
		Feedback,
		Debug;
	}
	public static enum VariablesNUPD {
		//constructor format:	unit,	default value,	precision,	importance,	toolTip,	classify
		beanRadiuPixel("/pixel ",55,0,1,"ѡ�еĿ��С���˷�Χ���ͼ�������Ч��̫С�򾫶Ȳ��ã�̫���˿��ܻᵼ�¶�λ��׼�ͼ����ʱ",VariablesClassify.General.name()),
		frameToCalcForce("/f ",300,0,1,"����֡�ƶ�һ�δ����������ʱ�Ƽ�ʹ��100+��Ҫ�������׼ȷ�������Ƽ�ʹ��1000+",VariablesClassify.General.name()),
		magnetStepSize("/uM ",100,0,1,"�ƶ�һ�δ����߹�ľ��룬̫��ʱ�ᵼ��MP285��Ӧ̫��",VariablesClassify.General.name()),
		chartWidth("",2000,0,1,"���ͼ�ĳ��ȣ��Ƽ�2000+",VariablesClassify.DataSheet.name()),
		
		calRange("/uM ",3,0.01,1,"�궨�ķ�Χ��̫������ӱ궨��ʱ������ѡ��2��DNA��������",VariablesClassify.General.name()),
		calStepSize("/uM ",0.1,0.01,1,"�궨�ľ��ȣ�ÿ������uM��¼һ���궨ֵ��̫�󾫶Ȳ��ã�̫С��ʱ���ӣ��Ƽ�0.01~0.1",VariablesClassify.General.name()),
		beanRadius("/uM ",1.4,0.001,1,"���������뾶�������������",VariablesClassify.General.name()),
		contourLen("/uM ",1,0.001,1,"DNA���ȣ������������",VariablesClassify.General.name()),
		//advance
		rInterStep("/pixel ",0.2,0.001,0,"�������ʱ���ڲ�ֵ��С��������¼���价��״��̫��ʱ���Ȳ��ã�̫Сʱ�����ʱ��ʹ��0.1ʱ������֪Bug���Ƽ�ʹ��Ĭ��ֵ",VariablesClassify.Advance.name()),
		persistance("/uM ",0.05,0.001,0,"DNA�նȣ���������������Ƽ�ʹ��Ĭ��ֵ",VariablesClassify.Advance.name()),
		kT("/pN*nM ",4.2,0.001,0,"Kb*T,��������������Ƽ�ʹ��Ĭ��ֵ",VariablesClassify.Advance.name()),
		precision("/uM",0.0001,0.0001,0,"��ֵ�㷨�еľ��ȣ����������ϵͳ������Ҫ����С���ȣ�̫��ʱ���Ȳ��ã�̫Сʱ�����ʱ���Ƽ�ʹ��Ĭ��ֵ",VariablesClassify.Advance.name()),
		 
		pixelToPhysX("(um/pixel) ",0.075,0.0001,0,"һ�����ض�Ӧ�������С��λ��̫�ɿ���XY�����ƶ�ʱ�������ã�������Ҫ��ݷŴ����CCD����ȷ��",VariablesClassify.Advance.name()),
		pixelToPhysY("(um/pixel) ",0.075,0.0001,0,"һ�����ض�Ӧ�������С��λ��̫�ɿ���XY�����ƶ�ʱ�������ã�������Ҫ��ݷŴ����CCD����ȷ��",VariablesClassify.Advance.name()),
		xFactor(" ",1,0.0001,0,"���价��X���������ϵ����CCD���ص������ʱʹ�ã���Ҫ�ο�CCD�ͺţ��Ƽ�ʹ��Ĭ��ֵ",VariablesClassify.Advance.name()),
		yFactor(" ",1,0.0001,0,"���价��Y���������ϵ����CCD���ص������ʱʹ�ã���Ҫ�ο�CCD�ͺţ��Ƽ�ʹ��Ĭ��ֵ",VariablesClassify.Advance.name()),
	 	 
		testingPrecision("",0.05,0.01,0,"����궨���ʱ�ľ��ȣ�ÿ������uM��һ�����飬̫С�����ʱ��̫���˾��Ȳ������Ƽ�ʹ��Ĭ��ֵ",VariablesClassify.Advance.name()),
		responceXY("",0,0,0,"����ר�ã��Ƿ��ڱ궨֮ǰ��¼����ʾ���´�����ݣ�1���ǣ�0����",VariablesClassify.Debug.name()),
		saveFile(" ",1,0,0,"����ר�ã��Ƿ񱣴����,1���ǣ�0����",VariablesClassify.Debug.name()),
		showDebugTime("",10000,0,0,"����ר�ã�����correlation �� posProfile ͼ���֡��",VariablesClassify.Debug.name()),
		
		chartStatisWindow("",1000,0,0,"���ͼ����ʾ����Ӧ�仯��֡��̫Сʱͼ�����׶�����̫��ʱͼ�������Զ����ţ��Ƽ�ʹ��200~1000",VariablesClassify.DataSheet.name()),
		frameToRefreshChart("",10,0,0,"���ͼ����ʾ������ͼ���֡��̫С�˼����ʱ��̫���˸�����,�Ƽ�ʹ��20~100",VariablesClassify.DataSheet.name()),
		frameToRefreshImage("",50,0,0,"ͼ����ʾ����Ӧ������ʱ�䣬̫С�˼����ʱ��̫������Ӧ�����׳���ѡ���������ƶ����Ƽ�ʹ��50~100",VariablesClassify.DataSheet.name()),
		stageMoveSleepTime("/ms",0,0,0,"λ��̨�ƶ��ȴ�ʱ�䣬̫С�˻ᵼ��λ��̨�ƶ�������Ҫλ�ã�̫���˺�ʱ���Ƽ��ο�λ��̨��Ϣ����ʹ��Ĭ��ֵ",VariablesClassify.Advance.name()),
		
		hasZStage("",1,0,0,"λ��̨�Ƿ���Կ�����Ʒ��Z�����ƶ���1���ǣ�0����",VariablesClassify.Debug.name()),
		hasXYStage("",0,0,0,"λ��̨�Ƿ���Կ�����Ʒ��XY�����ƶ���1���ǣ�0����",VariablesClassify.Debug.name()),
		needStageServer("",0,0,0,"�Ƿ���Ҫʹ��λ��̨��������1���ǣ�0����",VariablesClassify.Debug.name()),
		needCheckStageMovment("",0,0,0,"����λ��̨λ�ú��Ƿ���Ҫȷ�ϲŷ��أ�1���ǣ�0����",VariablesClassify.Debug.name()),
		stageMovmentPrecision("",0.02,0.001,0,"λ��̨�ƶ��������",VariablesClassify.Debug.name()),
		frameToFeedBack("",5,0,0,"����֡����һ��",VariablesClassify.Feedback.name()),
		feedBackMaxStepSize("/uM",0.02,0.001,0,"������󲽳�,ÿ�η����ߵ����λ�ƣ�̫���������𵴣�̫С������",VariablesClassify.Feedback.name()),
		feedBackMinStepSize("/uM",0.000,0.001,0,"������С��������Ʈ��С�ڴ�ֵʱ����������",VariablesClassify.Feedback.name()),
		feedBackWindowSize("",10,0,0,"�����������ڴ�С",VariablesClassify.Feedback.name()),
		pTerm_x("",-0.2,0.0001,0,"����ϵ��",VariablesClassify.Feedback.name()),
		needXYcalibrate("",0,0,0,"xy�����Ƿ���Ҫ�궨(����ȷ��һ�����ض�Ӧ����nm)��1���ǣ�0����",VariablesClassify.Advance.name()),
		crossSize("",20,0,0,"ʮ�֡����",VariablesClassify.Advance.name()),
		iTerm_x("",0.01,0.0001,0,"���ϵ��",VariablesClassify.Feedback.name()),
		pTerm_y("",-0.2,0.0001,0,"����ϵ��",VariablesClassify.Feedback.name()),
		iTerm_y("",0.01,0.0001,0,"���ϵ��",VariablesClassify.Feedback.name()),
		pTerm_z("",-0.2,0.0001,0,"����ϵ��",VariablesClassify.Feedback.name()),
		iTerm_z("",0.01,0.0001,0,"���ϵ��",VariablesClassify.Feedback.name()),
		XYMirror("",1,0,0,"ͼ���XY�����Ƿ���Ҫ������1���ǣ�0�����Ƽ�ʹ��Ĭ��ֵ",VariablesClassify.Feedback.name()),
		skipRadius("Pixel",12,0,0,"����ʱ���Ե������Ļ��뾶",VariablesClassify.Debug.name()),
		AutoRange("",1,0,0,"�Ƿ��Զ��ػ�ͼ�?1���ǣ�0����",VariablesClassify.DataSheet.name()),
		polarFactor("",1,0,0,"���ϵ��Խ��Խ��ϸ������ʱ��1���ǣ�0����",VariablesClassify.Debug.name()),
		AutoDeleteRoi("",1,0,0,"�Ƿ��Զ�ɾ��ROI��1���ǣ�0����",VariablesClassify.Debug.name()),
		GetZAlgorithm("",0,0,0,"��Zʹ�õ��㷨��1����0����أ�",VariablesClassify.Debug.name()),
		CalibrateTimes("",10,0,0,"��Zÿ��λ���ظ��������",VariablesClassify.General.name()),
		TestingTimes("",5,0,0,"��Zÿ��λ���ظ��������",VariablesClassify.General.name()),
		GetPosTimes("",2,0,0,"��Zÿ��λ���ظ��������",VariablesClassify.General.name());
		private String unit;
		private double value;
		private double presicion;
		private  int important;
		private String toolTip;
		private String classify;
		VariablesNUPD(String u,double v,double p,int i,String t,String c) {
			unit = u;
			value = v;
			presicion = p;
			important = i;
			toolTip = t;
			classify = c;
			
		}
	 
		public double value() {
			return value;
		}
		public void value(double v) {
			value = v;
		}
		public String getUnit() {
			return unit;
		}
		public double getPresicion() {
			return presicion;
		}
		public String getToolTip(){
			return toolTip;
		}
		public String getClassify(){
			return classify;
		}
		public int getImp() {
			return important;
		}

		public int getTabIndex() {
			VariablesClassify[] classifyArray = VariablesClassify.values();
			int classifyLen = classifyArray.length;
			for(int i =0;i<classifyLen;i++){
				if(classify.equals(classifyArray[i].name()))
						return i;
			}
			return -1;
		}
		
	}
	public static void SetCurrentStagePosition(String stageZLabel_,
			double target) {
			if(stageZLabel_.equals(magnetZStage_))
				magnetCurrentPosition = target;
			else
				stageCurrentPosition = target;
	};
}
