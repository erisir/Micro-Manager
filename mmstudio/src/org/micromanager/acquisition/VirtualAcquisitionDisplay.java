///////////////////////////////////////////////////////////////////////////////
//FILE:          VirtualAcquisitionDisplay.java
//PROJECT:       Micro-Manager
//SUBSYSTEM:     mmstudio
//-----------------------------------------------------------------------------
//
// AUTHOR:       Henry Pinkard, henry.pinkard@gmail.com, & Arthur Edelstein, 2010
//
// COPYRIGHT:    University of California, San Francisco, 2012
//
// LICENSE:      This file is distributed under the BSD license.
//               License text is included with the source distribution.
//
//               This file is distributed in the hope that it will be useful,
//               but WITHOUT ANY WARRANTY; without even the implied warranty
//               of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//               IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//               CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//               INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.
//
package org.micromanager.acquisition;

import org.micromanager.internalinterfaces.DisplayControls;
import org.micromanager.internalinterfaces.Histograms;
import java.lang.reflect.InvocationTargetException;
import ij.ImageStack;
import ij.process.LUT;
import ij.CompositeImage;
import ij.ImagePlus;
import ij.WindowManager;
import ij.gui.ImageCanvas;
import ij.gui.ImageWindow;
import ij.gui.ScrollbarWithLabel;
import ij.gui.StackWindow;
import ij.gui.Toolbar;
import ij.io.FileInfo;
import ij.measure.Calibration;
import ij.plugin.frame.ContrastAdjuster;
import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.TimerTask;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;
import java.util.prefs.Preferences;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import javax.swing.Timer;
import mmcorej.TaggedImage;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.micromanager.MMStudioMainFrame;
import org.micromanager.api.*;
import org.micromanager.graph.HistogramControlsState;
import org.micromanager.graph.MultiChannelHistograms;
import org.micromanager.graph.SingleChannelHistogram;
import org.micromanager.utils.*;

