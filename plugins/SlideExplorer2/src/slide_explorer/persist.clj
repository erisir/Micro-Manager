(ns slide-explorer.persist
  (:import (org.micromanager.utils FileDialogs FileDialogs$FileType))
  (:require [clojure.java.io :as io]))

;; file dialogs

(def DATA-SET
  (FileDialogs$FileType. "SX2_DATA_SET",
                         "Slide Explorer Data Set",
                         (str (System/getProperty "user.home") "/Untitled")
                         false  nil))

(defn create-dir-dialog
  "Shows a file dialog to allow the user to create a directory."
  []
  (FileDialogs/save nil "Create Slide Explorer data set" DATA-SET))

(defn open-dir-dialog
  "Shows a file dialog to allow the user to choose a data set
   for opening. Returns the chosen path or nil if canceled."
  []
  (FileDialogs/openDir nil "Choose a Slide Explorer data set" DATA-SET))

(defn save-as
  "Ask the user to choose a new data set location and move an existing
   data set there."
  [old-location]
  (when-let [new-location (create-dir-dialog)]
    (when (.renameTo (io/file old-location) new-location)
      new-location)))
    