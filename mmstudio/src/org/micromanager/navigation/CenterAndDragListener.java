/**
 * 
 */
package org.micromanager.navigation;

import ij.gui.ImageCanvas;
import ij.gui.ImageWindow;
import ij.gui.Toolbar;
import java.awt.Event;
import java.awt.Window;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.util.Arrays;
import javax.swing.JOptionPane;
import mmcorej.CMMCore;
import mmcorej.MMCoreJ;
import org.micromanager.MMStudioMainFrame;
import org.micromanager.utils.ReportingUtils;

/**
 * @author OD
 *
 */
public class CenterAndDragListener implements MouseListener, MouseMotionListener {
      private CMMCore core_;
      private MMStudioMainFrame gui_;
	   private ImageCanvas canvas_;
	   private static boolean isRunning_ = false;
	   private boolean mirrorX_;
	   private boolean mirrorY_;
	   private boolean transposeXY_;
	   private boolean correction_;
	   private int lastX_, lastY_;

	   public CenterAndDragListener(MMStudioMainFrame gui) {
         gui_ = gui;
         core_ = gui.getMMCore();
	   }

	   public void start () {
	      if (isRunning_)
	         return;

	      isRunning_ = true;

	      // Get a handle to the Live window
         ImageWindow win = gui_.getImageWin();
         if (win != null) {
            attach(win);
         }
	   }

	   public void stop() {
	      if (canvas_ != null) {
	         canvas_.removeMouseListener(this);
	         canvas_.removeMouseMotionListener(this);
	      }
	      isRunning_ = false;
	   }

	   public boolean isRunning() {
	      return isRunning_;
	   }

      /*
       * Attached a MouseLisetener and MouseMotionListener to the Live Window
       */
	   public void attach(ImageWindow win) {
         if (win == null)
            return;
	      if (!isRunning_)
	         return;
	      canvas_ = win.getCanvas();
         if (!Arrays.asList(canvas_.getMouseListeners()).contains(this)) {
            canvas_.addMouseListener(this);
         }
         if (!Arrays.asList(canvas_.getMouseMotionListeners()).contains(this)) {
            canvas_.addMouseMotionListener(this);
         }

	      getOrientation();
	   }

      /*
       * Ensures that the stage moves in the expected direction
       */
	   public void getOrientation() {
	      String camera = core_.getCameraDevice();
	      if (camera == null) {
	         JOptionPane.showMessageDialog(null, "This function does not work without a camera");
	         return;
	      }
	      try {
	         String tmp = core_.getProperty(camera, "TransposeCorrection");
	         if (tmp.equals("0")) 
	            correction_ = false;
	         else 
	            correction_ = true;
	         tmp = core_.getProperty(camera, MMCoreJ.getG_Keyword_Transpose_MirrorX());
	         if (tmp.equals("0")) 
	            mirrorX_ = false;
	         else 
	            mirrorX_ = true;
	         tmp = core_.getProperty(camera, MMCoreJ.getG_Keyword_Transpose_MirrorY());
	         if (tmp.equals("0")) 
	            mirrorY_ = false;
	         else 
	            mirrorY_ = true;
	         tmp = core_.getProperty(camera, MMCoreJ.getG_Keyword_Transpose_SwapXY());
	         if (tmp.equals("0")) 
	            transposeXY_ = false;
	         else 
	            transposeXY_ = true;
	      } catch(Exception exc) {
	         ReportingUtils.showError(exc);
	         return;
	      }
	   }

