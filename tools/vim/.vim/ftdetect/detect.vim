augroup filetypedetect

au BufRead,BufNewFile SConstruct        setf python
au BufRead,BufNewFile Sconstruct        setf python
au BufRead,BufNewFile sconstruct        setf python
au BufRead,BufNewFile waf               setf python

au BufRead,BufNewFile *.txt             setf txt
au BufRead,BufNewFile *.log             setf txt

au BufRead,BufNewFile *.gcov            setf gcov

au BufRead,BufNewFile *.vprj            setf vimproj

au BufRead,BufNewFile *.ibs             setf vb

au BufRead,BufNewFile *.scr             setf apdu
augroup END