public final class VirtualAcquisitionDisplay implements AcquisitionDisplay, 
        ImageCacheListener, MMListenerInterface {

   public static VirtualAcquisitionDisplay getDisplay(ImagePlus imgp) {
      ImageStack stack = imgp.getStack();
      if (stack instanceof AcquisitionVirtualStack) {
         return ((AcquisitionVirtualStack) stack).getVirtualAcquisitionDisplay();
      } else {
         return null;
      }
   }
   private final static int SLOW_UPDATE_TIME = 1500;
   final static Color[] rgb = {Color.red, Color.green, Color.blue};
   final static String[] rgbNames = {"Red", "Blue", "Green"};
   final ImageCache imageCache_;
   final Preferences prefs_ = Preferences.userNodeForPackage(this.getClass());
   private static final String SIMPLE_WIN_X = "simple_x";
   private static final String SIMPLE_WIN_Y = "simple_y";
   private AcquisitionEngine eng_;
   private boolean finished_ = false;
   private boolean promptToSave_ = true;
   private String name_;
   private long lastDisplayTime_;
   private int lastFrameShown_ = 0;
   private int lastSliceShown_ = 0;
   private int lastPositionShown_ = 0;
   private int lockedSlice_ = -1;
   private int lockedPosition_ = -1;
   private int lockedChannel_ = -1;
   private int numComponents_;
   private ImagePlus hyperImage_;
   private ScrollbarWithLabel pSelector_;
   private ScrollbarWithLabel tSelector_;
   private ScrollbarWithLabel zSelector_;
   private ScrollbarWithLabel cSelector_;
   private DisplayControls controls_;
   public AcquisitionVirtualStack virtualStack_;
   private boolean simple_ = false;
   private boolean mda_ = false; //flag if display corresponds to MD acquisition
   private MetadataPanel mdPanel_;
   private boolean newDisplay_ = true; //used for autostretching on window opening
   private double framesPerSec_ = 7;
   private java.util.Timer zAnimationTimer_ = new java.util.Timer();
   private java.util.Timer tAnimationTimer_ = new java.util.Timer();
   private boolean zAnimated_ = false, tAnimated_ = false;
   private int animatedSliceIndex_ = -1;
   private Component zAnimationIcon_, pIcon_, tAnimationIcon_, cIcon_;
   private Component zLockIcon_, cLockIcon_, pLockIcon_;
   private HashMap<Integer, Integer> zStackMins_;
   private HashMap<Integer, Integer> zStackMaxes_;
   private Histograms histograms_;
   private HistogramControlsState histogramControlsState_;
   private boolean albumSaved_ = false;
   private static double snapWinMag_ = -1;
   private JPopupMenu saveTypePopup_;
   private int simpleWinImagesReceived_ = 0;
   private AtomicBoolean updatePixelSize_ = new AtomicBoolean(false);
   private AtomicLong newPixelSize_ = new AtomicLong();

   public void propertiesChangedAlert() {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   public void propertyChangedAlert(String device, String property, String value) {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   public void configGroupChangedAlert(String groupName, String newConfig) {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   public void pixelSizeChangedAlert(double newPixelSizeUm) {    
      // Signal that pixel size has changed so that the next image will update
      // metadata and scale bar
      newPixelSize_.set(Double.doubleToLongBits(newPixelSizeUm));
      updatePixelSize_.set(true);
   }

   public void stagePositionChangedAlert(String deviceName, double pos) {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   public void xyStagePositionChanged(String deviceName, double xPos, double yPos) {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   public void exposureChanged(String cameraName, double newExposureTime) {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   
   /**
    * This interface and the following two classes
    * allow us to manipulate the dimensions
    * in an ImagePlus without it throwing conniptions.
    */
   public interface IMMImagePlus {

      public int getNChannelsUnverified();

      public int getNSlicesUnverified();

      public int getNFramesUnverified();

      public void setNChannelsUnverified(int nChannels);

      public void setNSlicesUnverified(int nSlices);

      public void setNFramesUnverified(int nFrames);

      public void drawWithoutUpdate();
      
      public void updateAndDrawWithoutGUIUpdater();

   }

   public class MMCompositeImage extends CompositeImage implements IMMImagePlus {
      
      private GUIUpdater updater1 = new GUIUpdater(), updater2 = new GUIUpdater(), updater3 = new GUIUpdater();
      
      MMCompositeImage(ImagePlus imgp, int type) {
         super(imgp, type);
      }

      @Override
      public String getTitle() {
         return name_;
      }

      
      @Override
      public int getImageStackSize() {
         return super.nChannels * super.nSlices * super.nFrames;
      }

      @Override
      public int getStackSize() {
         return getImageStackSize();
      }

      @Override
      public int getNChannelsUnverified() {
         return super.nChannels;
      }

      @Override
      public int getNSlicesUnverified() {
         return super.nSlices;
      }

      @Override
      public int getNFramesUnverified() {
         return super.nFrames;
      }

      @Override
      public void setNChannelsUnverified(int nChannels) {
         super.nChannels = nChannels;
      }

      @Override
      public void setNSlicesUnverified(int nSlices) {
         super.nSlices = nSlices;
      }

      @Override
      public void setNFramesUnverified(int nFrames) {
         super.nFrames = nFrames;
      }

      private void superReset() {
         super.reset();
      }

      @Override
      public void reset() {
         if (SwingUtilities.isEventDispatchThread()) {
            super.reset();
         } else {
            SwingUtilities.invokeLater(new Runnable() {

               @Override
               public void run() {
                  superReset();
               }
            });
         }
      }

      @Override
      public synchronized void setMode(final int mode) {
               superSetMode(mode);
      }

      private void superSetMode(int mode) {
         super.setMode(mode);
      }

      @Override
      public synchronized void setChannelLut(final LUT lut) {
         superSetLut(lut);
      }

      private void superSetLut(LUT lut) {
         super.setChannelLut(lut);
      }

      @Override
      public synchronized void updateImage() {
           superUpdateImage();
      }

      private void superUpdateImage() {
         //Need to set this field to null, or else an infintie loop can be entered when 
         //The imageJ contrast adjuster is open
         ContrastAdjuster ca = (ContrastAdjuster) WindowManager.getFrame("B&C");
         if (ca != null) {
            try {
               JavaUtils.setRestrictedFieldValue(ca, ContrastAdjuster.class, "instance", null);
            } catch (NoSuchFieldException ex) {
               ReportingUtils.logError("ImageJ ContrastAdjuster doesn't have field named instance");
            }
         }
         
         super.updateImage();
      }
     


      private Runnable updateAndDrawRunnable() {
         return new Runnable() {

            @Override
            public void run() {
               superUpdateImage();
               imageChangedUpdate();
               try {
                  GUIUtils.invokeLater(new Runnable() {

                     @Override
                     public void run() {
                        try {
                           JavaUtils.invokeRestrictedMethod(this, ImagePlus.class, "notifyListeners", 2);
                        } catch (Exception ex) {
                        }
                        superDraw();
                        MMStudioMainFrame.getInstance().getLiveModeTimer().updateFPS();
                     }
                  });
               } catch (Exception e) {
                  ReportingUtils.logError(e);
               }
            }
         };
      }

      @Override
      public void updateAndDrawWithoutGUIUpdater() {
         try {
            GUIUtils.invokeLater(updateAndDrawRunnable());
         } catch (Exception e) {
            ReportingUtils.logError(e);
         }
      }
      
      @Override
      public void updateAndDraw() {         
          updater1.post(updateAndDrawRunnable());
      }
      
      private void superDraw() {
        if (super.win != null ) {
            super.getCanvas().paint(super.getCanvas().getGraphics());
         }
      }

      @Override
      public void draw() {
         Runnable runnable = new Runnable() {
             
            @Override
            public void run() {
               imageChangedUpdate();
               superDraw();
               MMStudioMainFrame.getInstance().getLiveModeTimer().updateFPS();
            }
         };
         updater2.post(runnable);
      }

      @Override
      public void drawWithoutUpdate() {
         Runnable runnable = new Runnable() {
            @Override
            public void run() {
               getWindow().getCanvas().setImageUpdated();
               superDraw();
            }           
         };
         updater3.post(runnable);
      }
   }

   public class MMImagePlus extends ImagePlus implements IMMImagePlus {

      private GUIUpdater updater1 = new GUIUpdater(), updater2 = new GUIUpdater();
      public VirtualAcquisitionDisplay display_;

      MMImagePlus(String title, ImageStack stack, VirtualAcquisitionDisplay disp) {
         super(title, stack);
         display_ = disp;
      }

      @Override
      public String getTitle() {
         return name_;
      }

      @Override
      public int getImageStackSize() {
         return super.nChannels * super.nSlices * super.nFrames;
      }

      @Override
      public int getStackSize() {
         return getImageStackSize();
      }

      @Override
      public int getNChannelsUnverified() {
         return super.nChannels;
      }

      @Override
      public int getNSlicesUnverified() {
         return super.nSlices;
      }

      @Override
      public int getNFramesUnverified() {
         return super.nFrames;
      }

      @Override
      public void setNChannelsUnverified(int nChannels) {
         super.nChannels = nChannels;
      }

      @Override
      public void setNSlicesUnverified(int nSlices) {
         super.nSlices = nSlices;
      }

      @Override
      public void setNFramesUnverified(int nFrames) {
         super.nFrames = nFrames;
      }

      private void superDraw() {
         if (super.win != null ) {
            super.getCanvas().paint(super.getCanvas().getGraphics());
         } 
      }
      
      private Runnable drawRunnable() {
         return new Runnable() {
            @Override
            public void run() {
               imageChangedUpdate();
               getWindow().getCanvas().setImageUpdated();
               superDraw();
               MMStudioMainFrame.getInstance().getLiveModeTimer().updateFPS();
            }       
         };
      }
      
      @Override
      public void updateAndDrawWithoutGUIUpdater() {
         try {
            GUIUtils.invokeLater(drawRunnable());
         } catch (Exception e) {
            ReportingUtils.logError(e);
         }
      }

      @Override
      public void draw() {        
         updater1.post(drawRunnable());
      }

      @Override
      public void drawWithoutUpdate() {
         Runnable runnable = new Runnable() {
            @Override
            public void run() {
               //ImageJ requires (this
               getWindow().getCanvas().setImageUpdated();
               superDraw();
            }
         };
         updater2.post(runnable);
      }
   }

   public VirtualAcquisitionDisplay(ImageCache imageCache, AcquisitionEngine eng) {
      this(imageCache, eng, WindowManager.getUniqueName("Untitled"));
   }

   public VirtualAcquisitionDisplay(ImageCache imageCache, AcquisitionEngine eng, String name) {
      name_ = name;
      imageCache_ = imageCache;
      eng_ = eng;
      pSelector_ = createPositionScrollbar();
      mda_ = eng != null;
      zStackMins_ = new HashMap<Integer, Integer>();
      zStackMaxes_ = new HashMap<Integer, Integer>();
      this.albumSaved_ = imageCache.isFinished();
   }

   //used for snap and live
   public VirtualAcquisitionDisplay(ImageCache imageCache, String name) throws MMScriptException {
      simple_ = true;
      imageCache_ = imageCache;
      name_ = name;
      mda_ = false;
      this.albumSaved_ = imageCache.isFinished();
      MMStudioMainFrame.getInstance().addMMListener(this);
   }

   private void startup(JSONObject firstImageMetadata) {
//      EDTProfiler edtp = new EDTProfiler();
      mdPanel_ = MMStudioMainFrame.getInstance().getMetadataPanel();
      JSONObject summaryMetadata = getSummaryMetadata();
      int numSlices = 1;
      int numFrames = 1;
      int numChannels = 1;
      int numGrayChannels;
      int numPositions = 1;
      int width = 0;
      int height = 0;
      int numComponents = 1;
      try {
         if (firstImageMetadata != null) {
            width = MDUtils.getWidth(firstImageMetadata);
            height = MDUtils.getHeight(firstImageMetadata);
         } else {
            width = MDUtils.getWidth(summaryMetadata);
            height = MDUtils.getHeight(summaryMetadata);
         }
         numSlices = Math.max(summaryMetadata.getInt("Slices"), 1);
         numFrames = Math.max(summaryMetadata.getInt("Frames"), 1);
         int imageChannelIndex;
         try {
            imageChannelIndex = MDUtils.getChannelIndex(firstImageMetadata);
         } catch (Exception e) {
            imageChannelIndex = -1;
         }
         numChannels = Math.max(1 + imageChannelIndex,
                 Math.max(summaryMetadata.getInt("Channels"), 1));
         numPositions = Math.max(summaryMetadata.getInt("Positions"), 1);
         numComponents = Math.max(MDUtils.getNumberOfComponents(summaryMetadata), 1);
      } catch (Exception e) {
         ReportingUtils.showError(e);
      }
      numComponents_ = numComponents;
      numGrayChannels = numComponents_ * numChannels;

      if (imageCache_.getDisplayAndComments() == null || imageCache_.getDisplayAndComments().isNull("Channels")) {
         imageCache_.setDisplayAndComments(getDisplaySettingsFromSummary(summaryMetadata));
      }

      int type = 0;
      try {
         if (firstImageMetadata != null) {
            type = MDUtils.getSingleChannelType(firstImageMetadata);
         } else {
            type = MDUtils.getSingleChannelType(summaryMetadata);
         }
      } catch (Exception ex) {
         ReportingUtils.showError(ex, "Unable to determine acquisition type.");
      }
      virtualStack_ = new AcquisitionVirtualStack(width, height, type, null,
              imageCache_, numGrayChannels * numSlices * numFrames, this);
      if (summaryMetadata.has("PositionIndex")) {
         try {
            virtualStack_.setPositionIndex(MDUtils.getPositionIndex(summaryMetadata));
         } catch (Exception ex) {
            ReportingUtils.logError(ex);
         }
      }
      if (simple_) {
         controls_ = new SimpleWindowControls(this);
      } else {
         controls_ = new HyperstackControls(this);
      }
      hyperImage_ = createHyperImage(createMMImagePlus(virtualStack_),
              numGrayChannels, numSlices, numFrames, virtualStack_, controls_);

      applyPixelSizeCalibration(hyperImage_);


      histogramControlsState_ =  mdPanel_.getContrastPanel().createDefaultControlsState();
      makeHistograms();
      createWindow();
      //Make sure contrast panel sets up correctly here
      windowToFront();


      cSelector_ = getSelector("c");
      if (!simple_) {
         tSelector_ = getSelector("t");
         zSelector_ = getSelector("z");

         if (imageCache_.lastAcquiredFrame() > 0) {
            setNumFrames(1 + imageCache_.lastAcquiredFrame());
         } else {
            setNumFrames(1);
         }
         configureAnimationControls();
         setNumPositions(numPositions);
      }
      
      // Load contrast settigns if opening datset
      if (imageCache_.isFinished()) {
      }

      updateAndDraw(false);
      updateWindowTitleAndStatus();

      forcePainting();
   }

   /*
    * Set display to one of three modes:
    * ij.CompositeImage.COMPOSITE
    * ij.CompositeImage.GRAYSCALE
    * ij.CompositeImage.COLOR
    */
   public void setDisplayMode(int displayMode) {
      mdPanel_.getContrastPanel().setDisplayMode(displayMode);
   }

   ///////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////Scrollbars and animation controls section/////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////////////
   private void forcePainting() {
      Runnable forcePaint = new Runnable() {

         @Override
         public void run() {
            if (zAnimationIcon_ != null) {
               zAnimationIcon_.paint(zAnimationIcon_.getGraphics());
            }
            if (tAnimationIcon_ != null) {
               tAnimationIcon_.paint(tAnimationIcon_.getGraphics());
            }
            if (cIcon_ != null) {
               cIcon_.paint(cIcon_.getGraphics());
            }
            if (cLockIcon_ != null) {
               cLockIcon_.paint(cLockIcon_.getGraphics());
            }
            if (zLockIcon_ != null) {
               zLockIcon_.paint(zLockIcon_.getGraphics());
            }
            if (pLockIcon_ != null) {
               pLockIcon_.paint(pLockIcon_.getGraphics());
            }
            if (controls_ != null) {
               controls_.paint(controls_.getGraphics());
            }
            if (pIcon_ != null && pIcon_.isValid()) {
               pIcon_.paint(pIcon_.getGraphics());
            }
         }
      };

      try {
         GUIUtils.invokeAndWait(forcePaint);
      } catch (Exception ex) {
         ReportingUtils.logError(ex);
      }
   }

   private void animateSlices(final boolean animate) {
      if (!animate) {
         zAnimationTimer_.cancel();
         zAnimated_ = false;
         refreshScrollbarIcons();
         return;
      } else {
         if (lockedSlice_ != -1) {
            //turn off locked z if animating
            lockedSlice_ = -1;
         }
         zAnimationTimer_ = new java.util.Timer();
         animateFrames(false);
         final int slicesPerStep;
         long interval = (long) (1000.0 / framesPerSec_);
         if (interval < 33) {
            interval = 33;
            slicesPerStep = (int) Math.round(framesPerSec_*33.0/1000.0);
         } else {
            slicesPerStep = 1;
         }
         TimerTask task = new TimerTask() {
            @Override
            public void run() {
               int slice = hyperImage_.getSlice();
               if (slice >= zSelector_.getMaximum() - 1) {
                  hyperImage_.setPosition(hyperImage_.getChannel(), 1, hyperImage_.getFrame());
               } else {
                  hyperImage_.setPosition(hyperImage_.getChannel(), slice + slicesPerStep, hyperImage_.getFrame());
               }
            }
         };
         zAnimationTimer_.schedule(task, 0, interval);
         zAnimated_ = true;
         refreshScrollbarIcons();
      }
   }

   private void animateFrames(final boolean animate) {
      if (!animate) {
         tAnimationTimer_.cancel();
         tAnimated_ = false;
         refreshScrollbarIcons();
         return;
      } else {
         tAnimationTimer_ = new java.util.Timer();
         animateSlices(false);
         final int framesPerStep;
         long interval = (long) (1000.0 / framesPerSec_);
         if (interval < 33) {
            interval = 33;
            framesPerStep = (int) Math.round(framesPerSec_*33.0/1000.0);
         } else {
            framesPerStep = 1;
         }
         TimerTask task = new TimerTask() {
            @Override
            public void run() {
               int frame = hyperImage_.getFrame();
               int channel = lockedChannel_ == -1 ? hyperImage_.getChannel() : lockedChannel_;
               int slice = lockedSlice_ == -1 ? hyperImage_.getSlice() : lockedSlice_;
               if (frame >= tSelector_.getMaximum() - 1) {
                  hyperImage_.setPosition(channel,slice, 1);
               } else {
                  hyperImage_.setPosition(channel,slice, frame + framesPerStep);
               }
            }
         };
         tAnimationTimer_.schedule(task, 0, interval);
         tAnimated_ = true;
         refreshScrollbarIcons();
      }
   }

   private void refreshScrollbarIcons() {
      if (zAnimationIcon_ != null) {
         zAnimationIcon_.repaint();
      }
      if (tAnimationIcon_ != null) {
         tAnimationIcon_.repaint();
      }
      if (zLockIcon_ != null) {
         zLockIcon_.repaint();
      }
      if (cLockIcon_ != null) {
         cLockIcon_.repaint();
      }
      if (pLockIcon_ != null) {
         pLockIcon_.repaint();
      }
   }

   private void configureAnimationControls() {
      if (zAnimationIcon_ != null) {
         zAnimationIcon_.addMouseListener(new MouseListener() {
            public void mousePressed(MouseEvent e) {
               animateSlices(!zAnimated_);
            }
            public void mouseClicked(MouseEvent e) {}
            public void mouseReleased(MouseEvent e) {}
            public void mouseEntered(MouseEvent e) {}
            public void mouseExited(MouseEvent e) {}
         });
      }
      if (tAnimationIcon_ != null) {
         tAnimationIcon_.addMouseListener(new MouseListener() {

            @Override
            public void mousePressed(MouseEvent e) {
               animateFrames(!tAnimated_);
            }
            public void mouseClicked(MouseEvent e) {}
            public void mouseReleased(MouseEvent e) {}
            public void mouseEntered(MouseEvent e) {}
            public void mouseExited(MouseEvent e) {}
         });
      }
   }
   
      private void setNumPositions(int n) {
      if (simple_) {
         return;
      }
      pSelector_.setMinimum(0);
      pSelector_.setMaximum(n);
      ImageWindow win = hyperImage_.getWindow();
      if (n > 1 && pSelector_.getParent() == null) {
         win.add(pSelector_, win.getComponentCount() - 1);
      } else if (n <= 1 && pSelector_.getParent() != null) {
         win.remove(pSelector_);
      }
      win.pack();
   }

   private void setNumFrames(int n) {
      if (simple_) {
         return;
      }
      if (tSelector_ != null) {
         //ImageWindow win = hyperImage_.getWindow();
         ((IMMImagePlus) hyperImage_).setNFramesUnverified(n);
         tSelector_.setMaximum(n + 1);
         // JavaUtils.setRestrictedFieldValue(win, StackWindow.class, "nFrames", n);
      }
   }

   private void setNumSlices(int n) {
      if (simple_) {
         return;
      }
      if (zSelector_ != null) {
         ((IMMImagePlus) hyperImage_).setNSlicesUnverified(n);
         zSelector_.setMaximum(n + 1);
      }
   }

   private void setNumChannels(int n) {
      if (cSelector_ != null) {
         ((IMMImagePlus) hyperImage_).setNChannelsUnverified(n);
         cSelector_.setMaximum(1 + n);
      }
   }
 
   /**
    * If animation was running prior to showImage, restarts it with sliders at
    * appropriate positions
    */
   private void restartAnimationAfterShowing(int frame, int slice, boolean framesAnimated, boolean slicesAnimated) {
      if (framesAnimated) {
         hyperImage_.setPosition(hyperImage_.getChannel(), hyperImage_.getSlice(), frame+1);
         animateFrames(true);
      } else if (slicesAnimated) {
         hyperImage_.setPosition(hyperImage_.getChannel(), slice+1, hyperImage_.getFrame());
         animateSlices(true);
      }
   }

   private void resetScrollBarsToLockedPositions() {
      hyperImage_.setPosition(lockedChannel_ == -1 ? hyperImage_.getChannel() : lockedChannel_,
              lockedSlice_ == -1 ? hyperImage_.getSlice() : lockedSlice_,
              hyperImage_.getFrame());
      if (pSelector_ != null && lockedPosition_ > -1) {
         setPosition(lockedPosition_);
      }
   }

    private ScrollbarWithLabel getSelector(String label) {
      // label should be "t", "z", or "c"
      ScrollbarWithLabel selector = null;
      ImageWindow win = hyperImage_.getWindow();
      int slices = ((IMMImagePlus) hyperImage_).getNSlicesUnverified();
      int frames = ((IMMImagePlus) hyperImage_).getNFramesUnverified();
      int channels = ((IMMImagePlus) hyperImage_).getNChannelsUnverified();
      if (win instanceof StackWindow) {
         try {
            //ImageJ bug workaround
            if (frames > 1 && slices == 1 && channels == 1 && label.equals("t")) {
               selector = (ScrollbarWithLabel) JavaUtils.getRestrictedFieldValue((StackWindow) win, StackWindow.class, "zSelector");
            } else {
               selector = (ScrollbarWithLabel) JavaUtils.getRestrictedFieldValue((StackWindow) win, StackWindow.class, label + "Selector");
            }
         } catch (NoSuchFieldException ex) {
            selector = null;
            ReportingUtils.logError(ex);
         }
      }
      //replace default icon with custom one
      if (selector != null) {
         try {
            Component icon = (Component) JavaUtils.getRestrictedFieldValue(
                    selector, ScrollbarWithLabel.class, "icon");
            selector.remove(icon);
         } catch (NoSuchFieldException ex) {
            ReportingUtils.logError(ex);
         }
         ScrollbarAnimateIcon newIcon = new ScrollbarAnimateIcon(label.charAt(0), this);
         if (label.equals("z")) {
            zAnimationIcon_ = newIcon;
         } else if (label.equals("t")) {
            tAnimationIcon_ = newIcon;
         } else if (label.equals("c")) {
            cIcon_ = newIcon;
         }

         selector.add(newIcon, BorderLayout.WEST);
         if (!label.equalsIgnoreCase("t")) {
            addSelectorLockIcon(selector, label);
         }
         
         selector.invalidate();
         selector.validate();
      }
      return selector;
   }
   
   private void addSelectorLockIcon(ScrollbarWithLabel selector, final String label) {
      final ScrollbarLockIcon icon = new ScrollbarLockIcon(this, label);
      selector.add(icon, BorderLayout.EAST);
      if (label.equals("p")) {
         pLockIcon_ = icon;
      } else if (label.equals("z")) {
         zLockIcon_ = icon;
      } else if (label.equals("c")) {
         cLockIcon_ = icon;
      }
      icon.addMouseListener(new MouseListener() {

         public void mouseClicked(MouseEvent e) {
            if (label.equals("p")) {
               if (lockedPosition_ == -1) {
                  lockedPosition_ = pSelector_.getValue();
               } else {
                  lockedPosition_ = -1;
               }
            } else if (label.equals("z")) {
               if (lockedSlice_ == -1) {
                  if (isZAnimated()) {
                     animateSlices(false);
                  }
                  lockedSlice_ = zSelector_.getValue();
               } else {
                  lockedSlice_ = -1;
               }
            } else if (label.equals("c")) {
               if (lockedChannel_ == -1) {
                  lockedChannel_ = cSelector_.getValue();
               } else {
                  lockedChannel_ = -1;
               }
            }
            resetScrollBarsToLockedPositions();
            refreshScrollbarIcons();
         }
         public void mousePressed(MouseEvent e) {}
         public void mouseReleased(MouseEvent e) { }
         public void mouseEntered(MouseEvent e) {}
         public void mouseExited(MouseEvent e) {}  
      }); 
   }
   
   boolean isScrollbarLocked(String label) {
      if (label.equals("z")) {
         return lockedSlice_ > -1;
      } else if (label.equals("c")) {
         return lockedChannel_ > -1;
      } else {
         return lockedPosition_ > -1;
      }
   }

   private ScrollbarWithLabel createPositionScrollbar() {
      final ScrollbarWithLabel pSelector = new ScrollbarWithLabel(null, 1, 1, 1, 2, 'p') {

         @Override
         public void setValue(int v) {
            if (this.getValue() != v) {
               super.setValue(v);
               updatePosition(v);
            }
         }
      };

      // prevents scroll bar from blinking on Windows:
      pSelector.setFocusable(false);
      pSelector.setUnitIncrement(1);
      pSelector.setBlockIncrement(1);
      pSelector.addAdjustmentListener(new AdjustmentListener() {

         @Override
         public void adjustmentValueChanged(AdjustmentEvent e) {
            updatePosition(pSelector.getValue());
         }
      });

      if (pSelector != null) {
         try {
            Component icon = (Component) JavaUtils.getRestrictedFieldValue(
                    pSelector, ScrollbarWithLabel.class, "icon");
            pSelector.remove(icon);
         } catch (NoSuchFieldException ex) {
            ReportingUtils.logError(ex);
         }

         pIcon_ = new ScrollbarAnimateIcon('p', this);
         pSelector.add(pIcon_, BorderLayout.WEST);
         addSelectorLockIcon(pSelector, "p");
         pSelector.invalidate();
         pSelector.validate();
      }

      return pSelector;
   }
   ////////////////////////////////////////////////////////////////////////////////
   ////////End of animation controls and scrollbars section///////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   
   /**
    * Allows bypassing the prompt to Save
    * @param promptToSave boolean flag
    */
   public void promptToSave(boolean promptToSave) {
      promptToSave_ = promptToSave;
   }

   /*
    * Method required by ImageCacheListener
    */
   @Override
   public synchronized void imageReceived(final TaggedImage taggedImage) {    
      updateDisplay(taggedImage, false);
   }

   /*
    * Method required by ImageCacheListener
    */
   @Override
   public void imagingFinished(String path) {
      updateDisplay(null, true);
      updateAndDraw();
      if (!(eng_ != null && eng_.abortRequested())) {
         updateWindowTitleAndStatus();
      }
   }

   private void updateDisplay(TaggedImage taggedImage, boolean finalUpdate) {
      try {
         long t = System.currentTimeMillis();
         JSONObject tags;
         if (taggedImage != null) {
            tags = taggedImage.tags;
         } else {
            tags = imageCache_.getLastImageTags();
         }
         if (tags == null) {
            return;
         }
         int frame = MDUtils.getFrameIndex(tags);
         int ch = MDUtils.getChannelIndex(tags);
         int slice = MDUtils.getSliceIndex(tags);
         int position = MDUtils.getPositionIndex(tags);
         boolean slowUpdates;
         if (histogramControlsState_ == null) {
            slowUpdates = false;
         } else {
            slowUpdates = histogramControlsState_.slowDisplayUpdates;
         }
         int updateTime = slowUpdates ? SLOW_UPDATE_TIME : 30;
         //update display if: final update, frame is 0, more than 30 ms since last update, 
         //last channel for given frame/slice/position, or final slice and channel for first frame and position
         boolean show = finalUpdate || frame == 0 || (Math.abs(t - lastDisplayTime_) > updateTime)
                 || (!slowUpdates && ch == getNumChannels() - 1 && lastFrameShown_ == frame && lastSliceShown_ == slice && lastPositionShown_ == position)
                 || (slice == getNumSlices() - 1 && frame == 0 && position == 0 && ch == getNumChannels() - 1);
         if (slowUpdates && ch != getNumChannels() - 1) {
            //only do slowupdates when all channels present
            show = false;
         }

         if (show) {
            showImage(tags, true);
            lastFrameShown_ = frame;
            lastSliceShown_ = slice;
            lastPositionShown_ = position;
            lastDisplayTime_ = t;
            forceImagePaint();
         }
      } catch (Exception e) {
         ReportingUtils.logError(e);
      }
   }

   private void forceImagePaint() {
      hyperImage_.getWindow().getCanvas().paint(hyperImage_.getWindow().getCanvas().getGraphics());
   }

   public int rgbToGrayChannel(int channelIndex) {
      try {
         if (MDUtils.getNumberOfComponents(imageCache_.getSummaryMetadata()) == 3) {
            return channelIndex * 3;
         }
         return channelIndex;
      } catch (Exception ex) {
         ReportingUtils.logError(ex);
         return 0;
      }
   }

   public int grayToRGBChannel(int grayIndex) {
      try {
         if (imageCache_ != null) {
            if (imageCache_.getSummaryMetadata() != null)
            if (MDUtils.getNumberOfComponents(imageCache_.getSummaryMetadata()) == 3) {
               return grayIndex / 3;
            }
         }
         return grayIndex;
      } catch (Exception ex) {
         ReportingUtils.logError(ex);
         return 0;
      }
   }

   public static JSONObject getDisplaySettingsFromSummary(JSONObject summaryMetadata) {
      try {
         JSONObject displaySettings = new JSONObject();

         JSONArray chColors = MDUtils.getJSONArrayMember(summaryMetadata, "ChColors");
         JSONArray chNames = MDUtils.getJSONArrayMember(summaryMetadata, "ChNames");
         JSONArray chMaxes, chMins;
         if ( summaryMetadata.has("ChContrastMin")) {
            chMins = MDUtils.getJSONArrayMember(summaryMetadata, "ChContrastMin");
         } else {
            chMins = new JSONArray();
            for (int i = 0; i < chNames.length(); i++)
               chMins.put(0);
         }
         if ( summaryMetadata.has("ChContrastMax")) {
            chMaxes = MDUtils.getJSONArrayMember(summaryMetadata, "ChContrastMax");
         } else {
            int max = 65536;
            if (summaryMetadata.has("BitDepth"))
               max = (int) (Math.pow(2, summaryMetadata.getInt("BitDepth"))-1);
            chMaxes = new JSONArray();
            for (int i = 0; i < chNames.length(); i++)
               chMaxes.put(max);
         }
            

         int numComponents = MDUtils.getNumberOfComponents(summaryMetadata);

         JSONArray channels = new JSONArray();
         if (numComponents > 1) //RGB
         {
            int rgbChannelBitDepth;
            try {
               rgbChannelBitDepth = MDUtils.getBitDepth(summaryMetadata);
            } catch (Exception e) {
                rgbChannelBitDepth = summaryMetadata.getString("PixelType").endsWith("32") ? 8 : 16;
            }
            for (int k = 0; k < 3; k++) {
               JSONObject channelObject = new JSONObject();
               channelObject.put("Color", rgb[k].getRGB());
               channelObject.put("Name", rgbNames[k]);
               channelObject.put("Gamma", 1.0);
               channelObject.put("Min", 0);
               channelObject.put("Max", Math.pow(2, rgbChannelBitDepth) - 1);
               channels.put(channelObject);
            }
         } else {
            for (int k = 0; k < chNames.length(); ++k) {
               String name = (String) chNames.get(k);
               int color = 0;
               if (k < chColors.length())
                  color = chColors.getInt(k);
               int min = 0;
               if (k < chMins.length())
                  min = chMins.getInt(k);
               int max = chMaxes.getInt(0);
               if (k < chMaxes.length())
                  max = chMaxes.getInt(k);
               JSONObject channelObject = new JSONObject();
               channelObject.put("Color", color);
               channelObject.put("Name", name);
               channelObject.put("Gamma", 1.0);
               channelObject.put("Min", min);
               channelObject.put("Max", max);
               channels.put(channelObject);
            }
         }

         displaySettings.put("Channels", channels);

         JSONObject comments = new JSONObject();
         String summary = "";
         try {
            summary = summaryMetadata.getString("Comment");
         } catch (JSONException ex) {
            summaryMetadata.put("Comment", "");
         }
         comments.put("Summary", summary);
         displaySettings.put("Comments", comments);
         return displaySettings;
      } catch (Exception e) {
         ReportingUtils.showError("Summary metadata not found or corrupt.  Is this a Micro-Manager dataset?");
         return null;
      }
   }

   /**
    * Sets ImageJ pixel size calibration
    * @param hyperImage
    */
   private void applyPixelSizeCalibration(final ImagePlus hyperImage) {
      try {
         JSONObject summary = getSummaryMetadata();
         double pixSizeUm = summary.getDouble("PixelSize_um");
         if (pixSizeUm > 0) {
            Calibration cal = new Calibration();
            cal.setUnit("um");
            cal.pixelWidth = pixSizeUm;
            cal.pixelHeight = pixSizeUm;
            String intMs = "Interval_ms";
            if (summary.has(intMs))
               cal.frameInterval = summary.getDouble(intMs) / 1000.0;
            String zStepUm = "z-step_um";
            if (summary.has(zStepUm))
               cal.pixelDepth = summary.getDouble(zStepUm);
            hyperImage.setCalibration(cal);
         }
      } catch (JSONException ex) {
         // no pixelsize defined.  Nothing to do
      }
   }

   public ImagePlus getHyperImage() {
      return hyperImage_;
   }

   public int getStackSize() {
      if (hyperImage_ == null) {
         return -1;
      }
      int s = hyperImage_.getNSlices();
      int c = hyperImage_.getNChannels();
      int f = hyperImage_.getNFrames();
      if ((s > 1 && c > 1) || (c > 1 && f > 1) || (f > 1 && s > 1)) {
         return s * c * f;
      }
      return Math.max(Math.max(s, c), f);
   }

   private void imageChangedWindowUpdate() {
      if (hyperImage_ != null && hyperImage_.isVisible()) {
         TaggedImage ti = virtualStack_.getTaggedImage(hyperImage_.getCurrentSlice());
         if (ti != null) {
            controls_.newImageUpdate(ti.tags);
         }
      }
   }

   public void updateAndDraw() {
      updateAndDraw(true);
   }
   
   public void updateAndDraw(boolean useGUIUpdater) {
      if (hyperImage_ != null && hyperImage_.isVisible()) {
         if (!useGUIUpdater) {
            ((IMMImagePlus) hyperImage_).updateAndDrawWithoutGUIUpdater(); 
         } else {
            hyperImage_.updateAndDraw();
         }
      }
   }

   public void updateWindowTitleAndStatus() {
      if (simple_) {
          int mag = (int) (100 * hyperImage_.getCanvas().getMagnification());
         String title = hyperImage_.getTitle() + " ("+mag+"%)";
         hyperImage_.getWindow().setTitle(title);
         return;
      }
      if (controls_ == null) {
         return;
      }

      String status = "";
      final AcquisitionEngine eng = eng_;

      if (eng != null) {
         if (acquisitionIsRunning()) {
            if (!abortRequested()) {
               controls_.acquiringImagesUpdate(true);
               if (isPaused()) {
                  status = "paused";
               } else {
                  status = "running";
               }
            } else {
               controls_.acquiringImagesUpdate(false);
               status = "interrupted";
            }
         } else {
            controls_.acquiringImagesUpdate(false);
            if (!status.contentEquals("interrupted")) {
               if (eng.isFinished()) {
                  status = "finished";
                  eng_ = null;
               }
            }
         }
         status += ", ";
         if (eng.isFinished()) {
            eng_ = null;
            finished_ = true;
         }
      } else {
         if (finished_ == true) {
            status = "finished, ";
         }
         controls_.acquiringImagesUpdate(false);
      }
      if (isDiskCached() || albumSaved_) {
         status += "on disk";
      } else {
         status += "not yet saved";
      }

      controls_.imagesOnDiskUpdate(imageCache_.getDiskLocation() != null);
      String path = isDiskCached()
              ? new File(imageCache_.getDiskLocation()).getName() : name_;

      if (hyperImage_.isVisible()) {
         int mag = (int) (100 * hyperImage_.getCanvas().getMagnification());
         hyperImage_.getWindow().setTitle(path + " (" + status + ") (" + mag + "%)" );
      }

   }

   private void windowToFront() {
      if (hyperImage_ == null || hyperImage_.getWindow() == null) {
         return;
      }
      hyperImage_.getWindow().toFront();
   }

   /**
    * Displays tagged image in the multi-D viewer
    * Will wait for the screen update
    *      
    * @param taggedImg
    * @throws Exception 
    */
   public void showImage(TaggedImage taggedImg) throws Exception {
      showImage(taggedImg, true);
   }

   /**
    * Displays tagged image in the multi-D viewer
    * Optionally waits for the display to draw the image
    *     * 
    * @param taggedImg
    * @throws Exception 
    */
   public void showImage(TaggedImage taggedImg, boolean waitForDisplay) throws InterruptedException, InvocationTargetException {
      showImage(taggedImg.tags, waitForDisplay);
   }

   public void showImage(final JSONObject tags, boolean waitForDisplay) throws InterruptedException, InvocationTargetException {
      updateWindowTitleAndStatus();

      if (tags == null) {
         return;
      }

      if (hyperImage_ == null) {
         GUIUtils.invokeAndWait(new Runnable() {
            @Override
            public void run() {
               try {
                  startup(tags);
               } catch (Exception e) {
                  ReportingUtils.logError(e);
               }
            }
         });
      }

      int channel = 0, frame = 0, slice = 0, position = 0, superChannel = 0;
      try {
         frame = MDUtils.getFrameIndex(tags);
         slice = MDUtils.getSliceIndex(tags);
         channel = MDUtils.getChannelIndex(tags);
         position = MDUtils.getPositionIndex(tags);
         superChannel = this.rgbToGrayChannel(MDUtils.getChannelIndex(tags));
      } catch (Exception ex) {
         ReportingUtils.logError(ex);
      }
            
      //This block allows animation to be reset to where it was before images were added
      final boolean framesAnimated = isTAnimated(), slicesAnimated = isZAnimated();
      final int animatedFrameIndex = hyperImage_.getFrame();
      if (slice == 0)
         animatedSliceIndex_ = hyperImage_.getSlice();
      if (framesAnimated || slicesAnimated) {
         animateFrames(false);
         animateSlices(false);
      }

     
       //make sure pixels get properly set
       if (hyperImage_ != null && frame == 0) {
           IMMImagePlus img = (IMMImagePlus) hyperImage_;
           if (img.getNChannelsUnverified() == 1) {
               if (img.getNSlicesUnverified() == 1) {
                   hyperImage_.getProcessor().setPixels(virtualStack_.getPixels(1));
               }
           } else if (hyperImage_ instanceof MMCompositeImage) {
               //reset rebuilds each of the channel ImageProcessors with the correct pixels
               //from AcquisitionVirtualStack
               MMCompositeImage ci = ((MMCompositeImage) hyperImage_);
               ci.reset();
               //This line is neccessary for image processor to have correct pixels in grayscale mode
               ci.getProcessor().setPixels(virtualStack_.getPixels(ci.getCurrentSlice()));
           }
       } else if (hyperImage_ instanceof MMCompositeImage) {
           MMCompositeImage ci = ((MMCompositeImage) hyperImage_);
           ci.reset();
       }
        
             

      if (cSelector_ != null) {
         if (cSelector_.getMaximum() <= (1 + superChannel)) {
            this.setNumChannels(1 + superChannel);
            ((CompositeImage) hyperImage_).reset();
            //JavaUtils.invokeRestrictedMethod(hyperImage_, CompositeImage.class,
            //       "setupLuts", 1 + superChannel, Integer.TYPE);
         }
      }

      initializeContrast(channel, slice);

      if (!simple_) {
         if (tSelector_ != null) {
            if (tSelector_.getMaximum() <= (1 + frame)) {
               this.setNumFrames(1 + frame);
            }
         }
         if (position + 1 > getNumPositions()) {
            setNumPositions(position + 1);
         }
         setPosition(position);
         hyperImage_.setPosition(1 + superChannel, 1 + slice, 1 + frame);
      }

      boolean useGUIUpdater = frame != 0 || simple_;
      if (simple_) {
         simpleWinImagesReceived_++;
         //Make sure update and draw gets called without GUI updater to initilze new snap win correctly
         int numChannels;
         try {
            numChannels = MDUtils.getNumChannels(getSummaryMetadata());
         } catch (Exception e) {
            numChannels = 7;
         }
         if ( simpleWinImagesReceived_ <= numChannels) {
            useGUIUpdater = false;
         }
      }
      
      updateAndDraw(useGUIUpdater);
      restartAnimationAfterShowing(animatedFrameIndex, animatedSliceIndex_, framesAnimated, slicesAnimated);
      resetScrollBarsToLockedPositions();
      
      if (cSelector_ != null) {
         if (histograms_.getNumberOfChannels() < (1 + superChannel)) {
                  if (histograms_ != null) {
               histograms_.setupChannelControls(imageCache_);
            }
      }
      }
   }

   /*
    * Live/snap should load window contrast settings
    * MDA should autoscale on first image
    * Not called when opening a dataset because stored settings are loaded automatically
    */
   private void initializeContrast(final int channel, final int slice) {
      Runnable autoscaleOrLoadContrast = new Runnable() {

         @Override
         public void run() {
            if (!newDisplay_) {
               return;
            }
            if (simple_) { //Snap/live
               if (hyperImage_ instanceof MMCompositeImage
                       && ((MMCompositeImage) hyperImage_).getNChannelsUnverified() - 1 != channel) {
                  return;
               }
               loadSimpleWinContrastWithoutDraw();
            } else if (mda_) { //Multi D acquisition
               IMMImagePlus immImg = ((IMMImagePlus) hyperImage_);
               if (immImg.getNSlicesUnverified() > 1) {  //Z stacks
                  autoscaleOverStackWithoutDraw(hyperImage_, channel, slice, zStackMins_, zStackMaxes_);
                  if (channel != immImg.getNChannelsUnverified() - 1 || slice != immImg.getNSlicesUnverified() - 1) {
                     return;  //don't set new display to false until all channels autoscaled
                  }
               } else {  //No z stacks
                  if (channel +1 != getNumChannels())
                     return;
                  autoscaleWithoutDraw();                 
               }
            } else //Acquire button
            if (hyperImage_ instanceof MMCompositeImage) {
               if (((MMCompositeImage) hyperImage_).getNChannelsUnverified() - 1 != channel) {
                  return;
               }
               autoscaleWithoutDraw();
            } else {
               autoscaleWithoutDraw(); // else do nothing because contrast automatically loaded from cache
            }
            newDisplay_ = false;
         }
      };
      if (!SwingUtilities.isEventDispatchThread()) {
         SwingUtilities.invokeLater(autoscaleOrLoadContrast);
      } else {
         autoscaleOrLoadContrast.run();
      }
   }
   
   private void loadSimpleWinContrastWithoutDraw() {
      int n = getImageCache().getNumChannels();
      ContrastSettings c;
      for (int i = 0; i < n; i++) {
         c = MMStudioMainFrame.getInstance().loadSimpleContrastSettigns(getImageCache().getPixelType(), i);
         histograms_.setChannelContrast(i, c.min, c.max, c.gamma);
      }
      histograms_.applyLUTToImage();
      
   }

   private void autoscaleWithoutDraw() {
      if (histograms_ != null) {
         histograms_.calcAndDisplayHistAndStats(true);
         histograms_.autostretch();
         histograms_.applyLUTToImage();
      }
   }
   
   private void autoscaleOverStackWithoutDraw(ImagePlus img, int channel, int slice,
           HashMap<Integer, Integer> mins, HashMap<Integer, Integer> maxes) {
      int nChannels = ((VirtualAcquisitionDisplay.IMMImagePlus) img).getNChannelsUnverified();
      int bytes = img.getBytesPerPixel();
      int pixMin, pixMax;
      if (mins.containsKey(channel)) {
         pixMin = mins.get(channel);
         pixMax = maxes.get(channel);
      } else {
         pixMax = 0;
         pixMin = (int) (Math.pow(2, 8 * bytes) - 1);
      }
      int flatIndex = 1 + channel + slice * nChannels;
      if (bytes == 2) {
         short[] pixels = (short[]) img.getStack().getPixels(flatIndex);
         for (short value : pixels) {
            //unsign short
            int val = value & 0xffff;
            if (val < pixMin) {
               pixMin = val;
            }
            if (val > pixMax) {
               pixMax = val;
            }
         }
      } else if (bytes == 1) {
         byte[] pixels = (byte[]) img.getStack().getPixels(flatIndex);
         for (byte value : pixels) {
            //unsign byte
            int val = value & 0xff;
            if (val < pixMin) {
               pixMin = val;
            }
            if (val > pixMax) {
               pixMax = val;
            }
         }
      }

      //autoscale the channel
      histograms_.setChannelContrast(channel, pixMin, pixMax, 1.0);     
      histograms_.applyLUTToImage();
      

      mins.put(channel, pixMin);
      maxes.put(channel, pixMax);
   }

   private void updatePosition(int p) {
      if (simple_) {
         return;
      }
      virtualStack_.setPositionIndex(p);
      if (!hyperImage_.isComposite()) {
         Object pixels = virtualStack_.getPixels(hyperImage_.getCurrentSlice());
         hyperImage_.getProcessor().setPixels(pixels);
      } else {
         CompositeImage ci = (CompositeImage) hyperImage_;
         if (ci.getMode() == CompositeImage.COMPOSITE) {
            for (int i = 0; i < ((MMCompositeImage) ci).getNChannelsUnverified(); i++) {
               //Dont need to set pixels if processor is null because it will get them from stack automatically  
               if (ci.getProcessor(i + 1) != null)                
                  ci.getProcessor(i + 1).setPixels(virtualStack_.getPixels(ci.getCurrentSlice() - ci.getChannel() + i + 1));
            }
         }
         ci.getProcessor().setPixels(virtualStack_.getPixels(hyperImage_.getCurrentSlice()));
      }
      //need to call this even though updateAndDraw also calls it to get autostretch to work properly
      imageChangedUpdate();
      updateAndDraw();
   }

   public void setPosition(int p) {
      if (simple_) {
         return;
      }
      pSelector_.setValue(p);
   }

   public void setSliceIndex(int i) {
      if (simple_) {
         return;
      }
      final int f = hyperImage_.getFrame();
      final int c = hyperImage_.getChannel();
      hyperImage_.setPosition(c, i + 1, f);
   }

   public int getSliceIndex() {
      return hyperImage_.getSlice() - 1;
   }

   boolean pause() {
      if (eng_ != null) {
         if (eng_.isPaused()) {
            eng_.setPause(false);
         } else {
            eng_.setPause(true);
         }
         updateWindowTitleAndStatus();
         return (eng_.isPaused());
      }
      return false;
   }

   boolean abort() {
      if (eng_ != null) {
         if (eng_.abortRequest()) {
            updateWindowTitleAndStatus();
            return true;
         }
      }
      return false;
   }

   public boolean acquisitionIsRunning() {
      if (eng_ != null) {
         return eng_.isAcquisitionRunning();
      } else {
         return false;
      }
   }

   public long getNextWakeTime() {
      return eng_.getNextWakeTime();
   }

   public boolean abortRequested() {
      if (eng_ != null) {
         return eng_.abortRequested();
      } else {
         return false;
      }
   }

   private boolean isPaused() {
      if (eng_ != null) {
         return eng_.isPaused();
      } else {
         return false;
      }
   }

   public void albumChanged() {
      albumSaved_ = false;
   }
   
   private Class createSaveTypePopup() {
      if (saveTypePopup_ != null) {
         saveTypePopup_.setVisible(false);
         saveTypePopup_ = null;
      }
      final JPopupMenu menu = new JPopupMenu();
      saveTypePopup_ = menu;
      JMenuItem single = new JMenuItem("Save as separate image files");
      JMenuItem multi = new JMenuItem("Save as image stack file");
      JMenuItem cancel = new JMenuItem("Cancel");
      menu.add(single);
      menu.add(multi);
      menu.addSeparator();
      menu.add(cancel);
      final AtomicInteger ai = new AtomicInteger(-1);
      cancel.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            ai.set(0);
         }
      });
      single.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            ai.set(1);
         }
      });
      multi.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            ai.set(2);
         }
      });
      MouseListener highlighter = new MouseListener() {
         @Override
         public void mouseClicked(MouseEvent e) {
         }
         @Override
         public void mousePressed(MouseEvent e) {}
         @Override
         public void mouseReleased(MouseEvent e) {}
         @Override
         public void mouseEntered(MouseEvent e) {
            ((JMenuItem) e.getComponent()).setArmed(true);
         }
         @Override
         public void mouseExited(MouseEvent e) {
            ((JMenuItem) e.getComponent()).setArmed(false);
         }       
      };
      single.addMouseListener(highlighter);
      multi.addMouseListener(highlighter);
      cancel.addMouseListener(highlighter);  
      Point mouseLocation = MouseInfo.getPointerInfo().getLocation();
      menu.show(null, mouseLocation.x, mouseLocation.y);
      while (ai.get() == -1) {
         try {
            Thread.sleep(10);
         } catch (InterruptedException ex) {}
         if (!menu.isVisible()) {
            return null;
         }
      }
      menu.setVisible(false);
      saveTypePopup_ = null;
      if (ai.get() == 0) {
         return null;
      } else if (ai.get() == 1) {
         return TaggedImageStorageDiskDefault.class;
      } else {
         return TaggedImageStorageMultipageTiff.class;
      }  
   }

   boolean saveAs() {
      return saveAs(null,true);
   }

   boolean saveAs(boolean pointToNewStorage) {
      return saveAs(null, pointToNewStorage);
   }

   private boolean saveAs(Class storageClass, boolean pointToNewStorage) {
      if (eng_ != null && eng_.isAcquisitionRunning()) {
         JOptionPane.showMessageDialog(null, 
                 "Data can not be saved while acquisition is running.");
         return false;
      }
      if (storageClass == null) {
         storageClass = createSaveTypePopup();
      }
      if (storageClass == null) {
         return false;
      }
      String prefix;
      String root;
      for (;;) {
         File f = FileDialogs.save(hyperImage_.getWindow(),
                 "Please choose a location for the data set",
                 MMStudioMainFrame.MM_DATA_SET);
         if (f == null) // Canceled.
         {
            return false;
         }
         prefix = f.getName();
         root = new File(f.getParent()).getAbsolutePath();
         if (f.exists()) {
            ReportingUtils.showMessage(prefix
                    + " is write only! Please choose another name.");
         } else {
            break;
         }
      }

      try {
         if (getSummaryMetadata() != null) {
            getSummaryMetadata().put("Prefix", prefix);
         }
         TaggedImageStorage newFileManager =
                 (TaggedImageStorage) storageClass.getConstructor(
                 String.class, Boolean.class, JSONObject.class).newInstance(
                 root + "/" + prefix, true, getSummaryMetadata());
         if (pointToNewStorage) {
            albumSaved_ = true;
         }

         imageCache_.saveAs(newFileManager, pointToNewStorage);
      } catch (Exception ex) {
         ReportingUtils.showError(ex, "Failed to save file");
      }
      MMStudioMainFrame.getInstance().setAcqDirectory(root);
      updateWindowTitleAndStatus();
      return true;
   }

   final public MMImagePlus createMMImagePlus(AcquisitionVirtualStack virtualStack) {
      MMImagePlus img = new MMImagePlus(imageCache_.getDiskLocation(), virtualStack, this);
      FileInfo fi = new FileInfo();
      fi.width = virtualStack.getWidth();
      fi.height = virtualStack.getHeight();
      fi.fileName = virtualStack.getDirectory();
      fi.url = null;
      img.setFileInfo(fi);
      return img;
   }

   final public ImagePlus createHyperImage(MMImagePlus mmIP, int channels, int slices,
           int frames, final AcquisitionVirtualStack virtualStack,
           DisplayControls hc) {
      final ImagePlus hyperImage;
      mmIP.setNChannelsUnverified(channels);
      mmIP.setNFramesUnverified(frames);
      mmIP.setNSlicesUnverified(slices);
      if (channels > 1) {        
         hyperImage = new MMCompositeImage(mmIP, imageCache_.getDisplayMode());
         hyperImage.setOpenAsHyperStack(true);
      } else {
         hyperImage = mmIP;
         mmIP.setOpenAsHyperStack(true);
      }
      return hyperImage;
   }

   public void liveModeEnabled(boolean enabled) {
      if (simple_) {
         controls_.acquiringImagesUpdate(enabled);
      }
   }

   private void createWindow() {
      final DisplayWindow win = new DisplayWindow(hyperImage_);
      win.getCanvas().addMouseListener(new MouseListener() {

         @Override
         public void mouseClicked(MouseEvent me) {
         }

         //used to store preferred zoom
         @Override
         public void mousePressed(MouseEvent me) {
            if (Toolbar.getToolId() == 11) {//zoom tool selected
               storeWindowSizeAfterZoom(win);
            }
            updateWindowTitleAndStatus();
         }

         //updates the histogram after an ROI is drawn
         @Override
         public void mouseReleased(MouseEvent me) {
            hyperImage_.updateAndDraw();
         }

         @Override
         public void mouseEntered(MouseEvent me) {
         }

         @Override
         public void mouseExited(MouseEvent me) {
         }
      });

      win.setBackground(MMStudioMainFrame.getInstance().getBackgroundColor());
      MMStudioMainFrame.getInstance().addMMBackgroundListener(win);

      win.add(controls_);
      win.pack();

       if (simple_) {
           win.setLocation(prefs_.getInt(SIMPLE_WIN_X, 0), prefs_.getInt(SIMPLE_WIN_Y, 0));
       }

      //Set magnification
      zoomToPreferredSize(win);
      
   
      mdPanel_.displayChanged(win);
      imageChangedUpdate();
   }
   
   public void storeWindowSizeAfterZoom(ImageWindow win) {
      if (simple_) {
         snapWinMag_ = win.getCanvas().getMagnification();
      }
   }
   
   private void zoomToPreferredSize(DisplayWindow win) {
      Point location = win.getLocation();
      win.setLocation(new Point(0,0));
      
      double mag;
      if (simple_ && snapWinMag_ != -1) {
         mag = snapWinMag_;
      } else {
         mag = MMStudioMainFrame.getInstance().getPreferredWindowMag();
      }

      ImageCanvas canvas = win.getCanvas();
      if (mag < canvas.getMagnification()) {
         while (mag < canvas.getMagnification()) {
            canvas.zoomOut(canvas.getWidth() / 2, canvas.getHeight() / 2);
         }
      } else if (mag > canvas.getMagnification()) {

         while (mag > canvas.getMagnification()) {
            canvas.zoomIn(canvas.getWidth() / 2, canvas.getHeight() / 2);
         }
      }

      //Make sure the window is fully on the screen
      Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      Point newLocation = new Point(location.x,location.y);
      if (newLocation.x + win.getWidth() > screenSize.width && win.getWidth() < screenSize.width) {
          newLocation.x = screenSize.width - win.getWidth();
      }
      if (newLocation.y + win.getHeight() > screenSize.height && win.getHeight() < screenSize.height) {
          newLocation.y = screenSize.height - win.getHeight();
      }
      
      win.setLocation(newLocation);
   }

   public JSONObject getCurrentMetadata() {
      try {
         if (hyperImage_ != null) {
            TaggedImage image = virtualStack_.getTaggedImage(hyperImage_.getChannel()-1, hyperImage_.getSlice()-1, hyperImage_.getFrame()-1);
            if (image != null) {
               return image.tags;
            } else {
               return null;
            }
         } else {
            return null;
         }
      } catch (NullPointerException ex) {
         return null;
      }
   }

   public int getCurrentPosition() {
      return virtualStack_.getPositionIndex();
   }

   public int getNumSlices() {
      if (simple_) {
         return 1;
      }
      return ((IMMImagePlus) hyperImage_).getNSlicesUnverified();
   }

   public int getNumFrames() {
      if (simple_) {
         return 1;
      }
      return ((IMMImagePlus) hyperImage_).getNFramesUnverified();
   }

   public int getNumPositions() {
      if (simple_) {
         return 1;
      }
      return pSelector_.getMaximum();
   }

   public ImagePlus getImagePlus() {
      return hyperImage_;
   }

   public ImageCache getImageCache() {
      return imageCache_;
   }

   public ImagePlus getImagePlus(int position) {
      ImagePlus iP = new ImagePlus();
      iP.setStack(virtualStack_);
      iP.setDimensions(numComponents_ * getNumChannels(), getNumSlices(), getNumFrames());
      iP.setFileInfo(hyperImage_.getFileInfo());
      return iP;
   }

   public void setComment(String comment) throws MMScriptException {
      try {
         getSummaryMetadata().put("Comment", comment);
      } catch (Exception ex) {
         ReportingUtils.logError(ex);
      }
   }

   public final JSONObject getSummaryMetadata() {
      return imageCache_.getSummaryMetadata();
   }
   
   /*
   public final JSONObject getImageMetadata(int channel, int slice, int frame, int position) {
      return imageCache_.getImageTags(channel, slice, frame, position);
   }
    */

   public void close() {
      if (hyperImage_ != null) {
         hyperImage_.getWindow().windowClosing(null);
         hyperImage_.close();
      }
   }

   public synchronized boolean windowClosed() {
      if (hyperImage_ != null) {
         ImageWindow win = hyperImage_.getWindow();
         return (win == null || win.isClosed());
      }
      return true;
   }

   public void showFolder() {
      if (isDiskCached()) {
         try {
            File location = new File(imageCache_.getDiskLocation());
            if (JavaUtils.isWindows()) {
               Runtime.getRuntime().exec("Explorer /n,/select," + location.getAbsolutePath());
            } else if (JavaUtils.isMac()) {
               if (!location.isDirectory()) {
                  location = location.getParentFile();
               }
               Runtime.getRuntime().exec("open " + location.getAbsolutePath());
            }
         } catch (IOException ex) {
            ReportingUtils.logError(ex);
         }
      }
   }

   public void setPlaybackFPS(double fps) {
      framesPerSec_ = fps;
      if (zAnimated_) {
         animateSlices(false);
         animateSlices(true);
      } else if (tAnimated_) {
         animateFrames(false);
         animateFrames(true);
      }
   }

   public double getPlaybackFPS() {
      return framesPerSec_;
   }

   public boolean isZAnimated() {
      return zAnimated_;
   }

   public boolean isTAnimated() {
      return tAnimated_;
   }

   public boolean isAnimated() {
      return isTAnimated() || isZAnimated();
   }

   public String getSummaryComment() {
      return imageCache_.getComment();
   }

   public void setSummaryComment(String comment) {
      imageCache_.setComment(comment);
   }

   void setImageComment(String comment) {
      imageCache_.setImageComment(comment, getCurrentMetadata());
   }

   String getImageComment() {
      try {
         return imageCache_.getImageComment(getCurrentMetadata());
      } catch (NullPointerException ex) {
         return "";
      }
   }

   public boolean isDiskCached() {
      ImageCache imageCache = imageCache_;
      if (imageCache == null) {
         return false;
      } else {
         return imageCache.getDiskLocation() != null;
      }
   }

   public void show() {
      if (hyperImage_ == null) {
         try {
            GUIUtils.invokeAndWait(new Runnable() {

               @Override
               public void run() {
                  startup(null);
               }
            });
         } catch (Exception ex) {
            ReportingUtils.logError(ex);
         }

      }
      hyperImage_.show();
      hyperImage_.getWindow().toFront();
   }

   public int getNumChannels() {
      return ((IMMImagePlus) hyperImage_).getNChannelsUnverified();
   }

   public int getNumGrayChannels() {
      return getNumChannels();
   }

   public void setWindowTitle(String name) {
      name_ = name;
      updateWindowTitleAndStatus();
   }

   public boolean isSimpleDisplay() {
      return simple_;
   }

   public void displayStatusLine(String status) {
      controls_.setStatusLabel(status);
   }

   public void setChannelContrast(int channelIndex, int min, int max, double gamma) {
      histograms_.setChannelContrast(channelIndex, min, max, gamma);
      histograms_.applyLUTToImage();
      drawWithoutUpdate();
   }
   
   public void updateChannelNamesAndColors() {
      if (histograms_ != null && histograms_ instanceof MultiChannelHistograms) {
         ((MultiChannelHistograms) histograms_).updateChannelNamesAndColors();
      }
   }
   
   public void setChannelHistogramDisplayMax(int channelIndex, int histMax) {
      histograms_.setChannelHistogramDisplayMax(channelIndex, histMax);
   }

   /*
    * called just before image is drawn.  Notifies metadata panel to update
    * metadata or comments if this display is the active window.  Notifies histograms
    * that image is change to create appropriate LUTs and to draw themselves if this
    * is the active window
    */
   private void imageChangedUpdate() {
      if (updatePixelSize_.get()) {
         try {
            JSONObject summary = getSummaryMetadata();
            if (summary != null) {
               summary.put("PixelSize_um", Double.longBitsToDouble(newPixelSize_.get()));
            }
            if (hyperImage_ != null) {
               applyPixelSizeCalibration(hyperImage_);
            }
            
         } catch (JSONException ex) {
            ReportingUtils.logError("Error in imageCHangedUpdate in VirtualAcquisitionDisplay.java");
         }
         updatePixelSize_.set(false);
      }
      if (histograms_ != null) {
         histograms_.imageChanged();
      }      
      if (isActiveDisplay()) {         
         mdPanel_.imageChangedUpdate(this);
         mdPanel_.redrawSizeBar();
      }      
      imageChangedWindowUpdate(); //used to update status line

   }
   
   public boolean isActiveDisplay() {
      if (hyperImage_ == null || hyperImage_.getWindow() == null)
           return false;
       if (hyperImage_.getWindow() == mdPanel_.getCurrentWindow()  )
           return true;
       return false;
   }

   public void drawWithoutUpdate() {
      if (hyperImage_ != null) {
         ((IMMImagePlus) hyperImage_).drawWithoutUpdate();
      }
   }
   
   private void makeHistograms() {
      if (getNumChannels() == 1 )
           histograms_ = new SingleChannelHistogram(this);
       else
           histograms_ = new MultiChannelHistograms(this);
   }
   
   public Histograms getHistograms() {
       return histograms_;
   }
   
   public HistogramControlsState getHistogramControlsState() {
       return histogramControlsState_;
   }
   
   public void disableAutoStretchCheckBox() {
       if (isActiveDisplay() ) {
          mdPanel_.getContrastPanel().disableAutostretch();
       } else {
          histogramControlsState_.autostretch = false;
       }
   }
   
   /*
    * used to store contrast settings for snap live window
    */
   private void saveSimpleWinSettings() {
      ImageCache cache = getImageCache();
      String pixelType = cache.getPixelType();
      int numCh = cache.getNumChannels();
      for (int i = 0; i < numCh; i++) {
         int min = cache.getChannelMin(i);
         int max = cache.getChannelMax(i);
         double gamma = cache.getChannelGamma(i);
         MMStudioMainFrame.getInstance().saveSimpleContrastSettings(new ContrastSettings(min, max, gamma), i, pixelType);
      }
   }
   
   public ContrastSettings getChannelContrastSettings(int channel) {
      return histograms_.getChannelContrastSettings(channel);
   }
           

   public class DisplayWindow extends StackWindow {

      private boolean windowClosingDone_ = false;
      private boolean closed_ = false;

      public DisplayWindow(ImagePlus ip) {
         super(ip);
      }

      @Override
      public boolean close() {
         windowClosing(null);
         return closed_;
      }

      @Override
      public void windowClosing(WindowEvent e) {
         if (windowClosingDone_) {
            return;
         }

         if (eng_ != null && eng_.isAcquisitionRunning()) {
            if (!abort()) {
               return;
            }
         }

         if (imageCache_.getDiskLocation() == null && promptToSave_ && !albumSaved_) {
            String[] options = {"Save single","Save multi","No","Cancel"};
            int result = JOptionPane.showOptionDialog(this, "This data set has not yet been saved.  "
                    + "Do you want to save it?\nData can be saved as single-image files or multi-image files.",
                    "Micro-Manager",JOptionPane.DEFAULT_OPTION, JOptionPane.QUESTION_MESSAGE, null,
                    options, options[0]);

            if (result == 0) {                          
               if (!saveAs(TaggedImageStorageDiskDefault.class, true)) {
                  return;           
               }
            } else if (result == 1) {
               if (!saveAs(TaggedImageStorageMultipageTiff.class, true)) {
                  return;           
               }
            } else if (result == 3) {
               return;
            }
         }

         if (simple_ && hyperImage_ != null && hyperImage_.getWindow() != null && hyperImage_.getWindow().getLocation() != null) {
            Point loc = hyperImage_.getWindow().getLocation();
            prefs_.putInt(SIMPLE_WIN_X, loc.x);
            prefs_.putInt(SIMPLE_WIN_Y, loc.y);
            saveSimpleWinSettings();
         }

         if (imageCache_ != null) {
            imageCache_.close();
         }

         
         removeMeFromAcquisitionManager(MMStudioMainFrame.getInstance());
         
         if (!closed_) {
            try {
               super.close();
            } catch (NullPointerException ex) {
               ReportingUtils.showError(ex, "Null pointer error in ImageJ code while closing window");
            }
         }

         //Call this because for some reason WindowManager doesnt always fire
         mdPanel_.displayChanged(null);
         zAnimationTimer_.cancel();
         tAnimationTimer_.cancel();

         super.windowClosing(e);
         MMStudioMainFrame.getInstance().removeMMBackgroundListener(this);

         windowClosingDone_ = true;         
         closed_ = true;
      }

      /*
       * Removes the VirtualAcquisitionDisplay from the Acquisition Manager.
       */
      private void removeMeFromAcquisitionManager(MMStudioMainFrame gui) {
         for (String name : gui.getAcquisitionNames()) {
            try {
               if (gui.getAcquisition(name).getAcquisitionWindow() == VirtualAcquisitionDisplay.this) {
                  gui.closeAcquisition(name);
               }
            } catch (Exception ex) {
               ReportingUtils.logError(ex);
            }
         }
      }

      @Override
      public void windowClosed(WindowEvent E) {
         try {
            // NS: I do not know why this line was here.  It causes problems since the windowClosing
            // function now will often run twice 
            //this.windowClosing(E);
            super.windowClosed(E);
         } catch (NullPointerException ex) {
               ReportingUtils.showError(ex, "Null pointer error in ImageJ code while closing window");
         }
      }

      @Override
      public void windowActivated(WindowEvent e) {
         if (!isClosed()) {
            super.windowActivated(e);
         }
      }

      @Override
      public void setAnimate(boolean b) {
         if (((IMMImagePlus) hyperImage_).getNFramesUnverified() > 1) {
            animateFrames(b);
         } else {
            animateSlices(b);
         }
      }

      @Override
      public boolean getAnimate() {
         return isAnimated();
      }
   };
}
