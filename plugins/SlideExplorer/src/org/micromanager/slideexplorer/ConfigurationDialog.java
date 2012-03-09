/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * ConfigurationDialog.java
 *
 * Created on Nov 12, 2009, 12:14:10 PM
 */

package org.micromanager.slideexplorer;

import java.text.ParseException;
import javax.swing.JTextField;
import javax.swing.table.AbstractTableModel;
import mmcorej.CMMCore;
import org.micromanager.slideexplorer.Hub.OffsetsRow;
import org.micromanager.utils.NumberUtils;
import org.micromanager.utils.ReportingUtils;

/**
 *
 * @author arthur
 */
public class ConfigurationDialog extends javax.swing.JFrame {
    private final Hub hub_;
    private final CMMCore core_;

    /** Creates new form ConfigurationDialog */

    public ConfigurationDialog(CMMCore core, Hub hub) {

        hub_ = hub;
        core_ = core;

        initComponents();

        navigationOffsetsTable.putClientProperty("terminateEditOnFocusLost", true);
        
        loadValues();
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
   // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
   private void initComponents() {

      closeButton = new javax.swing.JButton();
      applyButton = new javax.swing.JButton();
      jScrollPane1 = new javax.swing.JScrollPane();
      navigationOffsetsTable = new javax.swing.JTable();
      jLabel1 = new javax.swing.JLabel();

      setTitle("SlideExplorer Configuration");
      setResizable(false);
      addWindowFocusListener(new java.awt.event.WindowFocusListener() {
         public void windowGainedFocus(java.awt.event.WindowEvent evt) {
            formWindowGainedFocus(evt);
         }
         public void windowLostFocus(java.awt.event.WindowEvent evt) {
         }
      });
      addWindowListener(new java.awt.event.WindowAdapter() {
         public void windowClosing(java.awt.event.WindowEvent evt) {
            formWindowClosing(evt);
         }
      });

      closeButton.setText("Close");
      closeButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(java.awt.event.ActionEvent evt) {
            closeButtonActionPerformed(evt);
            closeButtonActionPerformed1(evt);
         }
      });

      applyButton.setText("Apply");
      applyButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(java.awt.event.ActionEvent evt) {
            applyButtonActionPerformed(evt);
         }
      });

      navigationOffsetsTable.setFont(new java.awt.Font("Lucida Grande", 0, 10)); // NOI18N
      navigationOffsetsTable.setModel(new OffsetTableModel());
      navigationOffsetsTable.getTableHeader().setReorderingAllowed(false);
      jScrollPane1.setViewportView(navigationOffsetsTable);

      jLabel1.setText("Navigation offsets (parcentricity & parfocality):");

      org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
      getContentPane().setLayout(layout);
      layout.setHorizontalGroup(
         layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
         .add(layout.createSequentialGroup()
            .addContainerGap()
            .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
               .add(layout.createSequentialGroup()
                  .add(applyButton)
                  .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                  .add(closeButton))
               .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 395, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
               .add(jLabel1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 331, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
            .addContainerGap(30, Short.MAX_VALUE))
      );
      layout.setVerticalGroup(
         layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
         .add(layout.createSequentialGroup()
            .addContainerGap()
            .add(jLabel1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 14, Short.MAX_VALUE)
            .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
            .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 119, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
            .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
            .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
               .add(applyButton)
               .add(closeButton))
            .addContainerGap())
      );

      java.awt.Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
      setBounds((screenSize.width-445)/2, (screenSize.height-236)/2, 445, 236);
   }// </editor-fold>//GEN-END:initComponents

    private void closeButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_closeButtonActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_closeButtonActionPerformed

    private void applyButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_applyButtonActionPerformed
        //double pixelSize = getPixelSizeValue();
        //hub_.setPixelSize(pixelSize);
        //navigationOffsetsTable.getDefaultEditor(null)
                
        hub_.saveOffsets();
        hub_.reapplyOffsets();


    }//GEN-LAST:event_applyButtonActionPerformed

    private void closeButtonActionPerformed1(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_closeButtonActionPerformed1
        dispose();
    }//GEN-LAST:event_closeButtonActionPerformed1

    private void formWindowClosing(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_formWindowClosing
        this.dispose();
    }//GEN-LAST:event_formWindowClosing

    private void formWindowGainedFocus(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_formWindowGainedFocus
        loadValues();
    }//GEN-LAST:event_formWindowGainedFocus



   // Variables declaration - do not modify//GEN-BEGIN:variables
   private javax.swing.JButton applyButton;
   private javax.swing.JButton closeButton;
   private javax.swing.JLabel jLabel1;
   private javax.swing.JScrollPane jScrollPane1;
   private javax.swing.JTable navigationOffsetsTable;
   // End of variables declaration//GEN-END:variables

    private void loadValues() {

    }


    private double getEntryDoubleValue(JTextField textField) {
        try {
            return NumberUtils.displayStringToDouble(textField.getText());
        } catch (ParseException ex) {
            return 0.0;
        }
    }

    private void setEntryValue(JTextField textField, double value) {
        String text = NumberUtils.doubleToDisplayString(value);
        textField.setText(text);
    }

    private class OffsetTableModel extends AbstractTableModel {

        
        private int numRows_ = 3;


        String columnNames_[] = { "Resolution setting", "x-offset, \u03BCm", "y-offset, \u03BCm", "z-offset, \u03BCm" };
        private final Object[] tableData_;


        public OffsetTableModel() {
            tableData_ = hub_.getOffsets().values().toArray();
        }

        public int getRowCount() {
            return tableData_.length;
        }

        public int getColumnCount() {
            return 4;
        }

        public String getColumnName(int columnIndex) {
            return columnNames_[columnIndex];
        }


        public boolean isCellEditable(int rowIndex, int columnIndex) {
            if (columnIndex != 0) {
                return true;
            } else {
                return false;
            }
        }

        public Object getValueAt(int rowIndex, int columnIndex) {
            OffsetsRow offsetsRow = (OffsetsRow) tableData_[rowIndex];
            Object result = null;

            switch (columnIndex) {
                case 0:
                    result = offsetsRow.resolutionName;
                    break;
                case 1:
                    result = NumberUtils.doubleToDisplayString(offsetsRow.x);
                    break;
                case 2:
                    result = NumberUtils.doubleToDisplayString(offsetsRow.y);
                    break;
                case 3:
                    result = NumberUtils.doubleToDisplayString(offsetsRow.z);
                    break;
            }

            return result;
        }

        public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
            OffsetsRow offsetsRow = (OffsetsRow) tableData_[rowIndex];
            try {
                switch (columnIndex) {

                    case 1:
                        offsetsRow.x = NumberUtils.displayStringToDouble(aValue);
                        break;
                    case 2:
                        offsetsRow.y = NumberUtils.displayStringToDouble(aValue);
                        break;
                    case 3:
                        offsetsRow.z = NumberUtils.displayStringToDouble(aValue);
                        break;
                }
            } catch (java.text.ParseException e) {
                ReportingUtils.showError("Please enter a valid number.");
            }
        }



    }

}