	   public void mouseClicked(MouseEvent e) {
         if (Toolbar.getInstance() != null) {
            if (Toolbar.getToolId() == Toolbar.HAND) {
               // right click and single click: ignore
               int nc=   e.getClickCount();
               if ((e.getModifiers() & Event.META_MASK) != 0 || nc < 2) 
                   return;

               // Get needed info from core
               getOrientation();
               String xyStage = core_.getXYStageDevice();
               if (xyStage == null)
                  return;
               double pixSizeUm = core_.getPixelSizeUm();
               if (! (pixSizeUm > 0.0)) {
                  JOptionPane.showMessageDialog(null, "Please provide pixel size calibration data before using this function");
                  return;
               }

               int width = (int) core_.getImageWidth();
               int height = (int) core_.getImageHeight();

               // Get coordinates of event
               int x = e.getX();
               int y = e.getY();
               int cX = canvas_.offScreenX(x);
               int cY = canvas_.offScreenY(y);

               // calculate needed relative movement
               double tmpXUm = ((0.5 * width) - cX) * pixSizeUm;
               double tmpYUm = ((0.5 * height) - cY) * pixSizeUm;

               double mXUm = tmpXUm;
               double mYUm = tmpYUm;
               // if camera does not correct image orientation, we'll correct for it here:
               if (!correction_) {
                  // Order: swapxy, then mirror axis
                  if (transposeXY_) {mXUm = tmpYUm; mYUm = tmpXUm;}
                  if (mirrorX_) {mXUm = -mXUm;}
                  if (mirrorY_) {mYUm = -mYUm;}
               }

               // Move the stage
               try {
                  core_.setRelativeXYPosition(xyStage, mXUm, mYUm);
               } catch (Exception ex) {
                  ReportingUtils.showError(ex);
                  return;
               }

               // refresh GUI x,y
               gui_.updateXYStagePosition();
            }
         }
      } 


	   // Mouse listener implementation
	   public void mousePressed(MouseEvent e) {
	      // Get the starting coordinate for the dragging
	      int x = e.getX();
	      int y = e.getY();
	      lastX_ = canvas_.offScreenX(x);
	      lastY_ = canvas_.offScreenY(y);
	   }

	   public void mouseDragged(MouseEvent e) {
	      if ((e.getModifiers() & Event.META_MASK) != 0) // right click: ignore
	         return;

	    	// only respond when the Hand tool is selected in the IJ Toolbat
	      if (Toolbar.getInstance() != null) {
			if (Toolbar.getToolId() != Toolbar.HAND)
               return;
         }

	      // Get needed info from core
         // (is it really needed to run this every time?)
	      getOrientation();
	      String xyStage = core_.getXYStageDevice();
	      if (xyStage == null)
	         return;
	      try {
	         if (core_.deviceBusy(xyStage))
	            return;
	      } catch (Exception ex) {
	         ReportingUtils.showError(ex);
	         return;
	      }

	      double pixSizeUm = core_.getPixelSizeUm();
	      if (! (pixSizeUm > 0.0)) {
	         JOptionPane.showMessageDialog(null, "Please provide pixel size calibration data before using this function");
	         return;
	      }

	      // Get coordinates of event
	      int x = e.getX();
	      int y = e.getY();
	      int cX = canvas_.offScreenX(x);
	      int cY = canvas_.offScreenY(y);

	      // calculate needed relative movement
	      double tmpXUm = cX - lastX_;
	      double tmpYUm = cY - lastY_;

	      tmpXUm *= pixSizeUm;
	      tmpYUm *= pixSizeUm;
	      double mXUm = tmpXUm;
	      double mYUm = tmpYUm;
	      // if camera does not correct image orientation, we'll correct for it here:
	      if (!correction_) {
	         // Order: swapxy, then mirror axis
	         if (transposeXY_) {mXUm = tmpYUm; mYUm = tmpXUm;}
	         if (mirrorX_) {mXUm = -mXUm;}
	         if (mirrorY_) {mYUm = -mYUm;}
	      }

	      // Move the stage
	      try {
	         core_.setRelativeXYPosition(xyStage, mXUm, mYUm);
	      } catch (Exception ex) {
	         ReportingUtils.showError(ex);
	         return;
	      }
	      lastX_ = cX;
	      lastY_ = cY;

         gui_.updateXYPosRelative(mXUm, mYUm);

	   } 

	   public void mouseReleased(MouseEvent e) {}
	   public void mouseExited(MouseEvent e) {}
	   public void mouseEntered(MouseEvent e) {}
	   public void mouseMoved(MouseEvent e) {}

}
