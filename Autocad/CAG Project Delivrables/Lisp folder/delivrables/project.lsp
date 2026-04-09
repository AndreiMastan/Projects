(defun c:concentric ( / ss1 i userInput entname ent entStartPoint entEndPoint entMidPoint dx dy newStartPoint newEndPoint entCenter newCenter)
  ;; Select all line and circle entities in the drawing
  (setq ss1 (ssget "x" '((0 . "LINE,CIRCLE"))))
  (setq i 0)
  ;; Get the new point from the user
  (setq userInput (getpoint "\nPlease give me a new point for all midpoints and centers: "))

  ;; Iterate over each selected entity
  (repeat (sslength ss1)
    (setq entname (ssname ss1 i)) ; Get the entity name
    (setq ent (entget entname))   ; Get the entity data list

    (cond
      ;; If the entity is a line
      ((eq (cdr (assoc 0 ent)) "LINE")
       ;; Get the start and end points of the line
       (setq entStartPoint (cdr (assoc 10 ent)))
       (setq entEndPoint (cdr (assoc 11 ent)))
       ;; Calculate the current midpoint of the line
       (setq entMidPoint (list (/ (+ (car entStartPoint) (car entEndPoint)) 2)
                               (/ (+ (cadr entStartPoint) (cadr entEndPoint)) 2)))
       ;; Calculate the displacement needed to move the midpoint to the user-specified point
       (setq dx (- (car userInput) (car entMidPoint)))
       (setq dy (- (cadr userInput) (cadr entMidPoint)))
       ;; Calculate the new start and end points
       (setq newStartPoint (list (+ (car entStartPoint) dx)
                                 (+ (cadr entStartPoint) dy)))
       (setq newEndPoint (list (+ (car entEndPoint) dx)
                               (+ (cadr entEndPoint) dy)))
       ;; Update the start and end points in the entity data list
       (setq ent (subst (cons 10 newStartPoint) (assoc 10 ent) ent))
       (setq ent (subst (cons 11 newEndPoint) (assoc 11 ent) ent))
      )

      ;; If the entity is a circle
      ((eq (cdr (assoc 0 ent)) "CIRCLE")
       ;; Get the center point of the circle
       (setq entCenter (cdr (assoc 10 ent)))
       ;; Calculate the displacement needed to move the center to the user-specified point
       (setq dx (- (car userInput) (car entCenter)))
       (setq dy (- (cadr userInput) (cadr entCenter)))
       ;; Calculate the new center point
       (setq newCenter (list (+ (car entCenter) dx)
                             (+ (cadr entCenter) dy)))
       ;; Update the center point in the entity data list
       (setq ent (subst (cons 10 newCenter) (assoc 10 ent) ent))
      )
    )

    ;; Modify the entity with the updated data list
    (entmod ent)
    (entupd entname) ; Ensure the change is applied

    ;; Increment the loop counter
    (setq i (+ i 1))
  )
  (princ) ; Exit quietly
)
