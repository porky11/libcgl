(use-modules (ice-9 optargs))
(use-modules (ice-9 receive))

(define *fragments* '())
(define* (shader-fragment name code #:key (uniforms '()))
  (set! *fragments* (cons (list name code uniforms) *fragments*)))

(define (find-fragment name)
  (let loop ((frags *fragments*))
    (if (null? frags)
        #f
        (if (string=? (car (car frags)) name)
            (car frags)
            (loop (cdr frags))))))

(define (handle-fragments code)
  (if code
      (let ((pos (string-contains code ",(use ")))
        (if pos
            (let* ((q1 (string-contains code "\"" pos))
                   (q2 (string-contains code "\"" (1+ q1)))
                   (end (string-contains code ")" q2))
                   (fragment-name (substring code (1+ q1) q2))
                   (pre (substring code 0 pos))
                   (post (substring code (1+ end)))
                   (fragment (find-fragment fragment-name)))
              (let ((new-code (string-append pre (cadr fragment) post))
                    (new-uniforms (caddr fragment)))
                (receive (newer-code newer-uniforms) (handle-fragments new-code)
                  (values newer-code (append new-uniforms newer-uniforms)))))
            (values code '())))
      (values #f '())))
      
(define make-shader-ll make-shader)
(define* (make-shader name #:key vertex-shader fragment-shader geometry-shader inputs uniforms)
  ;(format #t "make shader ~a ~a ~a~%" name inputs uniforms)
  (let ((old-shader (find-shader name)))
    (if old-shader
	    (destroy-shader old-shader)))
  (receive (vert-source vert-uniforms) (handle-fragments vertex-shader)
    (receive (frag-source frag-uniforms) (handle-fragments fragment-shader)
      (receive (geom-source geom-uniforms) (handle-fragments geometry-shader)
        (let* ((addiditonal-uniforms (append vert-uniforms frag-uniforms geom-uniforms))
               (uniforms (append (if uniforms uniforms '()) addiditonal-uniforms))
          	   (shader (make-shader-ll name (length inputs) (length uniforms))))
          (add-vertex-source shader vert-source)
          (add-fragment-source shader frag-source)
          (if geom-source (add-geometry-source shader geom-source))
          (let rec ((rest inputs) (i 0))
            (add-shader-input shader (car rest) i)
            (if (not (null? (cdr rest)))
                (rec (cdr rest) (1+ i))))
          (for-each (lambda (u) 
                      ;(format #t "adding uniform ~a~%" u) 
                      (add-shader-uniform shader u)) 
                    uniforms)
          (if (not (compile-and-link-shader shader))
              (begin
          	  (format #t "info log for shader ~a~%" name)
          	  (format #t "vertex info log:~%~a~%---------~%" (vertex-shader-info-log shader))
          	  (format #t "fragment info log:~%~a~%---------~%" (fragment-shader-info-log shader))
          	  (if geometry-shader (format #t "geometry info log:~%~a~%---------~%" (geometry-shader-info-log shader)))
          	  (format #t "linker info log:~%~a~%---------~%" (shader-link-info-log shader))))
          shader)))))

