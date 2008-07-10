;; gto.el --- Major mode for editing GTO text files

;; Copyright (C) 2007 Jim Hourihan

;; Author:	Jim Hourihan

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.

;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs; see the file COPYING.  If not, write to the
;; Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

;;; Commentary:
;;

;; == doc-mode settings

(defvar gto-mode-hook nil
  "Run when entering gto mode.")

(defvar gto-mode-abbrev-table nil
  "Abbrev table in use in gto-mode buffers.")
(define-abbrev-table 'gto-mode-abbrev-table ())

(defvar gto-font-lock-keywords
'( 
  ;; Header
  ("\\(GTOa\\) (\\([0-9]+\\))" 
   (1 'font-lock-keyword-face)
   (2 'font-lock-constant-face))

  ;; comment
  ("\\(#\\)\\(.*\\)$" 
   (1 'font-lock-comment-delimiter-face)
   (2 'font-lock-comment-face))

  ;; Object names
  ("\\(\\sw+\\|\".+\"\\)\\s-*:\\s-*\\(\\sw+\\|\".+\"\\)\\s-+(\\([0-9]+\\))"
   (1 'font-lock-function-name-face)
   (2 'italic)
   (3 'font-lock-constant-face))

  ;; Component names
  ("\\(\\sw+\\|\".+\"\\)\\s-*{"
   (1 'bold))

  ;; Property definitions array form
  ("\\b\\(byte\\|short\\|float\\|int\\|string\\|double\\|half\\|bool\\)\\b\\(?:\\[\\([0-9]+\\)+\\]\\)\\s-+\\(\\sw+\\|\"[^\"]+\"\\)"
   (1 'font-lock-type-face)
   (2 'font-lock-constant-face)
   (3 'font-lock-variable-name-face))

  ;; Property definitions
  ("\\b\\(byte\\|short\\|float\\|int\\|string\\|double\\|half\\|bool\\)\\s-+\\(\\sw+\\|\"[^\"]+\"\\)"
   (1 'font-lock-type-face)
   (2 'font-lock-variable-name-face))

  ;; keywords
  ("\\b\\(as\\|\\.\\.\\.\\)\\b" (1 'font-lock-keyword-face))

  ("\\({%%debug%%}\\)" (1 'font-lock-preprocessor-face))

  ;; quoted strings
  ("\\(\"[^\"]*\"\\)" (1 'font-lock-string-face))

  )
"Default expressions to highlight in GTO mode.")

(defun gto-list-objects ()
  "Show all of the object definitions in an Occur buffer"
  (interactive)
  (occur
   "\\(\\sw+\\|\".+\"\\)\\s-*:\\s-*\\(\\sw+\\|\".+\"\\)\\s-+(\\([0-9]+\\))"))

;;###autoload
(define-derived-mode gto-mode text-mode "gto"
  "Major mode for editing GTO text files.
 Turning on gto mode runs `gto-mode-hook'.
"
  (interactive)
  (modify-syntax-entry ?\'  ".")
  ;(flyspell-mode nil)

  (make-local-variable 'paragraph-start)
  (setq paragraph-start (concat "$\\|>" page-delimiter))
  (make-local-variable 'paragraph-separate)
  (setq paragraph-separate paragraph-start)
  (make-local-variable 'paragraph-ignore-fill-prefix)
  (setq paragraph-ignore-fill-prefix t)

  (make-local-variable 'require-final-newline)
  (setq require-final-newline t)

  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults 
	'(gto-font-lock-keywords 
	  nil				; KEYWORDS-ONLY: no
	  nil				; CASE-FOLD: no
	  ((?_ . "w"))			; SYNTAX-ALIST
	  ))
  (run-hooks 'gto-mode-hook))

(provide 'gto-mode)

;;}}}

;;; gto-mode.el ends here

