
package org.micromanager.intelligentacquisition;

import ij.IJ;
import ij.ImagePlus;
import ij.gui.Roi;
import ij.measure.ResultsTable;
import ij.text.TextPanel;
import ij.text.TextWindow;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import org.micromanager.api.MMWindow;


/**
 *
 * @author nico
 */
/**
 * KeyListener and MouseListenerclass for ResultsTable
 * When user selected a line in the ResulsTable and presses a key,
 * the corresponding image will move to the correct slice and draw the ROI
 * that was used to calculate the Gaussian fit
 * Works only in conjunction with appropriate column names
 * Up and down keys also work as expected
 */
public class ResultsListener implements KeyListener, MouseListener{
  
   ImagePlus siPlus_;
   ResultsTable res_;
   TextWindow win_;
   TextPanel tp_;
   int hBS_;
   public ResultsListener(ImagePlus siPlus, ResultsTable res, TextWindow win, int halfBoxSize) {
      siPlus_ = siPlus;
      res_ = res;
      win_ = win;
      tp_ = win.getTextPanel();
      hBS_ = halfBoxSize;
   }
   public void keyPressed(KeyEvent e) {
      int key = e.getKeyCode();
      int row = tp_.getSelectionStart();
      if (key == KeyEvent.VK_J) {
         if (row > 0) {
            row--;
            tp_.setSelection(row, row);
         }
      } else if (key == KeyEvent.VK_K) {
         if  (row < tp_.getLineCount() - 1) {
            row++;
            tp_.setSelection(row, row);
         }
      }
      update();
   }
   public void keyReleased(KeyEvent e) {}
   public void keyTyped(KeyEvent e) {}

   public void mouseReleased(MouseEvent e) {
      update();
   }
   public void mousePressed(MouseEvent e) {}
   public void mouseClicked(MouseEvent e) {}
   public void mouseEntered(MouseEvent e) {};
   public void mouseExited(MouseEvent e) {};

   private void update() {
      if (siPlus_ == null) {
         return;
      }
      int row = tp_.getSelectionStart();
      if (row >= 0 && row < tp_.getLineCount()) {
         if (siPlus_.getWindow() != null) {
            if (siPlus_ != IJ.getImage()) {
               siPlus_.getWindow().toFront();
               win_.toFront();
            }
         } else {
            siPlus_ = null;
            return;
         }
         try {
            // trick to make thing work also without Micro-Manager
            Class mmw = Class.forName("org.micromanager.api.MMWindow");
            MMWindow mw = new MMWindow(siPlus_);
            if (mw.isMMWindow()) {
               try {
                  //int position = (int) res_.getValue(Terms.POSITION, row);
                  //mw.setPosition(position);
               } catch (Exception ex) {
               }
            }
         } catch (ClassNotFoundException ex) {

         }

        // int frame = (int) res_.getValue(Terms.FRAME, row);
         //int slice = (int) res_.getValue(Terms.SLICE, row);
         //int channel = (int) res_.getValue(Terms.CHANNEL, row);
         //int x = (int) res_.getValue(Terms.XPIX, row);
         //int y = (int) res_.getValue(Terms.YPIX, row);
         //if (siPlus_.isHyperStack())
           // siPlus_.setPosition(channel, slice, frame);
        //else
           // siPlus_.setPosition(Math.max(frame, slice));
         //siPlus_.setRoi(new Roi(x - hBS_, y - hBS_, 2 * hBS_, 2 * hBS_));
      }
   }

}

//}
